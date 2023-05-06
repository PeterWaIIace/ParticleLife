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
    nParticles = 100
    nColors = 6

    particleSystem = ParticleSystem(rMax=rMax,dt=dt,friction=friction,Beta=Beta)
    particleSystem.generateRandomSystem(nParticles,nColors)

    for n in range(1000):
        screen.fill((0,0,0))

        particleSystem.loop()
        drawParticles(screen=screen,nParticles=nParticles,colors=particleSystem.colors,positions=particleSystem.positions,screenDim=screenDim)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()

        pygame.display.update()

    pygame.quit()
