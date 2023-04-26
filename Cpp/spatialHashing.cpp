#include "spatialHashing.hpp"
#include <iostream>

SpatialHash::SpatialHash(float cell_size) : cell_size_(cell_size) {
    int numberOfBuckets = int((1/cell_size)*(1/cell_size));
    objects_by_cell_.reserve(numberOfBuckets);

}

void SpatialHash::Add(const Particle& particle) {
    int cell_x = int(particle.position.x / cell_size_);
    int cell_y = int(particle.position.y / cell_size_);
    int cell_id = GetCellID(cell_x, cell_y);
    objects_by_cell_[cell_id].push_back(particle);
}

std::vector<Particle>& SpatialHash::GetObjectsInCell(int cell_x, int cell_y) {
    int cell_id = GetCellID(cell_x, cell_y);
    return objects_by_cell_[cell_id];
}

std::vector<Particle> SpatialHash::GetNearby(const Particle& particle) const {
    std::vector<Particle> nearby_objects;
    int cell_x = int(particle.position.x / cell_size_);
    int cell_y = int(particle.position.y / cell_size_);
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int cell_id = GetCellID(cell_x +i, cell_y + j);
            auto it = objects_by_cell_.find(cell_id);
            if (it != objects_by_cell_.end()) {
                std::vector<Particle> bucket = objects_by_cell_.at(cell_id);
                nearby_objects.insert(nearby_objects.end(), bucket.begin(), bucket.end());
            }
        }
    }
    return nearby_objects;
}

void SpatialHash::Clear() {
    objects_by_cell_.clear();
}

int SpatialHash::GetCellID(int cell_x, int cell_y) const {
    return cell_x + cell_y * num_cols_;
}