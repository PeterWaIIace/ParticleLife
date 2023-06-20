#include "particle.hpp"
#include <iostream>

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

Particle::Particle(float x, float y, int color) : uniqueID(reinterpret_cast<uint32_t>(this))
{
    this->hashIndex = 0.0;
    this->force = 50.0;
    this->beta = 0.3;
    this->rMax = 0.1;
    this->dt   = 0.01;
    this->friction = 0.01;
    this->color = color;
    this->position = { x, y };
    this->velocity = {0.0,0.0};
}

void Particle::resetForce()
{
    this->totalForce = {0.0,0.0};
}

uint32_t Particle::getID()
{
    return this->uniqueID;
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

    if(this->position.x <= this->boundary[0]-0.01)
    {
        this->position.x = this->boundary[1];
    }

    if(this->position.y <= this->boundary[0]-0.01)
    {
        this->position.y = this->boundary[1];
    }

    if(this->position.x > this->boundary[1]+0.01)
    {
        this->position.x = this->boundary[0];
    }

    if(this->position.y > this->boundary[1]+0.01)
    {
        this->position.y = this->boundary[0];
    }

}

Vector2 Particle::getPosition(){
    return this->position;
}


void experimental::Particle_exp::addForce(Element& otherElement, float relation)
{
    xDiff  = otherElement.x - x;
    yDiff  = otherElement.y - y;

    float r = sqrt(pow(otherElement.x - x,2) + pow(otherElement.y - y,2));

    if(0 < r && r < 10)
    {
        float f = F(r/10,relation,0.3);
        f_x += (otherElement.x - x)/r * 50.0;
        f_y += (otherElement.y - y)/r * 50.0;
    }
}
