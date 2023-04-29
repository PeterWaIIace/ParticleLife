import numpy as np
import pygame
import math
import time 


if __name__=="__main__":


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

        # update the screen
        pygame.display.update()
        # pygame.display.flip()