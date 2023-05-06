import numpy as np
import pygame
import json

colors = {
    0: (255,0,0),
    1: (0,255,0),
    2: (0,0,255),
    3: (255,255,0),
    4: (255,0,255),
    5: (0,255,255),
}

class RobotGenerator:

    def __init__(self,nColors,width,height) -> None:
        self.all_particles = []
        self.width         = width
        self.height        = height
        self.attraction = np.zeros((nColors,nColors))
        pass

    def add_particles(self,x,y,color):
        self.all_particles.append([x,y,color])

    def add_rules(self,c1,c2,strength):
        # if strength < 1.0 and strength > -1.0:
        self.attraction[c1,c2] = strength

    def save(self):
        data_to_save = {
            "width" : self.width,
            "height": self.height,
            "attraction" : self.attraction.tolist(),
            "robot" : self.all_particles
        }
        with open("particleRobot.json","w+") as fp:
            json.dump(data_to_save,fp)


#################################### Drawing ###################################################

def strength2Color(val):
    if val >= 0.0:
        return (0,255*val,0)
    if val < 0.0:
        return (255*abs(val),0,0)

class attractionMatrix:

    def __init__(self,attractionDim,pos_x,pos_y,colorColumnWidth,colorRowHeight,nColors) -> None:
        self.attractionDim = attractionDim
        self.pos_x = pos_x
        self.pos_y = pos_y
        self.colorColumnWidth = colorColumnWidth
        self.colorRowHeight = colorRowHeight
        self.sizeOfButton = ((attractionDim - colorColumnWidth)/nColors)

        self.circleRadius = self.sizeOfButton/2
        self.verticalColorColumn = [
            (colorColumnWidth + self.sizeOfButton/2 + m * self.sizeOfButton,attractionPosition_y + self.sizeOfButton) for m in range(nColors)
        ]

        self.horizontalColorColumn = [
            (self.sizeOfButton , attractionPosition_y + colorRowHeight + self.sizeOfButton/2 + n * self.sizeOfButton) for n in range(nColors)
        ]

        pass

    def draw(self,screen,colors,robot):

        for m,center in enumerate(self.verticalColorColumn):
            pygame.draw.circle(
                screen,
                colors[m],
                center = center,
                radius = self.sizeOfButton/2
            )

        for m,center in enumerate(self.horizontalColorColumn):
            pygame.draw.circle(
                screen,
                colors[m],
                center = center,
                radius = self.sizeOfButton/2
            )

        for n in range(nColors):
            for m in range(nColors):
                pygame.draw.rect(screen, strength2Color(robot.attraction[m][n]), pygame.Rect(
                    self.colorColumnWidth + self.pos_x + self.sizeOfButton * m,
                    self.colorRowHeight + self.pos_y + self.sizeOfButton * n,
                    self.sizeOfButton - 2 ,
                    self.sizeOfButton - 2
                ))# 2 pixel for nice separation

    def press(self,pos_x,pos_y,robot,factor=1.0):
        for n in range(nColors):
            for m in range(nColors):
                button_x = self.colorColumnWidth + self.pos_x + self.sizeOfButton * n
                button_y = self.colorRowHeight + self.pos_y + self.sizeOfButton * m
                button_width = self.sizeOfButton - 2
                button_height = self.sizeOfButton - 2

                if button_x < pos_x and pos_x < button_x + button_width and button_y < pos_y and pos_y < button_y + button_height:
                    strength = robot.attraction[n,m] + 0.1 * factor
                    robot.add_rules(n,m,strength)

    def pressColor(self,pos_x,pos_y,curr_color):
        for n in range(nColors):
            circle_1 = self.verticalColorColumn[n]
            circle_2 = self.verticalColorColumn[n]
            radius = self.sizeOfButton/2

            if circle_1[0]-radius < pos_x and pos_x < circle_1[0]+radius and circle_1[1]-radius < pos_y and pos_y < circle_1[1]+radius:
                return n

            if circle_2[0]-radius < pos_x and pos_x < circle_2[0]+radius and circle_2[1]-radius < pos_y and pos_y < circle_2[1]+radius:
                return n
        return curr_color

if __name__ == "__main__":
    screenDim = 600
    editorDim = 200
    attractionDim = 200

    attractionPosition_x = 0
    attractionPosition_y = screenDim/2-(attractionDim/2)
    colorColumnWidth = 40
    colorRowHeight   = 40

    editorPosition_x = screenDim/2-(editorDim/2)
    editorPosition_y = screenDim/2-(editorDim/2)

    curr_color = 0
    nColors = 6
    attractionArray = np.zeros((nColors,nColors))

    pygame.init()
    screen = pygame.display.set_mode((screenDim, screenDim))

    attractionMat = attractionMatrix(
        attractionDim,
        attractionPosition_x,
        attractionPosition_y,
        colorColumnWidth,
        colorRowHeight,
        nColors)

    robot = RobotGenerator(nColors,100,100)

    while True:
        screen.fill((30,30,30))
        pygame.draw.rect(screen, (0,0,0), pygame.Rect(editorPosition_x, editorPosition_y, editorDim, editorDim))

        attractionMat.draw(screen,colors,robot)

        for particle in robot.all_particles:
            pygame.draw.circle(screen, colors[particle[2]], center=(particle[0]*editorDim + editorPosition_x,particle[1]*editorDim + editorPosition_y), radius=5)

        for event in pygame.event.get():
            if event.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                pos_x = pos[0]
                pos_y = pos[1]

                pressed = pygame.mouse.get_pressed(num_buttons=5)
                factor = 1.0 - 2.0 * pressed[2] # detect left mouse click
                attractionMat.press(pos_x,pos_y,robot,factor)
                curr_color = attractionMat.pressColor(pos_x,pos_y,curr_color)

                # I love ugly oneliners
                if editorPosition_x < pos_x and pos_x < editorPosition_x + editorDim and editorPosition_y < pos_y and pos_y < editorPosition_y + editorDim:
                    robot.add_particles((pos_x - editorPosition_x)/(editorDim),(pos_y  - editorPosition_y)/(editorDim),curr_color)

            if event.type == pygame.QUIT:
                pygame.quit()

        robot.save()
        pygame.display.flip()

