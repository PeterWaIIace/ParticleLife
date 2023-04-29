import numpy as np
import random

def mixAdjacent(genomes,scores):
    genomes = sorted(zip(scores,genomes))

    new_genomes = []
    for n in range(0,len(genomes),2):
        new_genomes.append(crossover(genomes[n][1],genomes[n+1][1]))
        new_genomes.append(crossover(genomes[n+1][1],genomes[n][1]))

    return new_genomes

def mateRest(superior,rest):
    rest = rest
    if len(superior) > 0:
        for n,r in enumerate(rest):
            changes = np.random.randint(1,len(r))
            indexes = np.random.randint(0,len(r),changes)
            s_i = int((random.random()*len(superior)))
            rest[n][indexes] = superior[s_i][indexes]
    return rest

def mateSuperior(superior):
    if len(superior) > 0:
        for s in superior:
            indexes = np.random.randint(0,len(s)-1,int(len(s)/2))
            s_i_1 = int(random.random()*len(superior)-1)
            s_i_2 = int(random.random()*len(superior)-1)
            superior[s_i_1][indexes] = superior[s_i_2][indexes]
    return superior

def generateOffspring(parent1,parent2,population):
    offspring = []
    for n in range(population):
        genome = crossover(parent1,parent2)
        assert(len(genome)==len(parent1))
        offspring.append(genome)

    return offspring

def separateSuperior(genomes,scores,cmp =lambda score,mean : score < mean):
    superior = []
    rest     = []

    sortedGenomes = [x for _, x in sorted(zip(scores, genomes), key=lambda pair: pair[0])]
    superior = sortedGenomes[:2]
    rest = sortedGenomes[2:]

    return (superior,rest)


def mixRandomly(genomes,scores,cmp=lambda score,mean : score <= mean):
    genomes = sorted(zip(scores,genomes), key=lambda x: x[0])

    superior, rest= separateSuperior(genomes,scores,cmp=cmp)

    ## TO DO: add at least one superior gen
    rest = mateRest(superior,rest)
    superior = mateSuperior(superior)

    new_genomes = rest + superior
    return new_genomes


def crossover(genome1, genome2):
    crossover_point = np.random.randint(1, len(genome1))
    # crossover_point = int(min(len(genom1),len(genom2))/2)
    newGenome = genome2.copy()
    random_indices = np.random.choice(genome1.size, random.randint(1,len(genome1)), replace=False)
    newGenome[random_indices] = genome1[random_indices]

    return newGenome


# NoM - number of mutations
def mutate(genome,NoM,minRange=-2**16,maxRange=2**16):
    genome_cp = genome.copy()
    genome = genome.copy()

    while(genome_cp == genome).all():
        random_indices = np.random.choice(genome.size, random.randint(1,NoM), replace=False)
        genome[random_indices] += np.random.randint(minRange,maxRange,len(random_indices))
        # update the values outside the range to the nearest limit
        genome[genome < minRange] = minRange
        genome[genome > maxRange] = maxRange


    return genome

def __mutateMany(genomes,ms,rate=0.5,minRange=-2**16,maxRange=2**16):
    genomes = genomes.copy()
    if len(genomes):
        for n,genome in enumerate(genomes):
            if rate > random.random():
                genomes[n] = mutate(genome,ms,minRange,maxRange)

    return genomes


forbiddenGenomes = dict()
# check if genomes are in forbidden dict
def filterGenomes(genomes,minRange,maxRange):
    for n,genome in enumerate(genomes):
        while genome.tobytes() in forbiddenGenomes.keys():
            genome = mutate(genome,1,minRange=minRange,maxRange=maxRange)
        genomes[n] = genome

    return genomes

def addToForbiddenGenomes(genomes):
    for genome in genomes:
        forbiddenGenomes[genome.tobytes()] = 1

# msR - mutation strength of rest
# msS - mutation strength of superior
def mixAndMutate(genomes,scores,mr=0.5,ms=2,maxPopulation=50,genomePurifying=False,minRange=0,maxRange=0):

    assert(isinstance(genomes[0],np.ndarray))

    superior, rest= separateSuperior(genomes,scores)

    # ## TO DO: add at least one superior gen
    # offspring = mateSuperior(superior)

    offspring = generateOffspring(superior[0],superior[1],len(genomes))
    newGeneration = __mutateMany(offspring,ms,rate=mr,minRange=minRange,maxRange=maxRange)

    random.shuffle(newGeneration)
    newGeneration = newGeneration[:maxPopulation-1] + [superior[0]]
    # newGeneration = newGeneration[:maxPopulation-2] + superior
    # newGeneration = newGeneration[:]

    if genomePurifying == True:
        addToForbiddenGenomes(rest)
        newGeneration = filterGenomes(newGeneration,minRange,maxRange)

    assert(isinstance(newGeneration[0],np.ndarray))
    return newGeneration


def calculateScore(trainingVectors,outputVectors,trainingFunc):

    scores = []

    for tv,ov in zip(trainingVectors,outputVectors):
        scores.append(trainingFunc(tv,ov))

    return scores