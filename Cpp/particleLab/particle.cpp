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

    public:
        double x, y;
        double v_x = 0.0, v_y = 0.0;
        double f_x ,f_y;
        double dt = 0.1;
        double friction = 0.01;
        double rMax = 10.0;
        double force = 50.0;
        double beta = 0.3;

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


class Bucket
{
    private:
        int row = 0;
        int col = 0;
        std::vector<Particle> frame;
    public:
        void insert(Particle& particle)
        {
            frame.push_back(particle);
        };
        std::vector<Particle>& pop(){
            return frame;
        };
};
class Buckets
{
    private:
        double nHeight = 1.0;
        double nWidth = 1.0;
        double stepHeight = 1.0;
        double stepWidth = 1.0;
        std::vector<std::vector<Bucket>> b__;


        Bucket& chooseBucket(double x, double y)
        {
            if(x < 0.0) x = 0.0;
            if(y < 0.0) y = 0.0;
            if(x > 1.0) x = 1.0;
            if(y > 1.0) y = 1.0;

            int row = int(x*stepHeight);
            int col = int(y*stepWidth);
            return b__[row][col];
        }

    public:
        Buckets(Buckets& other)
        {
            this->b__ = b__;
            this->nWidth = nWidth;
            this->nHeight = nHeight;

        }
        Buckets(double nHeight = 1.0,double nWidth = 1.0)
        {
            stepHeight = 1.0/nHeight;
            stepWidth = 1.0/nWidth;
            int nBucketsHeight = 1.0/nHeight;
            int nBucketsWidth  = 1.0/nWidth;

            for(int n = 0 ; n < nBucketsHeight ; n++)
            {
                b__.push_back(std::vector<Bucket>());

                for(int i = 0 ; i < nBucketsWidth ; i++)
                {
                    b__[n].push_back(Bucket());
                }
            }
        }

        void insert(Particle& particle)
        {
            auto& bucket = chooseBucket(particle.x,particle.y);
            bucket.insert(particle);
        }


        std::vector<Particle>& getBucket(int i , int j)
        {
            return b__[i][j].pop();
        }

        std::vector<Particle> getSurroundingBuckets(int i , int j)
        {
            std::vector<Particle> neighbors;
            for(int n = -1 ; n <= 1 ; n++)
            {
                for(int m = -1 ; m <= 1 ; m++)
                {
                    int row = i + m;
                    int col = j + m;

                    if(row >= 0 && col >= 0 && row != i && col != j)
                    {
                        neighbors.insert(neighbors.end(),b__[row][col].pop().begin(),b__[row][col].pop().end());
                    }
                }
            }
            return neighbors;
        }
};

class ParticleSystem
{
    private:
        // std::vector<Particle> frame;
        // std::vector<Particle> nextFrame;
        Buckets buckets = Buckets(0.1,0.1);
        Buckets nextBuckets = Buckets(0.1,0.1);

    public:
        void init(int initSize = 10000)
        {
            double lower_bound = 0.0;
            double upper_bound = 1.0;
            std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
            std::default_random_engine re;

            for(int i = 0 ; i < initSize ; i++)
            {
                buckets.insert(Particle(unif(re),unif(re)));
            }
        }

        void run()
        {
            auto frame = buckets.getBucket(0,0);
            auto neighbors = buckets.getSurroundingBuckets(0,0);

            std::cout << "frame.size: " << frame.size() << " neighbors.size: " << neighbors.size() << std::endl;
            while(frame.size() > 0)
            {
                auto particle = frame.front();
                frame.pop_back();

                for(auto other : frame)
                {
                    particle.interact(other);
                    other.interact(particle);
                }

                for(auto other : neighbors)
                {
                    particle.interact(other);
                    other.interact(particle);
                }

                particle.updateVelocity();
                particle.updatePostion();

                nextBuckets.insert(particle);
            }

            buckets = nextBuckets;

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