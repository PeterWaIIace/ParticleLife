#include <map>
#include <array>
#include <random>

namespace particle_life {
    
    class Particle
    {  
        public:
        double x;
        double y;
        double radius;
        int x_d;
        int y_d;
        int flavour;

        Particle(): x(0), y(0), flavour(0){};
        Particle(double _x,double _y,int _flavour): x(_x), y(_y), flavour(_flavour){};

        void update(double dx, double dy)
        {
            x += dx;
            y += dy;
            x_d = int(x);
            y_d = int(y);
        };
    };

    class ParticlesMatrix
    {
        std::array<std::array<double,100>,100> matrix100x100 = {0.0}; // Well trying to work with predefined huge space

        ParticlesMatrix(int flavours)
        {
            for(int i = 0; i < flavours ; i++)
            {
                for(int j = 0; j < flavours ; j++)
                {
                    // Create a random device to seed the random number generator
                    std::random_device rd;
                    // Use a Mersenne Twister random number generator
                    std::mt19937 gen(rd());
                    // Create a uniform real distribution between -1.0 and 1.0
                    std::uniform_real_distribution<double> dis(-1.0, 1.0);
                    matrix100x100[i][j] = dis(gen);
                }
            }
        }

        double getRelation(int flav1, int flav2)
        {
            return matrix100x100[flav1][flav2];
        }
    };

    class SpatialHasher
    {
        std::map<std::string,std::array<Particle,10000>> spatial_hash;
        std::map<std::string,int> spatial_depth;

        SpatialHasher()
        {};
        
        void push_back(Particle p)
        {
            std::string result = std::to_string(p.x_d) + "_" + std::to_string(p.y_d);
            
        }
    };

}