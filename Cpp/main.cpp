#include "raylib.h"
#include "raymath.h"
#include <map>
#include <vector>
#include <random>
#include <iostream>

#define MAX_COLORS 24

Color allColors[] = {
    LIGHTGRAY  ,   // Light Gray
    GRAY       ,   // Gray
    DARKGRAY   ,      // Dark Gray
    YELLOW     ,     // Yellow
    GOLD       ,     // Gold
    ORANGE     ,     // Orange
    PINK       ,   // Pink
    RED        ,     // Red
    MAROON     ,     // Maroon
    GREEN      ,      // Green
    LIME       ,      // Lime
    DARKGREEN  ,      // Dark Green
    SKYBLUE    ,   // Sky Blue
    BLUE       ,     // Blue
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

class Particle
{
    public:
        Particle(int x, int y, int color, int x_limit, int y_limit)
        {
            this->radius = 1;
            this->color = color;
            this->position = { (float)x, (float)y };
            this->y_limit = y_limit;
            this->x_limit = x_limit;
        }

        void resetForces()
        {
            this->force = {0.0,0.0};
        }

        void addForce(Vector2 otherPostion, float relation)
        {
            Vector2 forceVector = Vector2Subtract(this->position,otherPostion);
            float distance = Vector2Distance(this->position,otherPostion);
            forceVector = Vector2Scale(forceVector,1/(distance));
            // std::cout << "forceVector: " << forceVector.x << " " << forceVector.y << std::endl;
            this->force = Vector2Add(this->force,Vector2Scale(forceVector,relation));
        }

        void updatePostion()
        {
            this->position = Vector2Subtract(this->position,this->force);

            if(this->position.x > x_limit)
            {
                this->position.x = x_limit;
            }

            if(this->position.x < 0)
            {
                this->position.x = 1;
            }

            if(this->position.y > y_limit)
            {
                this->position.y = y_limit;
            }

            if(this->position.y < 0)
            {
                this->position.y = 1;
            }
        }

        Vector2 getPostion()
        {
            std::cout << "this->position: " << this->position.x << " " << this->position.y << std::endl;
            return this->position;
        }

        bool operator==(const Particle& other) const {
            return this == &other;
        }

        bool operator!=(const Particle& other) const {
           return !(*this == other);
        }

        int color = 0;
    private:
        int radius = 1;
        int y_limit = 1000;
        int x_limit = 1000;
        Vector2 position;
        Vector2 force;

};

class relationMatrix
{
    private:
        int arr[MAX_COLORS][MAX_COLORS];

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
            for(int i = 0 ; i < MAX_COLORS ; i++)
            {
                for(int j = 0 ; j < MAX_COLORS ; j++)
                {
                    arr[i][j] = this->getRandomInt(-10,10);
                }
            }
        }

        int getRelation(int firstColor,int secondColor)
        {
            return arr[firstColor][secondColor];
        }


};

class ParticleUniverse
{
    public:
        ParticleUniverse(int nParticles) : rd_{}, gen_{rd_()}, dist_(1,100)
        {
            for(int i = 0 ; i < nParticles ; i++)
            {
                this->particles.push_back(Particle(
                    this->getRandomInt(100,1000), // X
                    this->getRandomInt(100,1000),  // Y
                    this->getRandomInt(0,MAX_COLORS),  // COLOR
                    1000,
                    1000
                ));
            }
        }

        std::vector<Particle> step()
        {
            for(auto &particle1 : this->particles)
            {
                particle1.resetForces();
                for(auto &particle2 : this->particles)
                {
                    if(particle1 != particle2)
                    {
                        particle1.addForce(particle2.getPostion(),relations.getRelation(particle1.color,particle2.color));
                    }
                }
            }

            for(auto &particle : this->particles)
            {
                particle.updatePostion();
            }
            return particles;
        }

    private:

        int ParticleUniverse::getRandomInt(int min, int max)
        {
            dist_.param(std::uniform_int_distribution<>::param_type{min, max});
            return dist_(gen_);
        }

        std::random_device rd_;
        std::mt19937 gen_;
        std::uniform_int_distribution<> dist_;

        std::vector<Particle> particles;

        relationMatrix relations;
};

int main(void)
{
    std::cout << "starting" << std::endl;

    InitWindow(1200, 1200, "raylib [core] example - basic window");

    SetTargetFPS(30);               // Set our game to run at 60 frames-per-second

    ParticleUniverse ParticleUniverse(2);

    std::cout << "ParticleUniverse generated" << std::endl;

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        std::vector<Particle> particles = ParticleUniverse.step();
        for(auto &particle : particles)
        {
            DrawCircleV(particle.getPostion(),10,allColors[particle.color]);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
};