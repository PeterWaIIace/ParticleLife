import numpy as np
import pygame
from particles import ParticleSystem, drawParticles
from genetic import mixAndMutate
import math
import time

class World:

    def __init__(self, genome):
        self.genome = genome
        self.nColors = 6
        attractionMatrix, listOfParticles = self.decodeGenome(self.genome)

        Beta = 0.01
        friction = 0.04
        rMax = 0.5 # max distance
        dt = 0.05
        self.particleSystem = ParticleSystem(rMax=rMax,dt=dt,friction=friction,Beta=Beta)
        self.particleSystem.generateSpecificSystem(np.array((0,0)),np.array((1,1)),listOfParticles,attractionMatrix)

    def decodeGenome(self,genome):
        attractionMatrix = np.zeros((self.nColors,self.nColors),dtype=np.int32)
        for n in range(self.nColors):
            for m in range(self.nColors):
                attractionMatrix[n][m] = int(genome[m+(n*self.nColors)]/32)

        offset = self.nColors*self.nColors
        listOfParticles = []
        for n in range(self.nColors):
            listOfParticles.append(np.ones((genome[n+offset])+32,dtype=np.int32) * n)

        listOfParticles = np.concatenate(listOfParticles)
        return attractionMatrix,listOfParticles

    def run(self):
        r = np.zeros(self.particleSystem.nParticles)
        for n in range(100):
            r = np.zeros(self.particleSystem.nParticles)

            self.particleSystem.loop()

            for n in range(self.particleSystem.nParticles):
                r[n] = math.dist(self.particleSystem.positions[n],np.array([target.x+target.width/2,target.y+target.height/2]))

        return np.median(r)


if __name__=="__main__":


    screenDim = 400

    # initialize pygame
    pygame.init()
    # set the width and height of the screen
    screen = pygame.display.set_mode((screenDim, screenDim))
    # main loop

    target = pygame.Rect(screenDim/2,screenDim/2,50,50)

    generation = 0
    nGenomes = 10
    genomes = [np.random.randint(low=-32,high=32,size=42) for _ in  range(nGenomes)]
    results = np.ones(nGenomes) * 1000
    while True:

        for n,genome in enumerate(genomes):
            w = World(genome)
            results[n] = w.run()
            print(results[n])

        genomes = mixAndMutate(genomes,results,maxPopulation=nGenomes,genomePurifying=True,minRange=-32,maxRange=32)


        # # r = np.zeros(particleSystem.nParticles)
        # # handle events
        # # clear the screen with the background color
        # screen.fill((0,0,0))

        # for event in pygame.event.get():
        #     if event.type == pygame.QUIT:
        #         pygame.quit()
        #         quit()

        # # particleSystem.loop()

        # # for n in range(particleSystem.nParticles):
        # #     r[n] = math.dist(particleSystem.positions[n],np.array([target.x+target.width/2,target.y+target.height/2]))

        # # print(np.median(r))
        # # pygame.draw.rect(screen, (0, 0, 255), target)
        # # drawParticles(screen,particleSystem.nParticles,particleSystem.colors,particleSystem.positions,screenDim)
        # # update the screen
        # pygame.display.update()
        # # pygame.display.flip()