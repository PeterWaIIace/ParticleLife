#include "RelationalFrames.hpp"
#include "DiagnosticLog.hpp"
#include "Particle.hpp"
#include <gtest/gtest.h>
#include <chrono>
#include <random>
#include <future>

TEST(RelationalBucket, createFrames) {
    unsigned int bucketSize = 5;
    RelationalFrames frames(bucketSize);

    ASSERT_EQ(bucketSize*bucketSize,frames.size());
}

TEST(RelationalBucket, insertOneParticle) {
    unsigned int bucketSize = 5;
    RelationalFrames frames(bucketSize);

    frames.insert(Particle());

    ASSERT_EQ(bucketSize*bucketSize,frames.size());
    ASSERT_EQ(1,frames.relations());
}

TEST(RelationalBucket, insertFiveParticleEQFiveRelationVectors) {
    unsigned int bucketSize = 5;
    RelationalFrames frames(bucketSize);

    frames.insert(Particle());
    frames.insert(Particle());
    frames.insert(Particle());
    frames.insert(Particle());
    frames.insert(Particle());

    ASSERT_EQ(bucketSize*bucketSize,frames.size());
    ASSERT_EQ(5,frames.relations());
}

TEST(RelationalBucket, checkBucketsSizesSanity) {
    unsigned int bucketSize = 5;
    RelationalFrames frames(bucketSize);

    frames.insert(Particle(0.01,0.01));
    frames.insert(Particle(0.011,0.011));
    frames.insert(Particle(0.010,0.011));
    frames.insert(Particle(0.011,0.010));
    frames.insert(Particle(0.009,0.009));

    auto relationSizes = frames.relationSizes();

    ASSERT_EQ(4,relationSizes[0]);
    ASSERT_EQ(3,relationSizes[1]);
    ASSERT_EQ(2,relationSizes[2]);
    ASSERT_EQ(1,relationSizes[3]);
    ASSERT_EQ(0,relationSizes[4]);
}

TEST(RelationalBucket, checkBucketsSizesSanityWithTwoSetsOfParticles) {
    unsigned int bucketSize = 5;
    RelationalFrames frames(bucketSize);

    frames.insert(Particle(0.01,0.01));
    frames.insert(Particle(0.011,0.011));
    frames.insert(Particle(0.010,0.011));
    frames.insert(Particle(0.011,0.010));
    frames.insert(Particle(0.009,0.009));

    frames.insert(Particle(0.11 ,0.11));
    frames.insert(Particle(0.111,0.111));
    frames.insert(Particle(0.110,0.111));
    frames.insert(Particle(0.111,0.110));
    frames.insert(Particle(0.109,0.109));

    auto relationSizes = frames.relationSizes();

    ASSERT_EQ(4,relationSizes[0]);
    ASSERT_EQ(3,relationSizes[1]);
    ASSERT_EQ(2,relationSizes[2]);
    ASSERT_EQ(1,relationSizes[3]);
    ASSERT_EQ(0,relationSizes[4]);

    ASSERT_EQ(4,relationSizes[5]);
    ASSERT_EQ(3,relationSizes[6]);
    ASSERT_EQ(2,relationSizes[7]);
    ASSERT_EQ(1,relationSizes[8]);
    ASSERT_EQ(0,relationSizes[9]);
}

int getKey(double x, double y, unsigned int nBuckets)
{
    double step = 1.0/double(nBuckets)+0.01;
    int key = int(x/step) + (int(y/step) * (nBuckets));
    return key;
}

TEST(RelationalBucket, checkKeyFunction) {

    unsigned int nBuckets = 5;
    double step = 0.01;
    int size = 101;

    for(int i = 0 ; i < size ; i++)
    {
        for(int j = 0 ; j < size ; j++)
        {
            ASSERT_LE(getKey(step*double(j),step*double(i),nBuckets),(nBuckets*nBuckets)-1);
        }
    }

    ASSERT_TRUE(true);
}

#define BUCKETS 20

TEST(RelationalBucket, checkIfAlgorithmIsInTimeConstraints1000samples) {
    unsigned int bucketSize = BUCKETS;
    RelationalFrames frames(bucketSize);

    double lower_bound = 0.0;
    double upper_bound = 1.0;

    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;

    auto time = timeit([&frames,&unif,&re](){
        for(int n = 0; n < 1000 ; n++)
        {
            float x = unif(re);
            float y = unif(re);
            ASSERT_LE(x,1.0);
            ASSERT_LE(y,1.0);

            frames.insert(Particle(x,y));
        }
    });
    //in 50 ms
    ASSERT_LE(time,50);
}


TEST(RelationalBucket, checkIfAlgorithmIsInTimeConstraints5000samples) {
    unsigned int bucketSize = BUCKETS;
    RelationalFrames frames(bucketSize);

    double lower_bound = 0.0;
    double upper_bound = 1.0;

    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;

    auto time = timeit([&frames,&unif,&re](){
        for(int n = 0; n < 5000 ; n++)
        {
            float x = unif(re);
            float y = unif(re);
            ASSERT_LE(x,1.0);
            ASSERT_LE(y,1.0);

            frames.insert(Particle(x,y));
        }
    });
    //in 50 ms
    ASSERT_LE(time,500);
}

TEST(RelationalBucket, checkIfAlgorithmIsInTimeConstraints10000samples) {
    unsigned int bucketSize = BUCKETS;
    RelationalFrames frames(bucketSize);

    double lower_bound = 0.0;
    double upper_bound = 1.0;

    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;

    auto time = timeit([&frames,&unif,&re](){
        for(int n = 0; n < 10000 ; n++)
        {
            float x = unif(re);
            float y = unif(re);
            ASSERT_LE(x,1.0);
            ASSERT_LE(y,1.0);

            frames.insert(Particle(x,y));
        }
    });
    //in 50 ms
    ASSERT_LE(time,500);
}