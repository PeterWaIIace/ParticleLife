#include "RelationalFrames.hpp"
#include <gtest/gtest.h>
#include <chrono>
#include <random>
#include <future>

TEST(RelationalBucket, create_object) {
    unsigned int bucketSize = 5;
    RelationalFrames frames(bucketSize);

    ASSERT_TRUE(true);
}
