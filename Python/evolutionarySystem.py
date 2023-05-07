from maze_world import MazeWorld
from generate import RobotGenerator
from genetic import mixAndMutate
import numpy as np

if __name__ == "__main__":

    nGenomes = 10
    nColors = 6
    nParticles = 30
    genomes = [np.random.randint(low=-36,high=36,size=(nColors*nColors + nParticles)) for _ in  range(nGenomes)]
    scores = np.ones(nGenomes) * 1000

    for _ in range(10):

        for i,genome in enumerate(genomes):
            rGen = RobotGenerator(nColors,40,40)

            for m in range(nColors):
                for n in range(nColors):
                    rGen.add_rules(m,n,genome[n]/36)

            for n in range(nColors*nColors, len(genome), 3):
                x = abs(genome[n])/36
                y = abs(genome[n])/36
                color = int(abs(genome[n])%6)
                rGen.add_particles(x,y,color)

            rGen.save(f"tmpParticle{i}.json")
            mw = MazeWorld(f"tmpParticle{i}.json")
            mw.silentLoop()
            scores[i] = mw.score

        genomes = mixAndMutate(genomes,scores,maxPopulation=nGenomes,genomePurifying=True,minRange=-36,maxRange=36)

    theBest = np.argmin(scores)
    mw = MazeWorld(f"tmpParticle{theBest}.json")
    mw.loop()

    pass