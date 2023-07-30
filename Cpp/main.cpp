#include "raylib.h"
#include "raymath.h"

#include <ParticleSystem.hpp>

int main(void)
{
    std::cout << "starting" << std::endl;

    ParticleSystem system;
    system.init(10000);

    int width  = 1000;
    int height = 1000;

    InitWindow(width, height, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    std::cout << "ParticleUniverse generated" << std::endl;

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        system.step();
        for(auto particle : system.getParticles())
        {
            float x = (particle.x)*width;
            float y = (particle.y)*height;
            DrawCircleGradient(x,y,3,Fade(BLUE,1),Fade(BLUE,0.0));
            // DrawCircleGradient(x,y,10,Fade(allColors[particle.color],0.2),Fade(allColors[particle.color],0.0));
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
};