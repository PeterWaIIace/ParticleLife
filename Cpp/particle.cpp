#include "particle.hpp"

double F(float distance,float a, float b)
{
    if(distance < b)
    {
        return distance/(b-1);
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

Particle::Particle(float x, float y, int color)
{
    this->force = 1.0;
    this->beta = 0.03;
    this->rMax = 0.1;
    this->dt   = 0.1;
    this->friction = 0.014;
    this->color = color;
    this->position = { x, y };
    this->y_limit = y_limit;
    this->x_limit = x_limit;
    this->velocity = {0.0,0.0};
}

void Particle::resetForce()
{
    this->totalForce = {0.0,0.0};
}

void Particle::addForce(Vector2 otherPostion, float relation)
{
    Vector2 diffV  = Vector2Subtract(this->position,otherPostion);
    float r = Vector2Distance(this->position,otherPostion);

    if(0 < r && r < this->rMax)
    {
        float f = F(r/this->rMax,relation,this->beta);
        this->totalForce.x += (otherPostion.x - this->position.x)/r * f;
        this->totalForce.y += (otherPostion.y - this->position.y)/r * f;
    }
}

void Particle::updateVelocity()
{
    this->totalForce.x *= this->rMax * this->force;
    this->totalForce.y *= this->rMax * this->force;

    this->velocity.x *= this->friction;
    this->velocity.y *= this->friction;

    this->velocity.x += this->totalForce.x * this->dt;
    this->velocity.y += this->totalForce.y * this->dt;
}


void Particle::updatePostion()
{
    this->position.x += this->velocity.x * this->dt;
    this->position.y += this->velocity.y * this->dt;

    if(this->position.x < 0)
    {
        this->position.x = 0.0;
    }

    if(this->position.y < 0)
    {
        this->position.y = 0.0;
    }

    if(this->position.x > x_limit)
    {
        this->position.x = x_limit;
    }

    if(this->position.y > y_limit)
    {
        this->position.y = y_limit;
    }


}

Vector2 Particle::getPosition(){
    return this->position;
}
