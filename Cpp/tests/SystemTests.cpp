#include "../particleLab/ParticleSystem.hpp"
#include <gtest/gtest.h>
#include <chrono>
#include <future>

TEST(Buckets, Init5x5) {
    float bucketSize = 0.2;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,5);
    ASSERT_EQ(buckets.nBucketsWidth,5);
}

TEST(Buckets, Init10x10) {
    float bucketSize = 0.1;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,10);
    ASSERT_EQ(buckets.nBucketsHeight,10);
}

TEST(Buckets, Init100x100) {
    float bucketSize = 0.01;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,100);
    ASSERT_EQ(buckets.nBucketsHeight,100);
}

TEST(Buckets, Init1x1) {
    float bucketSize = 1.0;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,1);
    ASSERT_EQ(buckets.nBucketsWidth,1);
}

TEST(Buckets, Init1x1Wrong) {
    float bucketSize = 1.2;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,1);
    ASSERT_EQ(buckets.nBucketsWidth,1);
}

TEST(Buckets, Init1x1Wrong2) {
    float bucketSize = 2.0;
    Buckets buckets = Buckets(bucketSize,bucketSize);

    ASSERT_EQ(buckets.nBucketsHeight,1);
    ASSERT_EQ(buckets.nBucketsWidth,1);
}