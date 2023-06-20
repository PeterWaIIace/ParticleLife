#pragma once
#include <iostream>
#include <vector>
#include <map>

namespace experimental
{
    class Element{
        public:
        float x = 0.0;
        float y = 0.0;
        // int bucketIndex = 0;
    };

    class SpatialHash
    {
        public:
            SpatialHash(int Nbuckets);
            void insert(Element& el);
            const std::vector<Element>& get(Element& el);

        private:

            int getKey(float x, float y);
            int getCell(float xy);

            int keys;
            int columns;
            float step;
            std::map<int,std::vector<Element>> buckets;
            std::map<int,int> fill;

    };
}