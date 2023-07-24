#include <gtest/gtest.h>
#include <spatialHashing.hpp>
#include <spatialHashing_experimental.hpp>
#include <particle.hpp>
#include <chrono>
#include <future>


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
  ASSERT_GE(10000.0,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}


TEST(LegacySpatialHashing, BasicInit) {
  using namespace std::chrono_literals;
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  SpatialHash sh(0.1);

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(0.1,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}

#define LAST_MEASURED_TIME_FOR_BASIC_INSERT_LEGACY 50
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


#define LAST_MEASURED_TIME_FOR_BASIC_INIT_EXPERIMENTAL 50
TEST(ExperimentalSpatialHashing, BasicInit) {
  using namespace std::chrono_literals;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  experimental::SpatialHash e_sh(10);

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_INIT_EXPERIMENTAL,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}


#define LAST_MEASURED_TIME_FOR_BASIC_INSERT_EXPERIMENTAL 50
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

#define LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_EXPERIMENTAL 10000
TEST(ExperimentalSpatialHashing, BasicPerformance) {
  using namespace std::chrono_literals;
  int nElements = 10000;
  std::vector<experimental::Particle_exp> elements;
  // setup latice of particles
  for(int i = 0 ; i < nElements ; i++)
  {
      experimental::Particle_exp el;
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
      elements[i].addForce(neighbour,1.0);
    };
  }

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_EXPERIMENTAL,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}

#define LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_ONE_EXPERIMENTAL 5
TEST(ExperimentalSpatialHashing_Async, BasicPerformanceOnlyForOneElement) {
  using namespace std::chrono_literals;
  int nElements = 10000;
  std::vector<experimental::Particle_exp> elements;
  // setup latice of particles
  for(int i = 0 ; i < nElements ; i++)
  {
      experimental::Particle_exp el;
      el.x = i/nElements;
      el.y = i/nElements;
      elements.push_back(el);
  }

  experimental::SpatialHash e_sh(10);
  for(int i = 0 ; i < nElements ; i++)
  {
    e_sh.insert(elements[i]);
  }

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  int i = 0;
  // to make it even we need iterate over that buffer
  for(auto neighbour : e_sh.get(elements[i]))
  {
    elements[i].addForce(neighbour,1.0);
  };

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_ONE_EXPERIMENTAL,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}

bool isReady(const std::future<void>& f) {
    if (f.valid()) { // otherwise you might get an exception (std::future_error: No associated state)
        return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    } else {
        return false;
    }
}

#define LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_ASYNC_EXPERIMENTAL 300
TEST(ExperimentalSpatialHashing_Async, BasicPerformanceOnlyForAsyncElement) {
  using namespace std::chrono_literals;
  int nElements = 10000;
  std::vector<experimental::Particle_exp> elements;
  // setup latice of particles
  for(int i = 0 ; i < nElements ; i++)
  {
      experimental::Particle_exp el;
      el.x = i/nElements;
      el.y = i/nElements;
      elements.push_back(el);
  }

  experimental::SpatialHash e_sh(10);
  for(int i = 0 ; i < nElements ; i++)
  {
    e_sh.insert(elements[i]);
  }

  // to make it even we need iterate over that buffer
  std::vector<std::future<void>> futures;
  int readyFutures = 0;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  for(int i = 0 ; i < nElements ; i++){
    futures.push_back(
      std::async(std::launch::async,[&elements,&e_sh,i](){
        for(auto neighbour : e_sh.get(elements[i]))
        {
          elements[i].addForce(neighbour,1.0);
        };
      })
    );
  }

  do {
    for (auto &future : futures) {
      if (isReady(future)) {
        readyFutures++;
      }
    }
  } while (readyFutures < futures.size());

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_ASYNC_EXPERIMENTAL,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}


#define LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_ASYNC_EXPERIMENTAL 200
TEST(ExperimentalSpatialHashing_Async, AsyncLaunch) {
  using namespace std::chrono_literals;
  int nElements = 10000;
  std::vector<experimental::Particle_exp> elements;
  // setup latice of particles
  for(int i = 0 ; i < nElements ; i++)
  {
      experimental::Particle_exp el;
      el.x = i/nElements;
      el.y = i/nElements;
      elements.push_back(el);
  }

  experimental::SpatialHash e_sh(10);
  for(int i = 0 ; i < nElements ; i++)
  {
    e_sh.insert(elements[i]);
  }

  // to make it even we need iterate over that buffer
  std::vector<std::future<void>> futures;
  int readyFutures = 0;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  for(int i = 0 ; i < nElements ; i++){
    futures.push_back(
      std::async(std::launch::async,[&elements,&e_sh,i](){
        for(auto neighbour : e_sh.get(elements[i]))
        {
          elements[i].addForce(neighbour,1.0);
        };
      })
    );
  }

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_ASYNC_EXPERIMENTAL,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}

#define LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_ASYNC_EXPERIMENTAL 10
TEST(ElementTest, AddingForce) {
  using namespace std::chrono_literals;
  int nElements = 10000;
  experimental::Particle_exp element1;
  experimental::Particle_exp element2;

  element1.x = 0.12;
  element1.y = 0.12;
  element2.x = 0.11;
  element2.y = 0.11;

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  element1.addForce(element2,1.0);
  element2.addForce(element1,1.0);

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  // 4 seconds for this number of particles
  ASSERT_GE(LAST_MEASURED_TIME_FOR_BASIC_PERFORMANCE_ASYNC_EXPERIMENTAL,std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}