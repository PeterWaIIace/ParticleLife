#include "raylib.h"
#include "raymath.h"
#include <unordered_map>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <thread>

#include "particle.hpp"
#include "SpatialHashing.hpp"

extern Color allColors[] = {
    BLUE       ,     // Blue
    GREEN      ,      // Green
    YELLOW     ,     // Yellow
    LIME       ,      // Lime
    DARKGREEN  ,      // Dark Green
    MAGENTA    ,     // Magenta
    RED        ,     // Red
    SKYBLUE    ,   // Sky Blue
    DARKBLUE   ,      // Dark Blue
    PURPLE     ,   // Purple
    VIOLET     ,    // Violet
    DARKPURPLE ,    // Dark Purple
    BEIGE      ,   // Beige
    BROWN      ,    // Brown
    DARKBROWN  ,      // Dark Brown
    WHITE      ,   // White
    BLANK      ,           // Blank (Transparent)
};

class ParticleUniverse
{
    public:
        ParticleUniverse(int nParticles, int width, int height) : rd_{}, gen_{rd_()}, dist_(1,100)
        {
            this->width = width;
            this->height = height;

            for(int i = 0 ; i < nParticles ; i++)
            {
                this->particles.push_back(Particle(
                    ((float)this->getRandomInt(-10000,10000))/10000.0, // X
                    ((float)this->getRandomInt(-10000,10000))/10000.0,  // Y
                    this->getRandomInt(0,MAX_COLORS-1)  // COLOR
                ));
            }
        };

        std::vector<Particle> step()
        {
            auto start = std::chrono::high_resolution_clock::now();
            int i = 0;
            for(auto &particle : this->particles)
            {
                particle.resetForce();
                spatialHash_.GetNearby(particle);
                particle.updateVelocity();
            }

            spatialHash_.Clear();
            for(auto &particle : this->particles)
            {
                particle.updatePostion(); // some random 0.1
                spatialHash_.Add(particle);
            }
            return particles;
        }

    private:

        int ParticleUniverse::getRandomInt(int min, int max)
        {
            dist_.param(std::uniform_int_distribution<>::param_type{min, max});
            return dist_(gen_);
        }

        SpatialHash spatialHash_{0.1};
        int quantization = 2;
        int height = 0;
        int width = 0;
        int cellWidth = 0;
        int cellHeight = 0;

        std::random_device rd_;
        std::mt19937 gen_;
        std::uniform_int_distribution<> dist_;

        std::vector<Particle> particles;

};

int main(void)
{
    std::cout << "starting" << std::endl;

    int width  = 1000;
    int height = 1000;

    InitWindow(width, height, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    ParticleUniverse ParticleUniverse(8000,width, height);

    std::cout << "ParticleUniverse generated" << std::endl;

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        std::vector<Particle> particles = ParticleUniverse.step();
        for(auto &particle : particles)
        {
            float x = (particle.getPosition().x+1.0)/2.0*width;
            float y = (particle.getPosition().y+1.0)/2.0*height;
            DrawCircleGradient(x,y,3,Fade(allColors[particle.color],1),Fade(allColors[particle.color],0.0));
            // DrawCircleGradient(x,y,10,Fade(allColors[particle.color],0.2),Fade(allColors[particle.color],0.0));
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
};