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

#include "ThreadPool.hpp"

using namespace std::chrono;

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


void timeit(std::function<void(void)> fn)
{
    auto start = high_resolution_clock::now();
    fn();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << "ms" << std::endl;
};


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

        void Particle::updateVelocity(void (updateVelocity__)(Particle&))
        {
            updateVelocity__(*this);
        }

        void Particle::updateVelocity()
        {
            f_x *= rMax * force;
            f_y *= rMax * force;

            v_x *= friction;
            v_y *= friction;

            v_x += f_x * dt;
            v_y += f_y * dt;
        }


        void Particle::updatePostion()
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

};


class Bucket
{
    private:
        std::vector<Particle> frame;
        std::shared_ptr<std::vector<Particle>> framePtr = std::make_shared<std::vector<Particle>>(frame);
    public:
        int row = 0;
        int col = 0;

        Bucket(int row, int col): row(row), col(col) {};

        void insert(Particle& particle)
        {
            framePtr ->push_back(particle);
        };
        std::shared_ptr<std::vector<Particle>> pop(){
            return framePtr ;
        };

        int size(){
            return framePtr->size();
        };
};
class Buckets
{
    public:
        double nHeight = 1.0;
        double nWidth = 1.0;
        double stepHeight = 1.0;
        double stepWidth = 1.0;
        int nBucketsHeight = 1;
        int nBucketsWidth = 1;
        int interatorIndex = 0;

        std::vector<std::vector<Bucket>> b__;
    private:

        Bucket& chooseBucket(double x, double y)
        {
            if(x < 0.0) x = 0.0;
            if(y < 0.0) y = 0.0;
            if(x > 1.0) x = 1.0;
            if(y > 1.0) y = 1.0;

            int row = int(x/stepHeight);
            int col = int(y/stepWidth);

            for(int i = 0 ; i < b__.size() ; i++){
                for(int j = 0 ; j < b__[i].size() ; j++){
                }
            }

            return b__[row][col];
        }

        struct Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = Bucket;
            using pointer           = Bucket*;  // or also value_type*
            using reference         = Bucket&;  // or also value_type&

            Iterator(pointer ptr,Buckets* baseThis) : m_ptr(ptr), m_baseThis(baseThis) {}

            std::pair<pointer,std::vector<Particle>> operator*() const { return std::make_pair(m_ptr,m_baseThis->getSurroundingBuckets()); }
            std::pair<pointer,std::vector<Particle>> operator->(){ return std::make_pair(m_ptr,m_baseThis->getSurroundingBuckets()); }

            // Prefix increment
            Iterator& operator++() {
                m_baseThis->interatorIndex++;
                int row = m_baseThis->interatorIndex/m_baseThis->nBucketsWidth;
                int col = m_baseThis->interatorIndex%m_baseThis->nBucketsWidth;
                m_ptr = &(m_baseThis->b__[row][col]);

                if(m_baseThis->interatorIndex >= m_baseThis->nBucketsWidth*m_baseThis->nBucketsHeight)
                {
                    m_baseThis->interatorIndex = 0;
                }
                return *this;
            }

            // Postfix increment
            Iterator operator++(int) {
                int row = m_baseThis->interatorIndex/m_baseThis->nBucketsWidth;
                int col = m_baseThis->interatorIndex%m_baseThis->nBucketsWidth;
                m_ptr = &(m_baseThis->b__[row][col]);
                m_baseThis->interatorIndex++;
                if(m_baseThis->interatorIndex >= m_baseThis->nBucketsWidth*m_baseThis->nBucketsHeight)
                {
                    m_baseThis->interatorIndex = 0;
                }
                return *this;
            }

            friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

        private:

            pointer m_ptr;
            Buckets* m_baseThis;

        };


    public:
        Buckets(Buckets& other)
        {
            this->b__ = b__;
            this->nWidth = nWidth;
            this->nHeight = nHeight;

        }

        Buckets(int nHeight = 1,int nWidth = 1)
        {
            stepHeight = 1.0/(double)(nHeight);
            stepWidth  = 1.0/(double)(nWidth);
            nBucketsHeight = nHeight;
            nBucketsWidth  = nWidth;

            for(int n = 0 ; n < nBucketsHeight ; n++)
            {
                b__.push_back(std::vector<Bucket>());

                for(int i = 0 ; i < nBucketsWidth ; i++)
                {
                    b__[n].push_back(Bucket(b__[n].size(),b__.size()));
                }
            }

            // adding end empty element
            b__.push_back(std::vector<Bucket>());
            b__[b__.size()-1].push_back(Bucket(b__[b__.size()-1].size(),b__.size()));
        }

        std::vector<int> getBucketsSize(){
            std::vector<int> sizes;
            for(int row = 0; row < nBucketsHeight ; row++){
                for(int col = 0; col < nBucketsWidth ; col++){
                    auto s = b__[row][col].size();
                    sizes.push_back(s);
                }
            }
            return sizes;
        };

        Iterator begin() { return Iterator(&b__[0][0],this); }
        Iterator end()   { return Iterator(&b__[b__.size()-1][b__[b__.size()-1].size()-1],this); } // 200 is out of bounds

        void insert(Particle& particle)
        {
            auto& bucket = chooseBucket(particle.x,particle.y);
            bucket.insert(particle);
        }


        std::shared_ptr<std::vector<Particle>> getBucket(int i , int j)
        {
            return b__[i][j].pop();
        }

        std::vector<Particle> getSurroundingBuckets()
        {
            int row = interatorIndex/nBucketsWidth;
            int col = interatorIndex%nBucketsWidth;
            return getSurroundingBuckets(row,col);
        }

        std::vector<Particle> getSurroundingBuckets(int i , int j)
        {
            std::vector<Particle> neighbors;
            for(int n = -1 ; n <= 1 ; n++)
            {
                int row = i + n;
                for(int m = -1 ; m <= 1 ; m++)
                {
                    int col = j + m;

                    if((row >= 0 && col >= 0) && ( row != i || col != j )  && (row < nBucketsHeight && col < nBucketsWidth) )
                    {
                        neighbors.insert(neighbors.end(),b__[row][col].pop()->begin(),b__[row][col].pop()->end());
                    }
                }
            }
            return neighbors;
        }

        std::vector<Particle> getAllBuckets()
        {
            std::vector<Particle> neighbors;
            for(int col = 0 ; col < nBucketsWidth ; col++)
            {
                for(int row = 0 ; row < nBucketsHeight ; row++)
                {
                    neighbors.insert(neighbors.end(),b__[row][col].pop()->begin(),b__[row][col].pop()->end());
                }
            }
            return neighbors;
        }
};


Particle task(std::pair<Particle,std::vector<Particle>>& workload){
    for(auto other : workload.second)
    {
        workload.first.interact(other);
    }
    // this may mess up calculations so it may be better in other thread
    workload.first.updateVelocity();
    workload.first.updatePostion();

    return workload.first;
}

class ParticleSystem
{
    private:
        // std::vector<Particle> frame;
        // std::vector<Particle> nextFrame;

        int bucketSize = 1;
        std::mutex g_bucket_mutex;

        Buckets buckets;
        Buckets nextBuckets;


        Pool<std::pair<Particle,std::vector<Particle>>,Particle> workers;

    public:
        ParticleSystem(){};
        void init(int initSize = 10000, unsigned int bucketSize = 1)
        {
            double lower_bound = 0.0;
            double upper_bound = 1.0;

            bucketSize = bucketSize;
            std::cout << "bucketSize: " << bucketSize << std::endl;
            buckets = Buckets(bucketSize,bucketSize);
            nextBuckets = Buckets(bucketSize,bucketSize);

            std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
            std::default_random_engine re;

            for(int i = 0 ; i < initSize ; i++)
            {
                buckets.insert(Particle(unif(re),unif(re)));
            }
        }

        // ParticleSystem(ParticleSystem& other){
        //     bucketSize = other.bucketSize;
        //     buckets    = other.buckets;
        //     nextBuckets= other.nextBuckets;
        //     workers    = other.workers;
        // };

        void step(void (interact__)(Particle&,Particle&),void (updateVelocity__)(Particle&))
        {

            int count =  0;
            int countBuckets = 0;

            for(auto& bucket : buckets)
            {
                countBuckets++;

                auto frame =  bucket.first->pop();
                auto neighbors = bucket.second;

                while(frame->size() > 0)
                {
                    auto particle = frame->back();
                    frame->pop_back();


                    for(auto other : *frame)
                    {
                        // particle.interact(other,interact__);
                        particle.interact(other);
                    }

                    for(auto other : neighbors)
                    {
                        // particle.interact(other,interact__);
                        particle.interact(other);
                    }

                    // particle.updateVelocity(updateVelocity__);
                    particle.updateVelocity();
                    particle.updatePostion();

                    nextBuckets.insert(particle);
                    count++;
                }

#ifdef DEBUG
                for(auto sizes : buckets.getBucketsSize())
                {
                    std::cout << sizes <<" ";
                }
                std::cout << std::endl;
#endif
            }

            // std::cout << "countBuckers: " << countBuckets << std::endl;
            buckets = nextBuckets;
            nextBuckets = Buckets(bucketSize,bucketSize);
        }

        void create_pool()
        {
            workers = Pool<std::pair<Particle,std::vector<Particle>>,Particle>(
                5,
                task
            );
        };

        void step_MT()
        {
            for(auto& bucket : buckets)
            {
                auto frame =  bucket.first->pop();

                while(frame->size() > 0)
                {
                    auto particle = frame->back();
                    frame->pop_back();
                    for(auto other : *frame){
                        bucket.second.push_back(other);
                    }
                    workers.push(std::make_pair(particle,bucket.second));
                }
            }

            workers.await();

            for(auto particle : workers.getResults()){
                nextBuckets.insert(particle);
            };

            buckets = nextBuckets;
            nextBuckets = Buckets(bucketSize,bucketSize);
        }

        void __testCreatePairs(){
            for(auto& bucket : buckets)
            {
                auto frame =  bucket.first->pop();

                while(frame->size() > 0)
                {
                    auto tmp = std::make_pair(frame->back(),bucket.second);
                    frame->pop_back();
                }

            }
        }

        std::vector<Particle> getParticles()
        {
            auto tmp = buckets.getAllBuckets();
            return tmp;
        };
};
