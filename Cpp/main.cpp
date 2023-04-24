#include "raylib.h"
#include "raymath.h"
#include <map>
#include <vector>
#include <random>
#include <iostream>

#define MAX_COLORS 2

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

class Particle
{
    public:
        Particle(int x, int y, int color)
        {
            this->rMax = 0.5;
            this->color = color;
            this->position = { (float)x, (float)y };
            this->y_limit = y_limit;
            this->x_limit = x_limit;
            this->velocity = {0.0,0.0};
        }

        void resetForce()
        {
            this->totalForce = {0.0,0.0};
        }

        void addForce(Vector2 otherPostion, float relation)
        {
            Vector2 diffV  = Vector2Subtract(this->position,otherPostion);
            float r = Vector2Distance(this->position,otherPostion);

            if(0 < r && r < this->rMax)
            {
                float f = F(r/this->rMax,relation);
                this->totalForce.y = (position.y - otherPostion.y)/r * f;
                this->totalForce.x = (position.x - otherPostion.x)/r * f;
            }
        }

        void updateVelocity()
        {
            this->totalForce.y *= this->rMax;
            this->totalForce.x *= this->rMax;

            this->velocity.x *= this->friction;
            this->velocity.y *= this->friction;

            this->velocity.x += this->totalForce.y * this->dt;
            this->velocity.y += this->totalForce.x * this->dt;
        }

        double F(float distance,float a)
        {
            float beta = 0.3;
            if(distance < beta)
            {
                return distance/(beta-1);
            }
            else if(distance > beta)
            {
                return a * (1 - abs(2 * distance - 1 - beta)/(1 - beta));
            }
            else
            {
                return 0.0;
            }
        }

        void updatePostion(float dt)
        {
            this->position.x += this->velocity.x * dt;
            this->position.y += this->velocity.y * dt;
            // std::cout << this->position.x << " " << this->position.y << std::endl;

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
        Vector2 totalForce = {0.0,0.0};
        float dt = 0.1;
        float rMax   = 10.0;
        float radius = 1;
        float y_limit = 1.0;
        float x_limit = 1.0;
        float friction = 0.1;
        Vector2 position;
        Vector2 velocity;

};

class relationMatrix
{
    private:
        float arr[MAX_COLORS][MAX_COLORS] = {{1.0,1.0},{1.0,1.0}};

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
                    this->getRandomInt(this->width/4,this->width-this->width/4), // X
                    this->getRandomInt(this->height/4,this->height-this->height/4),  // Y
                    this->getRandomInt(0,MAX_COLORS-1)  // COLOR
                ));
            }

            initSpatialMap();
        };

        std::vector<Particle> step()
        {
            for(auto &particle1 : this->particles)
            {
                for(auto &particle2 : this->particles)
                {
                    if(particle1 != particle2)
                    {
                        particle1.addForce(particle2.getPostion(),relations.getRelation(particle1.color,particle2.color));
                    }
                }
                particle1.updateVelocity();
            }

            for(auto &particle : this->particles)
            {
                particle.updatePostion(0.1); // some random 0.1
            }
            return particles;
        }

    private:

        int ParticleUniverse::getRandomInt(int min, int max)
        {
            dist_.param(std::uniform_int_distribution<>::param_type{min, max});
            return dist_(gen_);
        }

        void initSpatialMap()
        {
            this->cellWidth  = this->width/this->quantization;
            this->cellHeight = this->height/this->quantization;

            for(auto &particle1 : this->particles)
            {
                Vector2 keys = getSpatialPosition(particle1.getPostion());
                spatialMap[(int)(keys.x)][(int)(keys.y)].push_back(particle1);
            }
        }

        Vector2 getSpatialPosition(Vector2 position)
        {
            int xKey = (int)(std::ceil((double)position.x/this->cellWidth));
            int yKey = (int)(std::ceil((double)position.y/this->cellHeight));
            Vector2 hashKeys = {xKey,yKey};
            return hashKeys;
        }

        std::map<int,std::map<int,std::vector<Particle>>> spatialMap;
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

    ParticleUniverse ParticleUniverse(100,width, height);

    std::cout << "ParticleUniverse generated" << std::endl;

    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(BLACK);

        std::vector<Particle> particles = ParticleUniverse.step();
        for(auto &particle : particles)
        {
            std::cout << particle.getPostion().x << " " << particle.getPostion().y << std::endl;
            float x = particle.getPostion().x*width;
            float y = particle.getPostion().y*height;
            DrawCircleGradient(x,y,10,Fade(allColors[particle.color],1),Fade(allColors[particle.color],0.0));
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
};