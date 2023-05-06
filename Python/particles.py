import numpy as np
import pygame
import json
import math
import time 

BLUE = np.array((0, 0, 255))
GREEN = np.array((0, 255, 0))
RED = np.array((255, 0, 0))
COLOR4 = np.array((0, 255, 255))
COLOR5 = np.array((255, 0, 255))
COLOR6 = np.array((255, 255, 0))

colorsTable = [(255,0,0), (0,255,0), (0,0,255), (255,255,0), (255,0,255), (0,255,255)]

spatialBucket = {}

def timeit(function):
    def timing(*args, **kwargs):
        start = time.perf_counter()

        retVal = function(*args, **kwargs)
        print(f"[{function.__name__}] performance: {time.perf_counter() - start}s")
        return retVal
    return timing


def F(r,a,B):

    if(r < B):
        return r/B - 1
    elif(r > B and r < 1):
        return a * (1 - abs(2 * r - 1 - B)/(1 - B))
    else:
        return 0

class ParticleSystem:

    def __init__(self,rMax,dt,Beta,friction):

        self.rMax = rMax
        self.dt = dt
        self.Beta = Beta
        self.friction = friction
        pass

    def loadSystem(self,pos_x,pos_y,screenDim,fileName):

        with open(fileName, "r") as fp:
            data = json.load(fp)

        positions = np.array(data['robot'])[:,0:2]

        positions[:,0] = ((positions[:,0] * data['width'])  + pos_x - data['width']/2)  /screenDim
        positions[:,1] = ((positions[:,1] * data['height']) + pos_y - data['height']/2)  /screenDim
        self.nParticles = len(positions[:,0])

        self.positions = positions
        self.colors = np.array(data['robot'][:],dtype=np.int32)[:,2]

        self.attractionMatrix = np.array(data['attraction'])
        self.velocities = np.zeros((self.nParticles,2))

    def generateRandomSystem(self,nParticles,nColors):
        self.nParticles = nParticles
        self.positions  = np.random.rand(nParticles,2)
        self.colors     = np.random.randint(0,nColors,size=nParticles)
        self.attractionMatrix = np.random.uniform(-1, 1, (nColors,nColors))
        self.velocities = np.zeros((self.nParticles,2))

    # you need to scale center_xy by screen dimension, same with range_xy
    def generateSpecificSystem(self,coloredParticleList,attractionMatrix):
        self.nParticles = len(coloredParticleList)
        self.attractionMatrix = attractionMatrix
        self.colors = coloredParticleList
        self.positions = np.random.rand(self.nParticles,2)
        self.velocities = np.zeros((self.nParticles,2))

    def updateVelocities(self):
        for n in range(self.nParticles):
            positionsToCompare = self.positions.copy()
            positionsToCompare = np.delete(positionsToCompare,n,axis=0)
            self.velocities[n] = self.updateVelocity(self.positions[n],self.colors[n],self.velocities[n],positionsToCompare,self.colors)
        return self.velocities

    def updateVelocity(self,position,color,velocity,positionsToCompare,colors):
        # calcuate forces
        totalForce = np.zeros(2)

        for j in range(len(positionsToCompare)):
            r = math.dist(position,positionsToCompare[j])
            if(0 < r and r < self.rMax):
                f = F(r/self.rMax, self.attractionMatrix[color][colors[j]],self.Beta)
                totalForce += ((positionsToCompare[j] - position)/r) * f

        totalForce *= self.rMax

        velocity *= self.friction
        velocity += totalForce * self.dt

        return velocity

    # @timeit
    def updatePositions(self):
        # # update particles
        for n in range(self.nParticles):
            self.positions[n] += self.velocities[n] * self.dt

        return self.positions

    def loop(self,drawingTask=None):
        self.velocities = self.updateVelocities()
        # # update particles
        self.positions = self.updatePositions()


# @timeit
def drawParticles(screen,nParticles,colors,positions,screenDim):
    # draw particles
    for n in range(nParticles):
        # draw the circle
        circle_radius = 2
        pygame.draw.circle(screen, colorsTable[colors[n]], positions[n]*screenDim, circle_radius)
