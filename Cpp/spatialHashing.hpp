#pragma once

#include <unordered_map>

#include "particle.hpp"

class SpatialHash {
public:
    
    SpatialHash(float cell_size);

    void Add(const Particle& particle);

    std::vector<Particle>& GetObjectsInCell(int cell_x, int cell_y);
    std::vector<Particle> GetNearby(const Particle& particle) const;

    void Clear();

private:
    int GetCellID(int cell_x, int cell_y) const;

    float cell_size_;
    int num_cols_ = 100; // number of columns in the grid
    std::unordered_map<int, std::vector<Particle>> objects_by_cell_;

    // Particle objects_by_cell_[][][];
};
