#include <gtest/gtest.h>
#include <spatialHashing.hpp>
#include <spatialHashing_experimental.hpp>
#include <particle.hpp>
#include <chrono>


// Demonstrate some basic assertions.
TEST(LegacySpatialHashing, BasicPerformance) {
  using namespace std::chrono_literals;
  SpatialHash sh(0.1);
  int nParticles = 10000;

  std::vector<Particle> particles;

  // setup latice of particles
  for(int i = 0 ; i < nParticles ; i++)
  {
      particles.push_back(Particle(
          (i - nParticles/2)/(nParticles/2), // X
          (i - nParticles/2)/(nParticles/2), // Y
          0  // COLOR
      ));
  }

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  for(int i = 0 ; i < nParticles ; i++)
  {
    sh.Add(particles[i]);
  }

  for(int i = 0 ; i < nParticles ; i++)
  {
    sh.GetNearby(particles[i]);
  }

  sh.Clear();

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(4000.0,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}


TEST(LegacySpatialHashing, BasicInit) {
  using namespace std::chrono_literals;
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  SpatialHash sh(0.1);

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(0.1,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}

#define LAST_MEASURED_TIME_FOR_BASIC_INSERT_LEGACY 21
TEST(LegacySpatialHashing, BasicInsert) {
  using namespace std::chrono_literals;
  int nParticles = 10000;
  std::vector<Particle> particles;
  // setup latice of particles
  for(int i = 0 ; i < nParticles ; i++)
  {
      particles.push_back(Particle(
          (i - nParticles/2)/(nParticles/2), // X
          (i - nParticles/2)/(nParticles/2), // Y
          0  // COLOR
      ));
  }

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  SpatialHash sh(0.1);

  for(int i = 0 ; i < nParticles ; i++)
  {
    sh.Add(particles[i]);
  }

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_INSERT_LEGACY,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}


#define LAST_MEASURED_TIME_FOR_BASIC_INIT_EXPERIMENTAL 15
TEST(ExperimentalSpatialHashing, BasicInit) {
  using namespace std::chrono_literals;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  experimental::SpatialHash e_sh(10);

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_INIT_EXPERIMENTAL,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}


#define LAST_MEASURED_TIME_FOR_BASIC_INSERT_EXPERIMENTAL 21
TEST(ExperimentalSpatialHashing, BasicInsert) {
  using namespace std::chrono_literals;
  int nElements = 10000;
  std::vector<experimental::Element> elements;
  // setup latice of particles
  for(int i = 0 ; i < nElements ; i++)
  {
      experimental::Element el;
      el.x = i/nElements;
      el.y = i/nElements;
      elements.push_back(el);
  }

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  experimental::SpatialHash e_sh(10);
  for(int i = 0 ; i < nElements ; i++)
  {
    e_sh.insert(elements[i]);
  }

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_INSERT_EXPERIMENTAL,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}

#define LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_EXPERIMENTAL 150
TEST(ExperimentalSpatialHashing, BasicPerformance) {
  using namespace std::chrono_literals;
  int nElements = 10000;
  std::vector<experimental::Element> elements;
  // setup latice of particles
  for(int i = 0 ; i < nElements ; i++)
  {
      experimental::Element el;
      el.x = i/nElements;
      el.y = i/nElements;
      elements.push_back(el);
  }

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  experimental::SpatialHash e_sh(10);
  for(int i = 0 ; i < nElements ; i++)
  {
    e_sh.insert(elements[i]);
  }

  for(int i = 0 ; i < nElements ; i++)
  {
    // to make it even we need iterate over that buffer
    for(auto neighbour : e_sh.get(elements[i]))
    {

    };
  }

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_EXPERIMENTAL,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}