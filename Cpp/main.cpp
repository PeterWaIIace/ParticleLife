#include "raylib.h"
#include "raymath.h"
#include <unistd.h>

#include <ParticleSystem.hpp>

int main(int argc, char* argv[])
{

    // ParticleSystem(unsigned int size, 
    // double dt, 
    // double rMax, 
    // double force, 
    // double friction,
    // double Beta)
    ParticleSystem system(10,0.01,0.5,50,0.1,0.2);
    // system.create_pool(poolSize);
    int width  = 1000;
    int height = 1000;

    InitWindow(width, height, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    std::cout << "ParticleUniverse generated" << std::endl;
    // auto [X, Y] = system.step();

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        // sleep(1);
        BeginDrawing();
        ClearBackground(BLACK);

        auto [X, Y] = system.step();
        
        for(int n = 0 ; n < X.size() ; n++)
        {
            float x = (X[n])*width;
            float y = (Y[n])*height;
            DrawCircleGradient(x,y,2,Fade(BLUE,1),Fade(BLUE,1));
            // DrawCircleGradient(x,y,10,Fade(allColors[particle.color],0.2),Fade(allColors[particle.color],0.0));
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
};