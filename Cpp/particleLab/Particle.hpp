#pragma once

#include <vector>
#include <array>
#include <utility>
#include <functional>
#include <chrono>
#include <iostream>
#include <random>
#include <math.h>
#include <mutex>
#include <queue>


double F(float distance,float a, float b)
{
    if(distance < b)
    {
        return distance/b - 1;
    }
    else if(b < distance && distance < 1.0)
    {
        return a * (1 - fabs(2.0 * distance - 1.0 - b)/(1 - b));
    }
    else
    {
        return 0.0;
    }
}


class Particle{

    public:
        double x, y;
        double v_x = 0.0, v_y = 0.0;
        double f_x = 0.0 ,f_y = 0.0;
        double dt = 0.01;
        double friction = 0.01;
        double rMax = 0.1;
        double force = 10.0;
        double beta = 0.3;

        Particle()
        {
            this->x = 0.0;
            this->y = 0.0;
        }

        Particle(double x, double y)
        {
            this->x = x;
            this->y = y;
#ifdef DEBUG 
            std::cout <<"[" << this << "] this->x: " << this->x << " this->y: " << this->y << std::endl;
#endif
        }

        // void interact(Particle& other,std::function<void(Particle&,Particle&)> interact__){
        //     interact__(*this,other);
        // };

        void interact(Particle& other,void (interact__)(Particle&,Particle&)){
            interact__(*this,other);
        };

        void interact(Particle& other){
            float relation = 1.0;
            float r = sqrt(pow(other.x - x,2) + pow(other.y - y,2));

            if(0 < r && r < rMax)
            {
                float f = F(r/rMax,relation,0.3);
                f_x += ((other.x - x)/r) * f;
                f_y += ((other.y - y)/r) * f;

                other.f_x += ((x - other.x)/r) * f;
                other.f_y += ((y - other.y)/r) * f;
            }
        };

        // void Particle::updateVelocity(std::function<void(Particle&)> updateVelocity__)
        // {
        //     updateVelocity__(*this);
        // }

        void updateVelocity(void (updateVelocity__)(Particle&))
        {
            updateVelocity__(*this);
        }

        void updateVelocity()
        {
            f_x *= rMax * force;
            f_y *= rMax * force;

            v_x *= friction;
            v_y *= friction;

            v_x += f_x * dt;
            v_y += f_y * dt;
        }


        void updatePostion()
        {
#ifdef DEBUG
            std::cout <<"[" << this << "] Before this->x: " << this->x << " this->y: " << this->y << std::endl;
#endif
            x += v_x * dt;
            y += v_y * dt;

            if(x < 0.0)
            {
                x = 0.001;
            }

            if(y < 0.0)
            {
                y = 0.001;
            }

            if(x > 1.0)
            {
                x = 1.0 - 0.001;
            }

            if(y > 1.0)
            {
                y = 1.0 - 0.001;
            }
#ifdef DEBUG
            std::cout <<"[" << this << "] After this->x: " << this->x << " this->y: " << this->y << std::endl;
#endif
        }

        bool inRange(Particle& other)
        {
            float r = sqrt(pow(other.x - x,2) + pow(other.y - y,2));
            if(0 < r && r < rMax){
                return true;
            }
            else
            {
                return false;
            }
        }

        // how to name it nicely?
        std::vector<std::pair<double,double>> getMaxCoordinates()
        {
            std::vector<std::pair<double,double>> coordinates;

            coordinates.push_back(std::make_pair(x+rMax,y)); // right
            coordinates.push_back(std::make_pair(x,y+rMax)); // up
            coordinates.push_back(std::make_pair(x-rMax,y)); // left
            coordinates.push_back(std::make_pair(x,y-rMax)); // bottom

            coordinates.push_back(std::make_pair(x+rMax,y+rMax)); // right - up
            coordinates.push_back(std::make_pair(x-rMax,y+rMax)); // left - up
            coordinates.push_back(std::make_pair(x-rMax,y-rMax)); // left - bottom
            coordinates.push_back(std::make_pair(x+rMax,y-rMax)); // right - bottom 

            return coordinates;
        }
};