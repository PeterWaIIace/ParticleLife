import pygame

# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

# Set the dimensions of the screen
SCREEN_WIDTH = 600
SCREEN_HEIGHT = 600

# Define the size of the maze
MAZE_WIDTH = 20
MAZE_HEIGHT = 20

# Initialize Pygame
pygame.init()

# Set up the screen
screen = pygame.display.set_mode([SCREEN_WIDTH, SCREEN_HEIGHT])
pygame.display.set_caption("Pygame Maze")

# Create a 2D list to represent the maze
maze = []
for i in range(MAZE_HEIGHT):
    row = []
    for j in range(MAZE_WIDTH):
        row.append(1)
    maze.append(row)

# Set some of the squares in the maze to be walls
maze[2][2] = 0
maze[2][3] = 0
maze[2][4] = 0
maze[3][4] = 0
maze[4][4] = 0
maze[4][3] = 0
maze[4][2] = 0
maze[3][1] = 0

# Set up the ball
ball_x = 50
ball_y = 50
ball_radius = 10

# Set up the clock
clock = pygame.time.Clock()

# Main game loop
done = False
while not done:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True

    # Move the ball
    keys = pygame.key.get_pressed()
    if keys[pygame.K_LEFT]:
        if maze[int(ball_y/30)][int((ball_x-5)/30)] == 1:
            ball_x -= 5
    if keys[pygame.K_RIGHT]:
        if maze[int(ball_y/30)][int((ball_x+5)/30)] == 1:
            ball_x += 5
    if keys[pygame.K_UP]:
        if maze[int((ball_y-5)/30)][int(ball_x/30)] == 1:
            ball_y -= 5
    if keys[pygame.K_DOWN]:
        if maze[int((ball_y+5)/30)][int(ball_x/30)] == 1:
            ball_y += 5

    # Clear the screen
    screen.fill(BLACK)

    # Draw the maze
    for i in range(MAZE_HEIGHT):
        for j in range(MAZE_WIDTH):
            if maze[i][j] == 0:
                pygame.draw.rect(screen, WHITE, [j*30, i*30, 30, 30])
            else:
                pygame.draw.rect(screen, BLACK, [j*30, i*30, 30, 30])

    # Draw the ball
    pygame.draw.circle(screen, WHITE, [int(ball_x), int(ball_y)], ball_radius)

    # Update the screen
    pygame.display.flip()

    # Wait for the next frame
    clock.tick(60)

# Clean up
pygame.quit()
