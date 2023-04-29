#include "spatialHashing.hpp"
#include <iostream>

int relationMatrix::getRandomInt(int min, int max)
{
    dist_.param(std::uniform_int_distribution<>::param_type{min, max});
    return dist_(gen_);
}

relationMatrix::relationMatrix() : rd_{}, gen_{rd_()}, dist_(1,100)
{
    for(int i = 0 ; i <MAX_COLORS ; i++)
    {
        for(int j = 0 ; j <MAX_COLORS ; j++)
        {
            arr[i][j] = (float)getRandomInt(-100,100)/100.0;
        }
    }
}

float relationMatrix::getRelation(int firstColor,int secondColor)
{
    return arr[firstColor][secondColor];
}

SpatialHash::SpatialHash(float cell_size) : cell_size_(cell_size) {
    int numberOfBuckets = int((1/cell_size)*(1/cell_size));
    objects_by_cell_.reserve(numberOfBuckets);

}

void SpatialHash::Add(const Particle& particle) {
    int cell_x = int(particle.position.x / cell_size_);
    int cell_y = int(particle.position.y / cell_size_);
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int cell_id = GetCellID(cell_x+i, cell_y+j);
            objects_by_cell_[cell_id].push_back(particle);
        }
    }
}

std::vector<Particle>& SpatialHash::GetObjectsInCell(int cell_x, int cell_y) {
    int cell_id = GetCellID(cell_x, cell_y);
    return objects_by_cell_[cell_id];
}

void SpatialHash::GetNearby(Particle& particle) const {
    std::vector<Particle> nearby_objects;
    // std::array<Particle,5000> nearby_objects;
    int cell_x = int(particle.position.x / cell_size_);
    int cell_y = int(particle.position.y / cell_size_);

    int cell_id = GetCellID(cell_x, cell_y);
    auto it = objects_by_cell_.find(cell_id);
    if (it != objects_by_cell_.end()) {
        for (const Particle& particle2 : it->second) {
            if(particle != particle2)
            {
                float rel = const_cast<relationMatrix&>(relations).getRelation(particle.color,particle2.color);
                particle.addForce(const_cast<Particle&>(particle2).getPosition(),rel);
            }
        }
    }
}

void SpatialHash::Clear() {
    objects_by_cell_.clear();
}

int SpatialHash::GetCellID(int cell_x, int cell_y) const {
    return cell_x + cell_y * num_cols_;
}