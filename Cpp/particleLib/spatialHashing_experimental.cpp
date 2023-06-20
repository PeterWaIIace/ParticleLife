#include "spatialHashing_experimental.hpp"

namespace experimental
{
    SpatialHash::SpatialHash(int Nbuckets){
        // well as it is 2D array, we need buckets^2
        keys = Nbuckets*Nbuckets;
        step = 1.0/float(Nbuckets);
        columns = Nbuckets;

        for(int key = 0 ; key < keys ; key++)
        {
            Element tmp;
            std::vector<Element> element(10000,tmp);
            buckets[key] = element;
            fill[key] = 0;
        }
    };

    void SpatialHash::insert(Element& el)
    {
        int key = getKey(el.x,el.y);
        if(fill[key] > buckets[key].size())
        {
            buckets[key].reserve(buckets[key].size() + 100);
        }

        // el.bucketIndex = fill[key];
        buckets[key][fill[key]] = el;
        fill[key] += 1;
    };

    const std::vector<Element>& SpatialHash::get(Element& el)
    {
        int key = getKey(el.x,el.y);
        if(fill[key] > buckets[key].size())
        {
            buckets[key].reserve(buckets[key].size() + 100);
        }

        return buckets[key];
    };

    int SpatialHash::getKey(float x, float y)
    {
        int col = getCell(x);
        int row = getCell(y);
        int key = col + row*columns;
        return key;
    };

    int SpatialHash::getCell(float xy)
    {
        return int(xy / step);
    };
}