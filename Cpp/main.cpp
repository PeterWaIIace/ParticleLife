#include "raylib.h"
#include "raymath.h"
#include <unordered_map>
#include <vector>
#include <random>
#include <iostream>
#include <chrono>

#include "particle.hpp"
#include "SpatialHashing.hpp"

#define MAX_COLORS 8

Color allColors[] = {
    BLUE       ,     // Blue
    GREEN      ,      // Green
    YELLOW     ,     // Yellow
    GOLD       ,     // Gold
    ORANGE     ,     // Orange
    PINK       ,   // Pink
    RED        ,     // Red
    LIME       ,      // Lime
    DARKGREEN  ,      // Dark Green
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
    MAGENTA    ,     // Magenta
};

class relationMatrix
{
    private:
        float arr[MAX_COLORS][MAX_COLORS];
        // float arr[MAX_COLORS][MAX_COLORS] = {{1.0,0.5,0.0},{0.0,1.0,0.5},{-0.1,0.0,1.0}};
        // float arr[MAX_COLORS][MAX_COLORS] = {{1.0}}; // one for TESTING
        std::random_device rd_;
        std::mt19937 gen_;
        std::uniform_int_distribution<> dist_;

        int relationMatrix::getRandomInt(int min, int max)
        {
            dist_.param(std::uniform_int_distribution<>::param_type{min, max});
            return dist_(gen_);
        }
    public:
        relationMatrix() : rd_{}, gen_{rd_()}, dist_(1,100)
        {
            for(int i = 0 ; i <MAX_COLORS ; i++)
            {
                for(int j = 0 ; j <MAX_COLORS ; j++)
                {
                    arr[i][j] = (float)getRandomInt(-100,100)/100.0;
                }
            }
        }

        float getRelation(int firstColor,int secondColor)
        {
            return arr[firstColor][secondColor];
        }
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
                    ((float)this->getRandomInt(0,10000))/10000.0, // X
                    ((float)this->getRandomInt(0,10000))/10000.0,  // Y
                    this->getRandomInt(0,MAX_COLORS-1)  // COLOR
                ));
            }
        };

        std::vector<Particle> step()
        {
            for(auto &particle1 : this->particles)
            {
                particle1.resetForce();

                std::vector<Particle> surroundingParticles = spatialHash_.GetNearby(particle1);

                for(auto &particle2 : surroundingParticles)
                {
                    if(particle1 != particle2)
                    {
                        particle1.addForce(particle2.getPosition(),relations.getRelation(particle1.color,particle2.color));
                    }
                }
                particle1.updateVelocity();
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

        relationMatrix relations;
};

int main(void)
{
    std::cout << "starting" << std::endl;

    int width  = 1000;
    int height = 1000;

    InitWindow(width, height, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    ParticleUniverse ParticleUniverse(10000,width, height);

    std::cout << "ParticleUniverse generated" << std::endl;

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        std::vector<Particle> particles = ParticleUniverse.step();
        for(auto &particle : particles)
        {
            float x = particle.getPosition().x*width;
            float y = particle.getPosition().y*height;
            DrawCircleGradient(x,y,2,Fade(allColors[particle.color],1),Fade(allColors[particle.color],0.0));
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
};