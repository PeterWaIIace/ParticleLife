import numpy as np
import pygame
import math
import time 

BLUE = np.array((0, 0, 255))
GREEN = np.array((0, 255, 0))
RED = np.array((255, 0, 0))
COLOR4 = np.array((0, 255, 255))
COLOR5 = np.array((255, 0, 255))
COLOR6 = np.array((255, 255, 0))

colorsTable = [BLUE, GREEN, RED,COLOR4 , COLOR5, COLOR6]

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

        # self.nParticles = nParticles
        # self.positions  = np.random.rand(nParticles,2)
        # self.colors     = np.random.randint(0,nColors,size=nParticles)
        # self.attractionMatrix = np.random.uniform(-1, 1, (nColors,nColors))
        self.rMax = rMax
        self.dt = dt
        self.Beta = Beta
        self.friction = friction
        pass

    def generateRandomSystem(self,nParticles,nColors):
        self.nParticles = nParticles
        self.positions  = np.random.rand(nParticles,2)
        self.velocities = np.zeros((nParticles,2))
        self.colors     = np.random.randint(0,nColors,size=nParticles)
        self.attractionMatrix = np.random.uniform(-1, 1, (nColors,nColors))
        self.velocities = np.zeros((self.nParticles,2))

    # you need to scale center_xy by screen dimension, same with range_xy
    def generateSpecificSystem(self,center_xy,range_xy,coloredParticleList,attractionMatrix):
        self.nParticles = len(coloredParticleList)
        self.attractionMatrix = attractionMatrix
        self.colors = coloredParticleList
        self.positions = (center_xy - np.random.rand(self.nParticles,2))/range_xy
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

if __name__=="__main__":

    nColors = 6
    Beta = 0.01
    friction = 0.04
    rMax = 0.5 # max distance
    dt = 0.05
    nParticles = 100
    particles = ParticleSystem(nParticles,nColors,rMax,dt,Beta,friction)
    particles.generateRandomSystem()

    screenDim = 400
    # attractionMatrix = np.array([[1.0,0.5,0],[0,1.0,0.5],[-0.4,0,1.0]])

    # initialize pygame
    pygame.init()
    # set the width and height of the screen
    screen = pygame.display.set_mode((screenDim, screenDim))

    # main loop
    while True:
        # handle events
        # clear the screen with the background color
        screen.fill((0,0,0))

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        drawParticles(particles.nParticles,particles.colors,particles.positions,screenDim)

        particles.loop()

        # update the screen
        pygame.display.update()
        # pygame.display.flip()