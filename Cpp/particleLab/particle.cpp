#include <vector>
#include <array>
#include <utility>
#include <functional>
// Library effective with Windows
#include <windows.h>
#include <chrono>
#include <iostream>
#include <random>

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
    double v_x = 0.0, v_y = 0.0;
    double f_x ,f_y;
    double dt = 0.1;
    double friction = 0.01;
    double rMax = 10.0;
    double force = 50.0;
    double beta = 0.3;

    public:
        Particle(double x, double y)
        {
            this->x = x;
            this->y = y;
        }

        void interact(Particle& other){
            double relation = 1.0;

            float r = sqrt(pow(other.x - x,2) + pow(other.y - y,2));

            if(0 < r && r < 10)
            {
                float f = F(r/rMax,relation,0.3);
                f_x += (other.x - x)/r * 50.0;
                f_y += (other.y - y)/r * 50.0;
            }
        };

        void Particle::updateVelocity()
        {
            f_x *= rMax * force;
            f_y *= rMax * force;

            v_x *= friction;
            v_y *= friction;

            v_x += f_x * dt;
            v_y += f_y * dt;
        }


        void Particle::updatePostion()
        {
            x += v_x * dt;
            y += v_y * dt;
        }

};

class ParticleSystem
{
    private:
        std::vector<Particle> frame;
        std::vector<Particle> nextFrame;

    public:
        void init(int initSize = 10000)
        {
            double lower_bound = 0.0;
            double upper_bound = 1.0;
            std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
            std::default_random_engine re;

            for(int i = 0 ; i < initSize ; i++)
            {

                frame.push_back(Particle(unif(re),unif(re)));
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
    system.init(1000);
    timeit([&system](){
        system.run();
    });
    return 0;
}