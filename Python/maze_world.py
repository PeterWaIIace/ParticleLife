from particles import ParticleSystem, drawParticles
import pygame
import numpy as np
import time 


class MazeWorld:

    def __init__(self,robotName="particleRobot.json"):
        self.screenDim = 400
        self.maze = [["w","w","w","w","w","w","w"],
                     ["w","w","w","w","w","g","w"],
                     ["w","w","w","w","w","x","w"],
                     ["w","s","x","x","x","x","w"],
                     ["w","w","w","w","w","w","w"],
                     ["w","w","w","w","w","w","w"],
                     ["w","w","w","w","w","w","w"]]

        Beta = 0.01
        friction = 0.04
        rMax = 0.5 # max distance
        dt = 0.05

        square_height = self.screenDim/len(self.maze)
        square_width  = self.screenDim/len(self.maze[0])

        self.square_height = square_height/self.screenDim
        self.square_width  = square_width /self.screenDim

        start_x = 0
        start_y = 0

        self.goal_x = 0
        self.goal_y = 0
        self.score = 9999999999999999999

        for x,squares in enumerate(self.maze):
            for y,square in enumerate(squares):
                if square == "s":
                    start_x = x*self.square_width + self.square_width/2
                    start_y = y*self.square_height+ self.square_height/2
                if square == "g":
                    self.goal_x = x*self.square_width + self.square_width/2
                    self.goal_y = y*self.square_height+ self.square_height/2

        self.particleSystem = ParticleSystem(rMax=rMax,dt=dt,friction=friction,Beta=Beta)
        self.particleSystem.loadSystem(start_x * self.screenDim, start_y * self.screenDim,self.screenDim,robotName)

    def __drawMaze(self):
        for x,squares in enumerate(self.maze):
            for y,square in enumerate(squares):
                square_x = x*self.square_width
                square_y = y*self.square_height
                if square == "w":
                    pygame.draw.rect(self.screen, (40,0,40), pygame.Rect(square_x * self.screenDim, square_y * self.screenDim, self.square_width * self.screenDim, self.square_height * self.screenDim))
                elif square == "s":
                    pygame.draw.rect(self.screen, (10,10,0), pygame.Rect(square_x * self.screenDim, square_y * self.screenDim, self.square_width * self.screenDim, self.square_height * self.screenDim))
                elif square == "x":
                    pygame.draw.rect(self.screen, (0,0,0),   pygame.Rect(square_x * self.screenDim, square_y * self.screenDim, self.square_width * self.screenDim, self.square_height * self.screenDim))
                elif square == "g":
                    pygame.draw.rect(self.screen, (35,35,0),   pygame.Rect(square_x * self.screenDim, square_y * self.screenDim, self.square_width * self.screenDim, self.square_height * self.screenDim))

    def __collisions(self):
        # check collisions with maze
        for n,particle in enumerate(self.particleSystem.positions):
            for x,squares in enumerate(self.maze):
                for y,square in enumerate(squares):
                    square_y = y * self.square_height
                    square_x = x * self.square_width
                    if square == "w":
                        pos_x = particle[0]
                        pos_y = particle[1]

                        if pos_x > square_x and pos_x < square_x + self.square_width and pos_y > square_y and pos_y < square_y + self.square_height:

                            self.particleSystem.velocities[n][0] = 0
                            self.particleSystem.velocities[n][1] = 0

                            if abs(pos_x - square_x) > abs(pos_x - (square_x + self.square_width)):
                                self.particleSystem.positions[n][0] += 0.001
                            elif abs(pos_x - square_x) < abs(pos_x - (square_x + self.square_width)):
                                self.particleSystem.positions[n][0] -= 0.001

                            if abs(pos_y - square_y) > abs(pos_y - (square_y + self.square_height)):
                                self.particleSystem.positions[n][1] += 0.001
                            elif abs(pos_y - square_y) < abs(pos_y - (square_y + self.square_height)):
                                self.particleSystem.positions[n][1] -= 0.001

                            # pygame.draw.rect(self.screen, (50,0,50), pygame.Rect(square_x * self.screenDim, square_y * self.screenDim, square_width * self.screenDim, square_height * self.screenDim))

    def __score(self):
        self.score = 0.0
        for particle in self.particleSystem.positions:
            self.score += np.linalg.norm(np.array([self.goal_x,self.goal_y]) - particle)

        self.score /= len(self.particleSystem.positions)

    def silentLoop(self):
        for _ in range(500):
            self.particleSystem.updateVelocities()
            self.__collisions()
            self.particleSystem.updatePositions()

        self.__score()

    def loop(self):
        pygame.init()
        self.screen = pygame.display.set_mode((self.screenDim, self.screenDim))

        for _ in range(10000):
            self.screen.fill((0,0,0))

            # draw maze
            self.__drawMaze()
            self.particleSystem.updateVelocities()
            self.__collisions()
            self.particleSystem.updatePositions()

            drawParticles(screen=self.screen,nParticles=self.particleSystem.nParticles,colors=self.particleSystem.colors,positions=self.particleSystem.positions,screenDim=self.screenDim)

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()

            pygame.display.update()

        self.__score()
        pygame.quit()


if __name__=="__main__":

    mw = MazeWorld()
    mw.loop()