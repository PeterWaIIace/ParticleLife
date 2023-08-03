#include "raylib.h"
#include "raymath.h"

#include <ParticleSystem.hpp>

int main(void)
{
    std::cout << "starting" << std::endl;

    ParticleSystem system;
    system.init(1000);

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

        system.step(
            // Step1 Let particle interact
            [](Particle& main, Particle& other){
                float relation = 1.0;
                float r = sqrt(pow(other.x - main.x,2) + pow(other.y - main.y,2));

                if(0 < r && r < main.rMax)
                {
                    float f = F(r/main.rMax,relation,0.3);
                    main.f_x += ((other.x - main.x)/r) * f;
                    main.f_y += ((other.y - main.y)/r) * f;
                }
            },
            // Step2 Update its velocity
            [](Particle& main){
                main.f_x *= main.rMax * main.force;
                main.f_y *= main.rMax * main.force;

                main.v_x *= main.friction;
                main.v_y *= main.friction;

                main.v_x += main.f_x * main.dt;
                main.v_y += main.f_y * main.dt;
            }
        );
        for(auto particle : system.getParticles())
        {
            float x = (particle.x)*width;
            float y = (particle.y)*height;
            DrawCircleGradient(x,y,3,Fade(BLUE,1),Fade(BLUE,1));
            // DrawCircleGradient(x,y,10,Fade(allColors[particle.color],0.2),Fade(allColors[particle.color],0.0));
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
};