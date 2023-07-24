#include <vector>
#include <array>
#include <utility>
#include <functional>
// Library effective with Windows
#include <windows.h>
#include <chrono>
#include <iostream>
using namespace std::chrono;

class Particle{
    // MOCK
    public:
        void interact(Particle& other){
            Sleep(0.1);
        };
};

class ParticleSystem
{
    private:
        std::vector<Particle> frame;
        std::vector<Particle> nextFrame;

    public:
        void init(int initSize = 10000)
        {
            for(int i = 0 ; i < initSize ; i++)
            {
                frame.push_back(Particle());
            }
        }

        void run()
        {
            while(frame.size() > 0)
            {
                auto particle = frame.front();
                frame.pop_back();

                int next = 0;
                for(auto other : frame)
                {
                    particle.interact(other);
                    other.interact(particle);
                    next++;
                }

                nextFrame.push_back(particle);
            }
            frame = nextFrame;

        }
};

void timeit(std::function<void(void)> fn)
{
    auto start = high_resolution_clock::now();
    fn();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << "ms" << std::endl;
};

int main(int argc, char* argv[])
{
    ParticleSystem system;
    system.init(10000);
    timeit([&system](){
        system.run();
    });
    return 0;
}