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
#include <iso646.h>

using namespace std::chrono;

/***
Matrix parameters:
positions_X
positions_Y
forces_X
forces_Y
velocities_X
velocities_Y

Parameters:

dt
rMax
force
Beta

*/

using particles = std::pair<std::vector<float>,std::vector<float>>

std::vector<float> get_random_vector(unsigned int size)
{
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG

    // Define the distribution for random floats between 0 and 1
    std::uniform_real_distribution<float> dis(-0.5, 0.5);

    // Initialize the vector with random floats
    std::vector<float> randomFloats(size);
    for (int i = 0; i < size; ++i) {
        randomFloats[i] = dis(gen); // Generate a random float and assign it to the vector element
    }

    return randomFloats;
}

std::vector<float> get_zero_vector(unsigned int size)
{
    // Initialize the vector with random floats
    std::vector<float> zeroVector(size);
    for (int i = 0; i < size; ++i) {
        zeroVector[i] = 0.0; // Generate a random float and assign it to the vector element
    }

    return zeroVector;
}

inline double F(float distance , float relation, float b)
{
    return relation * (1 - fabs(2.0 * distance - 1.0 - b)/(1 - b)) * (double)(b < distance and distance < 1.0) + (distance/b - 1)* (double)(distance < b and distance > 0.0);
}

inline double dist(double x1, double x2, double y1, double y2)
{
    return sqrt(pow(x1 - x2,2) + pow(y1 - y2,2))
}

inline double relations(double flavour1, double flavour2)
{
    return 1;
}

inline double lim(float val , float lim)
{
    float sign = val/fabs(val) * lim;
    return -sign + fmod(val,lim);
}


class ParticleSystem
{
    public:
        ParticleSystem(unsigned int size, 
        float dt, 
        float rMax, 
        float force, 
        float Beta)
        {
            positions_X = get_random_vector(size);
            positions_Y = get_random_vector(size);

            forces_X = get_zero_vector(size);
            forces_Y = get_zero_vector(size);

            velocities_X = get_zero_vector(size);
            velocities_Y = get_zero_vector(size);

            flavour = get_zero_vector(size);

            dt = dt;
            rMax = rMax;
            force = force;
            Beta = Beta;
        }

        std::vector<std::vector<double>> getDists(std::vector<double> positions){
            std::vector<std::vector<double>> distances;
            for(int n = 0 ; n < positions.size(); n++)
            {
                for(int m = 0 ; m < positions.size(); m++)
                {
                    distances[n][m] = positions[n] - position[m];
                }
            }
            return distances;
        }

        std::vector<std::vector<double>> getR(std::vector<std::vector<double>> distX , std::vector<std::vector<double>> distY)
        {
            std::vector<std::vector<double>> r;

            assert(distX.size() == distY.size());

            for(int n = 0 ; n < distX.size(); n++)
            {
                for(int m = 0 ; m < distX.size(); m++)
                {
                    r[n][m] = sqrt(pow(distX[n][m],2) + pow(distY[n][m],2));
                }
            }

            return r;
        }


        particles step()
        {
            auto distX = getDists(positionsX);
            auto distY = getDists(positionsY);

            float r = getR(distX,distY);

            for(int n = 0 ; n < distX.size(); n++)
            {
                for(int m = 0 ; m < distX.size(); m++)
                {
                    float f = F(r[n][m]/rMax,relations(flavour[n],flavour[m]),Beta);

                    forces_X[n] += f * (postions_X[n] - postions_X[m])/r * (f * rMax) * friction;
                    forces_Y[n] += f * (postions_X[n] - postions_X[m])/r * (f * rMax) * friction;
                }
            }


            for(int n = 0 ; n < distX.size(); n++)
            {
                velocities_X[n]= forces_X[n] * dt;
                velocities_Y[n]= forces_Y[n] * dt;

                positions_X[n] += velocities_X * dt;
                positions_Y[n] += velocities_Y * dt;

                positions_X[n] = lim(positions_X[n],0.5);
                positions_Y[n] = lim(positions_Y[n],0.5);   
            }

            return std::make_pair(positions_X, positions_Y);
        }

    private:
        std::vector<float> positions_X;
        std::vector<float> positions_Y;
        std::vector<float> forces_X;
        std::vector<float> forces_Y;
        std::vector<float> velocities_X;
        std::vector<float> velocities_Y;

        std::vector<float> flavour;
       
        float dt;
        float rMax;
        float force;
        float Beta;

};
