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
            SpatialHash(int Nbuckets){
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

            void insert(Element& el)
            {
                int key = getKey(el.x,el.y);
                if(fill[key] > buckets[key].size())
                {
                    buckets[key].reserve(buckets[key].size() + 100);
                }

                // el.bucketIndex = fill[key];
                buckets[key][fill[key]] = el;
                fill[key] += 1;
            }

            const std::vector<Element> get(Element& el)
            {
                int key = getKey(el.x,el.y);
                if(fill[key] > buckets[key].size())
                {
                    buckets[key].reserve(buckets[key].size() + 100);
                }

                return buckets[key];
            }

        private:

            int getKey(float x, float y)
            {
               int col = getCell(x);
               int row = getCell(y);
               int key = col + row*columns;
               return key;
            }

            int getCell(float xy)
            {
                return int(xy / step);
            }

            int keys;
            int columns;
            float step;
            std::map<int,std::vector<Element>> buckets;
            std::map<int,int> fill;

    };
}