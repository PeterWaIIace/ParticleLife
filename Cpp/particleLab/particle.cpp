#include <ParticleSystem.hpp>

int main(int argc, char* argv[])
{
    ParticleSystem system;
    int numberOfParticles = atoi(argv[1]);
    system.init(numberOfParticles);

    timeit([&system](){
        system.step(
            // Step 1 Let particle interact
            [](Particle& main, Particle& other){
                float relation = 1.0;
                float r = sqrt(pow(other.x - main.x,2) + pow(other.y - main.y,2));

                if(0 < r && r < main.rMax)
                {
                    float f = F(r/main.rMax,relation,0.3);
                    main.f_x += ((other.x - main.x)/r) * f;
                    main.f_y += ((other.y - main.y)/r) * f;

                    // do same for other particle so it gets update anyway
                    other.f_x += ((main.x - other.x)/r) * f;
                    other.f_y += ((main.y - other.y)/r) * f;
                }
            },
            // Step 2 Update its velocity
            [](Particle& main){
                main.f_x *= main.rMax * main.force;
                main.f_y *= main.rMax * main.force;

                main.v_x *= main.friction;
                main.v_y *= main.friction;

                main.v_x += main.f_x * main.dt;
                main.v_y += main.f_y * main.dt;
            }

        );
    });

    // std::cout << "system.getParticles(): " << &system.getParticles() <<  "system.getParticles().size(): " << system.getParticles().size() << std::endl;
    std::cout << system.getParticles().size() << std::endl;

    system.create_pool();
    timeit([&system](){
        system.step_MT();
    });

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