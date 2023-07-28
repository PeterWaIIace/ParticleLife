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

        int size(){
            return frame.size();
        };
};
class Buckets
{
    public:
        double nHeight = 1.0;
        double nWidth = 1.0;
        double stepHeight = 1.0;
        double stepWidth = 1.0;
        int nBucketsHeight = 1;
        int nBucketsWidth = 1;
        int interatorIndex = 0;

        std::vector<std::vector<Bucket>> b__;
    private:

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

        struct Iterator 
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = Bucket;
            using pointer           = Bucket*;  // or also value_type*
            using reference         = Bucket&;  // or also value_type&

            Iterator(pointer ptr,Buckets* baseThis) : m_ptr(ptr), m_baseThis(baseThis) {
                std::cout << "constructing " << m_baseThis << std::endl;
            }

            std::pair<pointer,std::vector<Particle>> operator*() const {  return std::make_pair(m_ptr,m_baseThis->getSurroundingBuckets()); }
            std::pair<pointer,std::vector<Particle>> operator->() { return std::make_pair(m_ptr,m_baseThis->getSurroundingBuckets()); }

            // Prefix increment
            Iterator& operator++() {
                m_baseThis->interatorIndex++;
                int row = m_baseThis->interatorIndex/m_baseThis->nBucketsWidth;
                int col = m_baseThis->interatorIndex%m_baseThis->nBucketsWidth;
                m_ptr = &(m_baseThis->b__[row][col]);
                if(m_baseThis->interatorIndex > m_baseThis->nBucketsWidth*m_baseThis->nBucketsHeight) m_baseThis->interatorIndex = 0;
                return *this;
            }

            // Postfix increment
            Iterator operator++(int) {
                int row = m_baseThis->interatorIndex/m_baseThis->nBucketsWidth;
                int col = m_baseThis->interatorIndex%m_baseThis->nBucketsWidth;
                m_ptr = &(m_baseThis->b__[row][col]);
                m_baseThis->interatorIndex++;
                if(m_baseThis->interatorIndex > m_baseThis->nBucketsWidth*m_baseThis->nBucketsHeight) m_baseThis->interatorIndex = 0;
                return *this;
            }

            friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

        private:

            pointer m_ptr;
            Buckets* m_baseThis;

        };


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
            nBucketsHeight = 1.0/nHeight;
            nBucketsWidth  = 1.0/nWidth;

            for(int n = 0 ; n < nBucketsHeight ; n++)
            {
                b__.push_back(std::vector<Bucket>());

                for(int i = 0 ; i < nBucketsWidth ; i++)
                {
                    b__[n].push_back(Bucket());
                }
            }
        }

        std::vector<int> getBucketsSize(){
            std::vector<int> sizes;
            for(int row = 0; row < nBucketsHeight ; row++){
                for(int col = 0; col < nBucketsWidth ; col++){
                    sizes.push_back(b__[row][col].size());
                }
            }
            return sizes;
        };

        Iterator begin() { return Iterator(&b__[0][0],this); }
        Iterator end()   { return Iterator(&b__[nBucketsHeight-1][nBucketsWidth-1],this); } // 200 is out of bounds

        void insert(Particle& particle)
        {
            auto& bucket = chooseBucket(particle.x,particle.y);
            bucket.insert(particle);
        }


        std::vector<Particle>& getBucket(int i , int j)
        {
            return b__[i][j].pop();
        }

        std::vector<Particle> getSurroundingBuckets()
        {
            int row = interatorIndex/nBucketsWidth;
            int col = interatorIndex%nBucketsWidth;
            return getSurroundingBuckets(row,col);
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

                    if(row >= 0 && col >= 0 && row != i && col != j && row < nBucketsHeight && col < nBucketsWidth)
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
            for(auto& bucket : buckets)
            {
                auto frame = bucket.first->pop();
                auto neighbors = bucket.second;

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

                    // particle.updateVelocity();
                    // particle.updatePostion();

                    nextBuckets.insert(particle);
                }

                for(auto i : buckets.getBucketsSize())
                {
                    std::cout << i <<" ";
                }
                std::cout << std::endl;
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