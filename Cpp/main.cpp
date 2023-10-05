#include "raylib.h"
#include "raymath.h"
#include <unistd.h>

#include <ParticleSystem.hpp>

Color cflavours[6] = {BLUE,RED,GREEN,PINK,YELLOW,PURPLE};
int main(int argc, char* argv[])
{
    std::vector<std::vector<double>> flavourMatrix = {
        {1.0},
    };

    // ParticleSystem(unsigned int size, 
    // double dt, 
    // double rMax, 
    // double force, 
    // double friction,
    // double Beta)
    double beta = 0.01;
    double range = 0.1;
    ParticleSystem system(1000,0.01,range,100,0.01,beta,flavourMatrix);
    // system.create_pool(poolSize);
    int width  = 1000;
    int height = 1000;

    InitWindow(width, height, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    std::cout << "ParticleUniverse generated" << std::endl;
    // auto [X, Y] = system.step();

    auto flavour = system.getFlavour();
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
            Vector2 center = {.x = x, .y = y};
            DrawCircleGradient(x,y,2,Fade(cflavours[flavour[n]],1),Fade(cflavours[flavour[n]],0.5));

            // #define DEBUG
            #ifdef DEBUG
            DrawCircleGradient(x,y,width*beta,Fade(BLUE,0),Fade(GREEN,0.2));
            DrawRing(center,width*range,width*range-1,0, 360, 0,Fade(RED,1));
            #endif
            // DrawCircleGradient(x,y,10,Fade(allColors[particle.color],0.2),Fade(allColors[particle.color],0.0));
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
};