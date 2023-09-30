#include "raylib.h"
#include "raymath.h"

#include <ParticleSystem.hpp>

int main(int argc, char* argv[])
{
    int numberOfParticles = atoi(argv[1]);
    unsigned int bucketSize = atoi(argv[2]);
    unsigned int poolSize = atoi(argv[3]);

    std::cout << "starting: " << numberOfParticles << " bucket: " << bucketSize << std::endl;

    ParticleSystem system();
    // system.create_pool(poolSize);
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

        auto particles = system.step();

        std::vector<float> X = particles.first();
        std::vector<float> Y = particles.second();
        
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