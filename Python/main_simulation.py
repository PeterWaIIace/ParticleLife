from particles import ParticleSystem, drawParticles
import pygame

if __name__=="__main__":

    screenDim = 400

    pygame.init()
    screen = pygame.display.set_mode((screenDim, screenDim))

    Beta = 0.01
    friction = 0.04
    rMax = 0.5 # max distance
    dt = 0.05
    nColors = 6

    particleSystem = ParticleSystem(rMax=rMax,dt=dt,friction=friction,Beta=Beta)
    particleSystem.loadSystem(screenDim/2,screenDim/2,screenDim,"particleRobot.json")

    while True:
        screen.fill((0,0,0))

        particleSystem.loop()
        drawParticles(screen=screen,nParticles=particleSystem.nParticles,colors=particleSystem.colors,positions=particleSystem.positions,screenDim=screenDim)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()

        pygame.display.update()

    pygame.quit()
