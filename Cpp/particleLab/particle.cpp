#include <vector>
#include <array>
#include <utility>
#include <functional>
// Library effective with Windows
#include <windows.h>
#include <chrono>
#include <iostream>
using namespace std::chrono;

double F(float distance,float a, float b)
{
    if(distance < b)
    {
        return distance/b - 1;
    }
    else if(b < distance && distance < 1.0)
    {
        return a * (1 - fabs(2.0 * distance - 1.0 - b)/(1 - b));
    }
    else
    {
        return 0.0;
    }
}

class Particle{
    // MOCK

    double x, y;
    double xDiff,yDiff;
    double f_x ,f_y;
    public:
        void interact(Particle& other){
            double relation = 1.0;
            xDiff  = other.x - x;
            yDiff  = other.y - y;

            float r = sqrt(pow(other.x - x,2) + pow(other.y - y,2));

            if(0 < r && r < 10)
            {
                float f = F(r/10,relation,0.3);
                f_x += (other.x - x)/r * 50.0;
                f_y += (other.y - y)/r * 50.0;
            }
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