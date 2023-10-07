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
#include <functional>
#include <chrono>
#include <iostream>
using namespace std::chrono;

#define NULL_AVOID 0.00000001

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
***/

using particles = std::pair<std::vector<double>,std::vector<double>>;

void timeit(std::function<void(void)> fn)
{
    auto start = high_resolution_clock::now();
    fn();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << "ms" << std::endl;
};

inline double lim(double val , double lim)
{
    double sign = val/fabs(val) * lim * (double)(fabs(val) > 0.5);
    return -sign + fmod(val,lim);
}

inline int mod(int val , int lim)
{
    int sign = val/fabs(val + NULL_AVOID) * lim * (val <= 0);
    return -sign + val % lim;
}

std::pair<int,int> getSwing(float rMax,float dHash)
{
    int swing = ceil((rMax)/(1.0/dHash));

    int swing_low = swing; 
    int swing_high = swing; 

    if((swing_low + 1 + swing_high) - dHash  > 0)
    {
        auto diff = (swing_low + 1 + swing_high) - dHash;
        swing_high -= ceil(diff/2);
        swing_low  -= floor(diff/2);
    }
    return std::make_pair(swing_low,swing_high);
}

template<typename T> 
std::vector<T> get_random_vector(unsigned int size,T lower, T upper)
{
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG

    // Define the distribution for random doubles between 0 and 1
    std::uniform_real_distribution<T> dis(lower, upper);
    
    // Initialize the vector with random doubles
    std::vector<T> randomVars(size);

    for (int i = 0; i < size; ++i) {
        randomVars[i] = lim(dis(gen),0.5); // Generate a random double and assign it to the vector element
    }
    return randomVars;
}

template<>
std::vector<int> get_random_vector<int>(unsigned int size, int lower, int upper) {
    // Seed for the random number generator
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG

    // Define the distribution for random doubles between 0 and 1
    std::uniform_int_distribution<int> dis(lower, upper);
    
    // Initialize the vector with random doubles
    std::vector<int> randomVars(size);

    for (int i = 0; i < size; ++i) {
        randomVars[i] = dis(gen); // Generate a random double and assign it to the vector element
    }
    return randomVars;
}

template<typename T> std::vector<T> get_zero_vector(unsigned int size)
{
    // Initialize the vector with random doubles
    std::vector<T> zeroVector(size);

    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        zeroVector[i] = static_cast<T>(0.0); // Generate a random double and assign it to the vector element
    }

    return zeroVector;
}

inline double F(double distance , double relation, double b)
{
    return relation * (1 - fabs(2.0 * distance - 1.0 - b)/(1 - b)) * (double)(b < distance and distance < 1.0) + \
     (distance/b - 1)* (double)(distance <= b and distance > NULL_AVOID);
}

template<typename T> 
void showVector(std::vector<T> vec){
    // Print elements of the 1D vector
    std::cout << "1D Vector Elements: ";
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << "" << vec[i] << ", ";
    }
    std::cout << std::endl;
};


template<typename T> 
void showMatrix(std::vector<std::vector<T>> matrix){
    // Print elements of the 2D vector
    std::cout << "2D Vector Elements:" << std::endl;
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            std::cout <<  matrix[i][j] << " ";
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
        double Beta,
        std::vector<std::vector<double>> flavourMatrix
        )
        {
            this->dt = dt;
            this->Beta = Beta; 
            this->rMax = rMax;
            this->force = force;
            this->friction = friction;

            this->parameter = (force * rMax) * friction;

            calcSpatialAttention();

            positions_X = get_random_vector<double>(size,-0.5,0.5);
            positions_Y = get_random_vector<double>(size,-0.5,0.5);

            shifted_X = get_zero_vector<double>(size);
            shifted_Y = get_zero_vector<double>(size);

            forces_X = get_zero_vector<double>(size);
            forces_Y = get_zero_vector<double>(size);

            velocities_X = get_zero_vector<double>(size);
            velocities_Y = get_zero_vector<double>(size);

            flavour = get_random_vector<int>(size,0,flavourMatrix.size()-1);

            this->spatialVector = std::vector<std::vector<int>>(size,std::vector<int>());
            this->spatialHash = std::vector<std::vector<std::vector<int>>>(
                dHash,
                std::vector<std::vector<int>>(
                    dHash,
                    std::vector<int>()
            ));

            auto[swing_low,swing_high] = getSwing(rMax, dHash);
            for(int n = 0 ; n < positions_X.size(); n++){
                shifted_X[n] = (positions_X[n] + 0.5);
                shifted_Y[n] = (positions_Y[n] + 0.5);
                encode(shifted_X[n],shifted_Y[n],n,swing_low,swing_high);
            };

            for(int i = 0 ; i < size ; i++){

                int n = int(shifted_X[i] * (dHash - 0.01));
                int m = int(shifted_Y[i] * (dHash - 0.01));
                std::vector<std::pair<int,int>> indecies;
                
                for(int mod_i = (n-swing_low) ; mod_i <= (n+swing_high) ; mod_i++)
                {
                    for(int mod_j = (m-swing_low) ; mod_j <= (m+swing_high) ; mod_j++)
                    {
                        indecies.push_back(std::make_pair(mod(mod_i,dHash),mod(mod_j,dHash)));
                    }
                }
                // make new spatial cors
                spatialCoors.push_back(std::make_pair(std::make_pair(n,m),indecies));
            }

            this->flavours = self_flauvoring(flavour,flavourMatrix);
        }

        std::vector<int> getFlavour()
        {
            return this->flavour;
        }

        particles step()
        {
            // better to have computation in bigger chunks for parallelism than divided per function
            auto[swing_low,swing_high] = getSwing(rMax, dHash);

            #pragma omp parallel for  
            for(int first = 0 ; first < spatialVector.size() ; first++)
            {
                double f1,f2,r,distX1,distX2,distY1,distY2;
                for(auto second : spatialVector[first])
                {
                    if(first != second)[[likely]]
                    {
                        distX1 = lim(positions_X[first] - positions_X[second],1.0);
                        distY1 = lim(positions_Y[first] - positions_Y[second],1.0);

                        r = sqrt(pow(distX1,2) + pow(distY1,2)) + 0.0000000000000001;
                        
                        f1 = -1.0*F(r/rMax,flavours[first][second],Beta);

                        // first != second - branchless check if frist is diff second, if it is equal then entire euqation is zeroed
                        forces_X[first] += f1 * (distX1)/r * force * rMax;
                        forces_Y[first] += f1 * (distY1)/r * force * rMax;
                    }
                    
                }
            }

            #pragma omp parallel for
            for(int n = 0 ; n < spatialHash.size() ; n++){
                for(int m = 0 ; m < spatialHash[n].size() ; m++){
                    spatialHash[n][m].erase(spatialHash[n][m].begin(),spatialHash[n][m].end());  
                }
            }

            #pragma omp parallel for
            for(int n = 0 ; n < forces_X.size(); n++)
            {
                forces_X[n] *= friction;
                forces_Y[n] *= friction; 

                velocities_X[n] *= friction;
                velocities_Y[n] *= friction;

                velocities_X[n] += forces_X[n] * dt;
                velocities_Y[n] += forces_Y[n] * dt;

                positions_X[n] += velocities_X[n] * dt;
                positions_Y[n] += velocities_Y[n] * dt;

                positions_X[n] = lim(positions_X[n],0.5);
                positions_Y[n] = lim(positions_Y[n],0.5);   

                shifted_X[n] = (positions_X[n] + 0.5);
                shifted_Y[n] = (positions_Y[n] + 0.5);

                encode(shifted_X[n],shifted_Y[n],n,swing_high,swing_low);
                encodeCoors(shifted_X[n],shifted_Y[n],n,swing_low,swing_high);
            }

            #pragma omp parallel for
            for(int n = 0 ; n < shifted_X.size(); n++)
            {
                getSpatialVector(shifted_X[n],shifted_Y[n],n,swing_low,swing_high);
            }

            return std::make_pair(shifted_X, shifted_Y);
        }

        #ifdef EXPERIMENTAL

        void set_rMax(double rMax){
            this->rMax = rMax;
            calcSpatialAttention();
        };

        void set_dt(double dt){
            this->dt = dt;
        };
        void set_force(double force){
            this->force = force;
        }; 
        void set_friction(double friction){
            this->friction = friction;
        };
        void set_Beta(double Beta){
            this->Beta = Beta;
        };
        void set_Matrix(std::vector<std::vector<double>> flavourMatrix){
            this->flavourMatrix = flavourMatrix;
        };

        bool add_particles(std::vector<double> positions_X,std::vector<double> positions_Y){
            if(positions_X.size() == positions_Y.size())
            {
                auto size = positions_X.size();
                this->positions_X.insert(this->positions_X.end(),positions_X.begin(),positions_X.end());
                this->positions_Y.insert(this->positions_Y.end(),positions_Y.begin(),positions_Y.end());

                shifted_X.resize(shifted_X.size() + size, 0.0);
                shifted_Y.resize(shifted_Y.size() + size, 0.0);

                forces_X.resize(forces_X.size() + size, 0.0);
                forces_Y.resize(forces_Y.size() + size, 0.0);

                velocities_X.resize(velocities_X.size() + size, 0.0);
                velocities_Y.resize(velocities_Y.size() + size, 0.0);

                flavour = get_random_vector<int>(this->positions_X.size(),0,flavourMatrix.size()-1);
                spatialVector = std::vector<std::vector<int>>(this->positions_X.size(),std::vector<int>());

                auto[swing_low,swing_high] = getSwing(rMax, dHash);
                for(int n = 0 ; n < this->positions_X.size(); n++){
                    shifted_X[n] = (this->positions_X[n] + 0.5);
                    shifted_Y[n] = (this->positions_Y[n] + 0.5);
                    encode(shifted_X[n],shifted_Y[n],n,swing_low,swing_high);
                };

                for(int i = 0 ; i < this->positions_X.size() ; i++){

                    int n = int(shifted_X[i] * (dHash - 0.01));
                    int m = int(shifted_Y[i] * (dHash - 0.01));
                    std::vector<std::pair<int,int>> indecies;
                    
                    for(int mod_i = (n-swing_low) ; mod_i <= (n+swing_high) ; mod_i++)
                    {
                        for(int mod_j = (m-swing_low) ; mod_j <= (m+swing_high) ; mod_j++)
                        {
                            indecies.push_back(std::make_pair(mod(mod_i,dHash),mod(mod_j,dHash)));
                        }
                    }
                    // make new spatial cors
                    spatialCoors.push_back(std::make_pair(std::make_pair(n,m),indecies));
                }

                this->flavours = self_flauvoring(flavour,flavourMatrix);
                return true;
            }
            return false
        };

        #endif
    private:

        // self correlation for particle flavours 
        std::vector<std::vector<double>> self_flauvoring(std::vector<int> flavour, std::vector<std::vector<double>> matrix) // that function is called only on init
        {
            auto flavours = std::vector<std::vector<double>>(flavour.size(),std::vector<double>(flavour.size(),0.0));
            for(int n = 0 ; n < flavour.size() ; n++)
            {
                for(int m = 0 ; m < flavour.size() ; m++)
                {
                    flavours[n][m] = matrix[flavour[n]][flavour[m]] * (not (m == n));
                }
            }
            return flavours;
        }

        void calcSpatialAttention() 
        {
            if(spatialAtention.size()) spatialAtention.erase(spatialAtention.begin(),spatialAtention.end());

            auto[swing_low,swing_high] = getSwing(rMax,dHash);
            for(int n = 0 ; n < dHash; n++){    
                spatialAtention.push_back(std::vector<std::vector<std::pair<int,int>>>());

                for(int m = 0 ; m < dHash; m++){     
                    std::vector<std::pair<int,int>> indecies;
                    for(int mod_i = (n-swing_low) ; mod_i <= (n+swing_high) ; mod_i++)
                    {
                        for(int mod_j = (m-swing_low) ; mod_j <= (m+swing_high) ; mod_j++)
                        {
                            indecies.push_back(std::make_pair(mod(mod_i,dHash),mod(mod_j,dHash)));
                        }
                    }
                    spatialAtention[n].push_back(indecies);         
                }
            }
        }


        void encode(double X, double Y, int index, int swing_low,int swing_high){    
            int n = int(X * (dHash - 0.01));
            int m = int(Y * (dHash - 0.01));

            #pragma omp critical
            spatialHash[n][m].push_back(index);
        }

        void getSpatialVector(double X, double Y, int index, int swing_low,int swing_high)
        {
            int n = int(X * (dHash - 0.01));
            int m = int(Y * (dHash - 0.01));

            std::vector<int> indecies;
            
            for(auto[i,j] : spatialCoors[index].second)
            {
                indecies.insert(indecies.end(),spatialHash[i][j].begin(),spatialHash[i][j].end());
            }

            spatialVector[index] = indecies;
        }


        void encodeCoors(double X, double Y, int index, int swing_low, int swing_high){
            int n = int(X * (dHash - 0.01));
            int m = int(Y * (dHash - 0.01));

            auto[xHash,yHash] = spatialCoors[index].first;
            
            if(n != xHash or m != yHash) [[unlikely]]
            {
                #pragma omp critical
                spatialCoors[index] = std::make_pair(std::make_pair(n,m),spatialAtention[n][m]);
            }     
        }

        std::vector<double> positions_X;
        std::vector<double> positions_Y;
        std::vector<double> forces_X;
        std::vector<double> forces_Y;
        std::vector<double> velocities_X;
        std::vector<double> velocities_Y;
        std::vector<double> shifted_X;
        std::vector<double> shifted_Y;

        std::vector<int> flavour;
        std::vector<std::vector<double>> flavours;

        std::vector<std::vector<std::vector<int>>> spatialHash;
        std::vector<std::vector<int>> spatialVector;
        std::vector<
            std::vector<
                std::vector<
                    std::pair<int,int>
                >
            >
        > spatialAtention;
        
        std::vector<
            std::pair<
                std::pair<int,int>,
                std::vector<std::pair<int,int>>
            >
        > spatialCoors;

        int dHash = 35;

        double dt;
        double rMax;
        double force;
        double Beta;
        double friction;
        double parameter;
};
