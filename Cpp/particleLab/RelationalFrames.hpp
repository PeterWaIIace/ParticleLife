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
        Relation newRelation;
        newRelation.p = p;

        this->addRelation(newRelation.p);
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

    std::vector<size_t> getRelationsSize()
    {
        std::vector<size_t> relsSize;
        for(auto& r : rels){
            relsSize.push_back(r.related.size());
        }
        return relsSize;
    };

};


class RelationalFrames{
    private:

    std::vector<RelationalBucket> buckets;

    unsigned int nBuckets = 1;

    int getKey(double x, double y)
    {
        double step = 1.0/double(nBuckets)+0.01; // +0.01 is for making sure that x or y divided by /step will never be equal 1.0
        int key = int(x/step) + (int(y/step) * (nBuckets));
        return key;
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

        this->nBuckets = nBuckets;

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

    size_t size()
    {
        return buckets.size();
    }

    size_t relations()
    {
        size_t relationsSize = 0;
        for(auto& bucket : buckets){
            relationsSize += bucket.getRelations().size();
        };
        return relationsSize;
    }

    std::vector<size_t> relationSizes()
    {
        std::vector<size_t> relationsSize;
        for(auto& bucket : buckets){
            auto& tmp_ = bucket.getRelationsSize();
            relationsSize.insert(relationsSize.end(),tmp_.begin(),tmp_.end());
        };
        return relationsSize;
    }
};