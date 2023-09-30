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
#include <cassert>
#include <iso646.h>
#include <omp.h>


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

using particles = std::pair<std::vector<double>,std::vector<double>>;

std::vector<double> get_random_vector(unsigned int size)
{
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG

    // Define the distribution for random doubles between 0 and 1
    std::uniform_real_distribution<double> dis(-0.5, 0.5);

    // Initialize the vector with random doubles
    std::vector<double> randomdoubles(size);

    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        randomdoubles[i] = dis(gen); // Generate a random double and assign it to the vector element
    }

    return randomdoubles;
}

std::vector<double> get_zero_vector(unsigned int size)
{
    // Initialize the vector with random doubles
    std::vector<double> zeroVector(size);

    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        zeroVector[i] = 0.0; // Generate a random double and assign it to the vector element
    }

    return zeroVector;
}

inline double F(double distance , double relation, double b)
{
    return relation * (1 - fabs(2.0 * distance - 1.0 - b)/(1 - b)) * (double)(b < distance and distance < 1.0) + (-1)*(distance/b - 1)* (double)(distance <= b and distance > 0.0000000001);
}

inline double relations(double flavour1, double flavour2)
{
    return -1;
}

inline double lim(double val , double lim)
{
    double sign = val/fabs(val) * lim * (double)(fabs(val) > 0.5);
    return -sign + fmod(val,lim);
}

void showVector(std::vector<double> vec){
    // Print elements of the 1D vector
    std::cout << "1D Vector Elements: ";
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
};


void showMatrix(std::vector<std::vector<double>> matrix){
    // Print elements of the 2D vector
    std::cout << "2D Vector Elements:" << std::endl;
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
};

class ParticleSystem
{
    public:
        ParticleSystem(unsigned int size, 
        double dt, 
        double rMax, 
        double force, 
        double friction,
        double Beta)
        {
            positions_X = get_random_vector(size);
            positions_Y = get_random_vector(size);

            showVector(positions_X);
            showVector(positions_Y);

            forces_X = get_zero_vector(size);
            forces_Y = get_zero_vector(size);

            velocities_X = get_zero_vector(size);
            velocities_Y = get_zero_vector(size);

            flavour = get_zero_vector(size);

            this->dt = dt;
            this->Beta = Beta; 
            this->rMax = rMax;
            this->force = force;
            this->friction = friction;

            this->parameter = (force * rMax) * friction;
        }

        particles step()
        {

            // better to have computation in bigger chunks for parallelism than divided per function
            // #pragma omp parallel for
            for(int n = 0 ; n < positions_X.size(); n++)
            {
                for(int m = 0 ; m < positions_Y.size(); m++)
                {
                    double r = sqrt(pow(positions_X[n] - positions_X[m],2) + pow(positions_Y[n] - positions_Y[m],2)) + 0.0000000000000001;

                    double f = F(r/rMax,relations(flavour[n],flavour[m]),Beta);
                    forces_X[n] += f * (positions_X[n] - positions_X[m])/r * force * rMax;
                    forces_Y[n] += f * (positions_Y[n] - positions_Y[m])/r * force * rMax;

                }
            }

            // #pragma omp parallel for
            for(int n = 0 ; n < forces_X.size(); n++)
            {
                forces_X[n] *=  friction;
                forces_Y[n] *=  friction; 

                velocities_X[n]= forces_X[n] * dt;
                velocities_Y[n]= forces_Y[n] * dt;

                positions_X[n] += velocities_X[n] * dt;
                positions_Y[n] += velocities_Y[n] * dt;

                positions_X[n] = lim(positions_X[n],0.5);
                positions_Y[n] = lim(positions_Y[n],0.5);   
            }

            std::vector<double> shifted_X(positions_X.size(), 0.0);
            std::vector<double> shifted_Y(positions_Y.size(), 0.0);

            #pragma omp parallel for
            for(int n = 0 ; n < shifted_X.size(); n++)
            {
                shifted_X[n] = (positions_X[n] + 0.5);
                shifted_Y[n] = (positions_Y[n] + 0.5);
            }

            return std::make_pair(shifted_X, shifted_Y);
        }

    private:
        std::vector<double> positions_X;
        std::vector<double> positions_Y;
        std::vector<double> forces_X;
        std::vector<double> forces_Y;
        std::vector<double> velocities_X;
        std::vector<double> velocities_Y;

        std::vector<double> flavour;
       
        double dt;
        double rMax;
        double force;
        double Beta;
        double friction;
        double parameter;
};
