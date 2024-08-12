#include <map>

class Particle
{  
    public:
    double x;
    double y;
    double radius;
    int x_d;
    int y_d;
    int flavour;

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

};

class SpatialHasher
{

};