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
#include "DiagnosticLog.hpp"
#include "Particle.hpp"
#include "HashedBuckets.hpp"
#include "RelationalFrames.hpp"

using namespace std::chrono;

inline Particle task(std::pair<Particle,std::vector<Particle>>& workload){
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

        RelationalFrames relFrameNow;
        RelationalFrames relFrameNext;

        Pool<std::pair<Particle,std::vector<Particle>>,Particle> workers;

    public:
        ParticleSystem(){};
        void init(int initSize = 10000, unsigned int bucketSize = 1)
        {
            double lower_bound = 0.0;
            double upper_bound = 1.0;

            this->bucketSize  = bucketSize;
            this->buckets     = Buckets(bucketSize,bucketSize);
            this->nextBuckets = Buckets(bucketSize,bucketSize);

            this->relFrameNow = RelationalFrames(bucketSize);
            this->relFrameNext= RelationalFrames(bucketSize);

            std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
            std::default_random_engine re;

            for(int i = 0 ; i < initSize ; i++)
            {
                this->buckets.insert(Particle(unif(re),unif(re)));
                this->relFrameNow.insert(Particle(unif(re),unif(re)));
            }
        }


        // void step(void (interact__)(Particle&,Particle&),void (updateVelocity__)(Particle&))
        void step()
        {

            int count =  0;
            int countBuckets = 0;

            for(auto& bucket : relFrameNow.getRelations())
            {
                countBuckets++;

                auto& particle = bucket.first;
                auto others   = bucket.second;
;
                for(auto other : others)
                {
                    // particle.interact(other,interact__);
                    particle.interact(*other);
                }

                // particle.updateVelocity(updateVelocity__);
                particle.updateVelocity();
                particle.updatePostion();

                relFrameNext.insert(particle);
                count++;

            }
            relFrameNow = relFrameNext;
            relFrameNext = RelationalFrames(bucketSize);
        }

        void create_pool(int poolSize = 1)
        {
            workers = Pool<std::pair<Particle,std::vector<Particle>>,Particle>(
                poolSize,
                task
            );
        };

        void step_MT()
        {
            timeit([this](){
            for(auto& bucket : buckets)
            {
                auto frame =  bucket.first->pop();

                while(frame->size() > 0)
                {
                    auto particle = frame->back();
                    frame->pop_back();

                    workers.push(std::make_pair(particle,*frame));
                }
            }
            },0,"pushing: ");

            // diagnostics

            timeit([this](){
            workers.await();
            },0,"awaiting: ");

            timeit([this](){
            for(auto particle : workers.getResults()){
                nextBuckets.insert(particle);
            };
            },0,"obtaining results: ");

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

        std::vector<Particle> getExperimentalParticlesFromRelationalFrames()
        {
            std::vector<Particle> particles;
            for(auto& bucket : relFrameNow.getRelations())
            {
                particles.push_back(bucket.first);
            }
            return particles;
        };

        size_t getRelations()
        {
            return relFrameNow.size();
        };
};
