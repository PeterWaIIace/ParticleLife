#pragma once

#include "raylib.h"
#include "raymath.h"
#include <unordered_map>
#include <random>

#include "particle.hpp"

#define MAX_COLORS 8

class relationMatrix
{
    private:
        float arr[MAX_COLORS][MAX_COLORS];
        // float arr[MAX_COLORS][MAX_COLORS] = {{1.0,0.5,0.0},{0.0,1.0,0.5},{-0.1,0.0,1.0}};
        // float arr[MAX_COLORS][MAX_COLORS] = {{1.0}}; // one for TESTING
        std::random_device rd_;
        std::mt19937 gen_;
        std::uniform_int_distribution<> dist_;

        int relationMatrix::getRandomInt(int min, int max);
    public:
        relationMatrix::relationMatrix();
        float relationMatrix::getRelation(int firstColor,int secondColor);
};

class SpatialHash {
public:

    SpatialHash(float cell_size);

    void Add(const Particle& particle);

    std::vector<Particle>& GetObjectsInCell(int cell_x, int cell_y);
    void GetNearby(Particle& particle) const;

    void Clear();

private:
    int GetCellID(int cell_x, int cell_y) const;

    float cell_size_;
    int num_cols_ = 100; // number of columns in the grid
    std::unordered_map<int, std::vector<Particle>> objects_by_cell_;

    relationMatrix relations;
    // Particle objects_by_cell_[][][];
};
