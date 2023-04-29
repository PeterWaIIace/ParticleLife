import numpy as np
from multiprocessing import Pool
from particles import ParticleSystem, drawParticles
from genetic import mixAndMutate
import math
import time
import pygame

class World:

    def __init__(self, genome):
        self.genome = genome
        self.nColors = 6
        positions, attractionMatrix, listOfParticles = self.decodeGenome(self.genome)

        Beta = 0.01
        friction = 0.04
        rMax = 0.5 # max distance
        dt = 0.05
        self.particleSystem = ParticleSystem(rMax=rMax,dt=dt,friction=friction,Beta=Beta)
        self.particleSystem.generateSpecificSystem(,listOfParticles,attractionMatrix)

    def decodeGenome(self,genome):
        attractionMatrix = np.zeros((self.nColors,self.nColors),dtype=np.float32)
        for n in range(self.nColors):
            for m in range(self.nColors):
                attractionMatrix[n][m] = genome[m+(n*self.nColors)]/32

        offset = self.nColors*self.nColors
        listOfParticles = []
        for n in range(self.nColors):
            listOfParticles.append(np.ones((genome[n+offset])+32,dtype=np.int32) * n)

        listOfParticles = np.concatenate(listOfParticles)
        return position,attractionMatrix,listOfParticles

    def run(self):
        r = np.zeros(self.particleSystem.nParticles)

        target_x = 0.75
        target_y = 0.75

        for n in range(50):

            self.particleSystem.loop()

            for n in range(self.particleSystem.nParticles):
                r[n] = math.dist(self.particleSystem.positions[n],np.array([target_x,target_y]))


        return np.median(r)

    def show(self):

        pygame.init()
        # set the width and height of the screen
        screenDim = 400
        screen = pygame.display.set_mode((screenDim, screenDim))

        r = np.zeros(self.particleSystem.nParticles)

        target_x = 0.75
        target_y = 0.75

        for n in range(100):

            screen.fill((0,0,0))

            self.particleSystem.loop()

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()

            drawParticles(screen,self.particleSystem.nParticles,self.particleSystem.colors,self.particleSystem.positions,screenDim)
            rect_x = target_x*screenDim
            rect_y = target_y*screenDim
            rect = pygame.Rect(rect_x, rect_y, 5, 5)
            pygame.draw.rect(screen, (250,125,125), rect, 5)


            # update the screen
            pygame.display.update()

        pygame.quit()

def runInstance(genome):
    w = World(genome)
    return w.run()

if __name__=="__main__":


    screenDim = 400

    # initialize pygame
    # pygame.init()
    # set the width and height of the screen
    # main loop

    # target = pygame.Rect(screenDim/2,screenDim/2,50,50)

    nGenomes = 10
    genomes = [np.random.randint(low=-32,high=32,size=42) for _ in  range(nGenomes)]
    results = np.ones(nGenomes) * 1000
    for generation in range(100):

        with Pool(processes=10) as pool:
            results = pool.map(runInstance,genomes)

        print(results,generation)
        best = np.argmin(results)
        bestGenome = genomes[best]

        w = World(genomes[best])
        w.show()

        results,generation

        genomes = mixAndMutate(genomes,results,maxPopulation=nGenomes,genomePurifying=True,minRange=-32,maxRange=32)

