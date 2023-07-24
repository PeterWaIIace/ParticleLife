#pragma once

#include <stdint.h>
#include "raylib.h"
#include "raymath.h"
#include "spatialHashing_experimental.hpp"

double F(float distance,float a, float b);

class Particle
{
    public:
        Particle(float x, float y, int color);

        void resetForce();

        void addForce(Vector2 otherPostion, float relation);
        void updateVelocity();

        uint32_t getID();

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
        float boundary[2] = {-1.0,1.0};
        int hashIndex;
        float dt;
        float rMax;
        float radius;
        float friction;
        float beta;
        float force;

        uint32_t uniqueID;
        Vector2 velocity;

};

namespace experimental
{
    class Particle_exp : public Element
    {
        public:
            // Particle(float x, float y, int color);

            void addForce(Element& otherElement, float relation);

        private:
            float xDiff = 0.0;
            float yDiff = 0.0;
            float f_x = 0.0;
            float f_y = 0.0;
    };
};