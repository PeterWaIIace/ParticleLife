#pragma once

#include "raylib.h"
#include "raymath.h"
double F(float distance,float a, float b);

class Particle
{
    public:
        Particle(float x, float y, int color);

        void resetForce();

        void addForce(Vector2 otherPostion, float relation);
        void updateVelocity();

        void updatePostion();
        Vector2 getPosition();

        bool operator==(const Particle& other) const {
            return this == &other;
        }

        bool operator!=(const Particle& other) const {
            return !(*this == other);
        }

        int color = 0;
        Vector2 position;
    private:
        Vector2 totalForce = {0.0,0.0};
        int hashIndex = 0.0;
        float dt = 0.02;
        float rMax   = 1.0;
        float radius = 1;
        float y_limit = 1.0;
        float x_limit = 1.0;
        float friction = 0.001;
        float beta = 0.3;
        float force = 10.0;
        Vector2 velocity;

};