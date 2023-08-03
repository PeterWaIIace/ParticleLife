#include "../particleLab/ParticleSystem.hpp"
#include <gtest/gtest.h>
#include <chrono>
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