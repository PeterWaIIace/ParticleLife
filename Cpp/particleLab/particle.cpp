// this file should be named differently than particle

#include <ParticleSystem.hpp>
#include <DiagnosticLog.hpp>
#include <Particle.hpp>

int main(int argc, char* argv[])
{
    ParticleSystem system;

    int steps = 15;
    int numberOfParticles = atoi(argv[1]);
    unsigned int bucketSize = atoi(argv[2]);
    unsigned int poolSize =  atoi(argv[3]);
    system.init(numberOfParticles,bucketSize);

    for(int n = 0; n < steps ; n++){
        std::cout << "=====================================================================" << std::endl;
        timeit([&system](){
            system.step();
        });
        std::cout << system.getExperimentalParticlesFromRelationalFrames().size() << std::endl;
    }

    // std::cout << "system.getParticles(): " << &system.getParticles() <<  "system.getParticles().size(): " << system.getParticles().size() << std::endl;

    ParticleSystem system2;
    system2.init(numberOfParticles,bucketSize);
    system2.create_pool(poolSize);

    for(int n = 0 ; n  < steps ; n++)
    {
        timeit([&system2](){
            system2.step_MT();
        });
    }

    // Particle el1(0.01,0.01);
    // Particle el2(0.01,0.011);

    // std::cout << " el1.x: " << el1.x << " el1.y: " << el1.y << std::endl;
    // std::cout << " el2.x: " << el2.x << " el2.y: " << el2.y << std::endl;
    // el1.interact(el2);
    // el1.updateVelocity();
    // el1.updatePostion();

    // std::cout << " el1.x: " << el1.x << " el1.y: " << el1.y << std::endl;
    return 0;
}