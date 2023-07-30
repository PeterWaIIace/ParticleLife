#include <ParticleSystem.hpp>

int main(int argc, char* argv[])
{
    ParticleSystem system;
    system.init(10000);
    timeit([&system](){
        system.step();
    });

    timeit([&system](){
        system.step();

    });
    std::cout << system.getParticles().size() << std::endl;

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