import numpy as np
import pygame
import math
import time 

BLUE = np.array((0, 0, 255))
GREEN = np.array((0, 255, 0))
RED = np.array((255, 0, 0))

colorsTable = [BLUE, GREEN, RED]

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

# @timeit
def updateVelocities(nParticles,positions,colors,velocities,rMax,dt):
    for n in range(nParticles):
        positionsToCompare = positions.copy()
        positionsToCompare = np.delete(positionsToCompare,n)
        velocities[n] = updateVelocity(nParticles,positions[n],colors[n],velocities[n],positions,colors,rMax,dt)
    return velocities

def updateVelocity(nParticles,position,color,velocity,positionToCompare,colors,rMax,dt):
    # calcuate forces
    totalForce = np.zeros(2)

    for j in range(nParticles):
        r = math.dist(position,positionToCompare[j])
        if(0 < r and r < rMax):
            f = F(r/rMax, attractionMatrix[color][colors[j]],Beta)
            totalForce += ((positionToCompare[j] - position)/r) * f

    totalForce *= rMax

    velocity *= friction
    velocity += totalForce * dt

    return velocity

# @timeit
def updatePositions(nParticles,positions,dt):
    # # update particles
    for n in range(nParticles):
        positions[n] += velocities[n] * dt

    return positions

# @timeit
def drawParticles(nParticles,colors,positions,screenDim):
    # draw particles
    for n in range(nParticles):
        # draw the circle
        pygame.draw.circle(screen, colorsTable[colors[n]], positions[n]*screenDim, circle_radius)

def getHash(x,y,step):
    return math.ceil(x/step),math.ceil(y/step)

if __name__=="__main__":

    nColors = 3
    Beta = 0.01
    friction = 0.04
    rMax = 1 # max distance
    dt = 0.05
    maxForce = 1
    nParticles = 100

    attractionMatrix = np.random.uniform(-1, 1, (nColors,nColors))
    # attractionMatrix = np.array([[1.0,0.5,0],[0,1.0,0.5],[-0.4,0,1.0]])

    screenDim = 800

    positions  = np.random.rand(nParticles,2)
    velocities = np.zeros((nParticles,2))
    colors     = np.random.randint(0,nColors,size=nParticles)

    # initialize pygame
    pygame.init()

    # set the width and height of the screen
    screen = pygame.display.set_mode((screenDim, screenDim))

    # set the center and radius of the circle
    circle_center = (400, 300)
    circle_radius = 1

    # main loop
    while True:
        # handle events
        # clear the screen with the background color
        screen.fill((0,0,0))

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        drawParticles(nParticles,colors,positions,screenDim)

        velocities = updateVelocities(nParticles,positions,colors,velocities,rMax,dt)
        # # update particles
        positions = updatePositions(nParticles,positions,dt)
        # update the screen
        pygame.display.update()
        # pygame.display.flip()