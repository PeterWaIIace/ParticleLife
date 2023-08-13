#pragma once

#include "ParticleSystem.hpp"
#include "DiagnosticLog.hpp"
#include "Particle.hpp"

#include <vector>
#include <set>

typedef struct Relation{
    Particle p;
    std::vector<Particle*> related;
};

class RelationalBucket{

    std::vector<Relation> rels;

    public:
    void insert(Particle &p){
        this->addRelation(p);

        Relation newRelation;
        newRelation.p = p;
        rels.push_back(newRelation);
    }

    void addRelation(Particle &p)
    {
        // TODO: can we change this into tree?
        for(auto& r : rels)
        {
            if(r.p.inRange(p))
            {
                r.related.push_back(&p);
            }
        }
    }

    std::vector<Relation> getRelations()
    {
        return rels;
    };

};


class RelationalFrames{
    private:

    std::vector<RelationalBucket> buckets;

    size_t nBuckets = 1;

    int getKey(double x, double y)
    {
        return int(x) + (int(y) * nBuckets);
    }

    void insertIntoBucket(Particle &p)
    {
        buckets[getKey(p.x,p.y)].insert(p);
        for(auto r : getRelatedBuckets(p))
        {
            r->addRelation(p);
        };
    }

    std::set<RelationalBucket*> getRelatedBuckets(Particle &p)
    {
        std::set<RelationalBucket*> bucketsPtr;
        for(auto coords : p.getMaxCoordinates())
        {
            bucketsPtr.insert(&buckets[getKey(coords.first,coords.second)]);
        }
        bucketsPtr.erase(&buckets[getKey(p.x,p.y)]);
        return bucketsPtr;
    }

    public:

    RelationalFrames(unsigned int nBuckets){

        for(int n = 0 ; n < nBuckets*nBuckets ; n++){
            buckets.push_back(RelationalBucket());
        }

    };

    RelationalFrames(const RelationalFrames& other)
    {
        nBuckets = other.nBuckets;
        buckets  = other.buckets;
    }

    void insert(Particle &p){
        insertIntoBucket(p);
    }

    std::vector<std::pair<Particle, std::vector<Particle*>>> getRelations()
    {
        std::vector<std::pair<Particle, std::vector<Particle*>>> relations;
        for(auto& bucket : buckets){
            for(auto& x : bucket.getRelations())
            {
                relations.push_back(std::make_pair(x.p,x.related));
            }
        };
        return relations;
    }
};