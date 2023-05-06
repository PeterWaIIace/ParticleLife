from particles import ParticleSystem, drawParticles
import pygame
import numpy as np
import time 

maze = [["w","w","w","w","w"],
        ["w","w","w","w","w"],
        ["w","s","x","x","w"],
        ["w","w","w","w","w"],
        ["w","w","w","w","w"]]

if __name__=="__main__":

    screenDim = 400

    pygame.init()
    screen = pygame.display.set_mode((screenDim, screenDim))

    Beta = 0.01
    friction = 0.04
    rMax = 0.5 # max distance
    dt = 0.05
    nColors = 6

    square_height = screenDim/len(maze)
    square_width  = screenDim/len(maze[0])

    square_height = square_height/screenDim
    square_width  = square_width/screenDim

    start_x = 0
    start_y = 0

    for x,squares in enumerate(maze):
        for y,square in enumerate(squares):
            if square == "s":
                start_x = x*square_width + square_width/2
                start_y = y*square_height+ square_height/2

    particleSystem = ParticleSystem(rMax=rMax,dt=dt,friction=friction,Beta=Beta)
    particleSystem.loadSystem(start_x * screenDim, start_y * screenDim,screenDim,"particleRobot.json")

    while True:
        screen.fill((0,0,0))

        # draw maze
        for x,squares in enumerate(maze):
            for y,square in enumerate(squares):
                square_x = x*square_width
                square_y = y*square_height
                if square == "w":
                    pygame.draw.rect(screen, (10,0,10), pygame.Rect(square_x * screenDim, square_y * screenDim, square_width * screenDim, square_height * screenDim))
                elif square == "s":
                    pygame.draw.rect(screen, (10,10,0), pygame.Rect(square_x * screenDim, square_y * screenDim, square_width * screenDim, square_height * screenDim))
                elif square == "x":
                    pygame.draw.rect(screen, (0,0,0),   pygame.Rect(square_x * screenDim, square_y * screenDim, square_width * screenDim, square_height * screenDim))

        particleSystem.updateVelocities()

        # check collisions with maze
        for n,particle in enumerate(particleSystem.positions):
            for x,squares in enumerate(maze):
                for y,square in enumerate(squares):
                    square_y = y * square_height
                    square_x = x * square_width
                    if square == "w":
                        pos_x = particle[0]
                        pos_y = particle[1]

                        if pos_x > square_x and pos_x < square_x + square_width and pos_y > square_y and pos_y < square_y + square_height:
                            print(pos_x,square_x,pos_y,square_y)
                            pygame.draw.rect(screen, (50,0,50), pygame.Rect(square_x * screenDim, square_y * screenDim, square_width * screenDim, square_height * screenDim))

        particleSystem.updatePositions()
        drawParticles(screen=screen,nParticles=particleSystem.nParticles,colors=particleSystem.colors,positions=particleSystem.positions,screenDim=screenDim)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()

        pygame.display.update()

    pygame.quit()
