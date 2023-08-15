#include "ParticleSystem.hpp"
#include <gtest/gtest.h>
#include <chrono>
#include <random>
#include <future>

TEST(Buckets, Init5x5) {
    int bucketSize = 5;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,5);
    ASSERT_EQ(buckets.nBucketsWidth,5);
}

TEST(Buckets, Init10x10) {
    int bucketSize = 10;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,10);
    ASSERT_EQ(buckets.nBucketsHeight,10);
}

TEST(Buckets, Init100x100) {
    int bucketSize = 100;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,100);
    ASSERT_EQ(buckets.nBucketsHeight,100);
}

TEST(Buckets, Init1x1) {
    float bucketSize = 1;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,1);
    ASSERT_EQ(buckets.nBucketsWidth,1);
}


TEST(Buckets, Load2x2) {
    float bucketSize = 2;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    // TODO: prepare loading for bucket of that size
    int initSize = 20;
    double lower_bound = 0.0;
    double upper_bound = 1.0;

    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;

    for(int i = 0 ; i < initSize ; i++)
    {
        buckets.insert(Particle(unif(re),unif(re)));
    }

    for(auto sizes : buckets.getBucketsSize())
    {
        // check if there are at least 2 particles in each bucket
        ASSERT_GE(sizes,2);
    }
}

TEST(Buckets, RunOneStepAndCheckSamples) {
    ParticleSystem system;

    int steps = 1;
    int bucketSize = 20;
    int numberOfParticles = 10000;
    system.init(numberOfParticles,bucketSize);

    system.step();

    ASSERT_EQ(system.getExperimentalParticlesFromRelationalFrames().size(), numberOfParticles);
}


TEST(Buckets, RunTenStepAndCheckSamples) {
    ParticleSystem system;

    int steps = 10;
    int bucketSize = 20;
    int numberOfParticles = 10000;
    system.init(numberOfParticles,bucketSize);

    for(int n = 0; n < steps ; n++)
    {
        system.step();
    }

    ASSERT_EQ(system.getExperimentalParticlesFromRelationalFrames().size(), numberOfParticles);
}


TEST(Buckets, RunOneHundredStepAndCheckSamples) {
    ParticleSystem system;

    int steps = 100;
    int bucketSize = 20;
    int numberOfParticles = 10000;
    system.init(numberOfParticles,bucketSize);

    for(int n = 0; n < steps ; n++)
    {
        system.step();
    }

    ASSERT_EQ(system.getExperimentalParticlesFromRelationalFrames().size(), numberOfParticles);
}