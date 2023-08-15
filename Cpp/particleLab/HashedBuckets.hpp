#pragma once
#include <vector>
#include <array>
#include <utility>
#include <functional>
#include <chrono>
#include <iostream>
#include <random>
#include <math.h>
#include <mutex>
#include <queue>

#include "Particle.hpp"

using namespace std::chrono;
class Bucket
{
    private:
        std::vector<Particle> frame;
        std::shared_ptr<std::vector<Particle>> framePtr = std::make_shared<std::vector<Particle>>(frame);
    public:
        int row = 0;
        int col = 0;

        Bucket(int row, int col): row(row), col(col) {};

        void insert(Particle& particle)
        {
            framePtr ->push_back(particle);
        };
        std::shared_ptr<std::vector<Particle>> pop(){
            return framePtr ;
        };

        int size(){
            return framePtr->size();
        };
};

class Buckets
{
    public:
        double nHeight = 1.0;
        double nWidth = 1.0;
        double stepHeight = 1.0;
        double stepWidth = 1.0;
        int nBucketsHeight = 1;
        int nBucketsWidth = 1;
        int interatorIndex = 0;

        std::vector<std::vector<Bucket>> b__;
    private:

        Bucket& chooseBucket(double x, double y)
        {
            if(x < 0.0) x = 0.0;
            if(y < 0.0) y = 0.0;
            if(x > 1.0) x = 1.0;
            if(y > 1.0) y = 1.0;

            int row = int(x/stepHeight);
            int col = int(y/stepWidth);

            for(int i = 0 ; i < b__.size() ; i++){
                for(int j = 0 ; j < b__[i].size() ; j++){
                }
            }

            return b__[row][col];
        }

        struct Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = Bucket;
            using pointer           = Bucket*;  // or also value_type*
            using reference         = Bucket&;  // or also value_type&

            Iterator(pointer ptr,Buckets* baseThis) : m_ptr(ptr), m_baseThis(baseThis) {}

            std::pair<pointer,std::vector<Particle>> operator*() const { return std::make_pair(m_ptr,m_baseThis->getSurroundingBuckets()); }
            std::pair<pointer,std::vector<Particle>> operator->(){ return std::make_pair(m_ptr,m_baseThis->getSurroundingBuckets()); }

            // Prefix increment
            Iterator& operator++() {
                m_baseThis->interatorIndex++;
                int row = m_baseThis->interatorIndex/m_baseThis->nBucketsWidth;
                int col = m_baseThis->interatorIndex%m_baseThis->nBucketsWidth;
                m_ptr = &(m_baseThis->b__[row][col]);

                if(m_baseThis->interatorIndex >= m_baseThis->nBucketsWidth*m_baseThis->nBucketsHeight)
                {
                    m_baseThis->interatorIndex = 0;
                }
                return *this;
            }

            // Postfix increment
            Iterator operator++(int) {
                int row = m_baseThis->interatorIndex/m_baseThis->nBucketsWidth;
                int col = m_baseThis->interatorIndex%m_baseThis->nBucketsWidth;
                m_ptr = &(m_baseThis->b__[row][col]);
                m_baseThis->interatorIndex++;
                if(m_baseThis->interatorIndex >= m_baseThis->nBucketsWidth*m_baseThis->nBucketsHeight)
                {
                    m_baseThis->interatorIndex = 0;
                }
                return *this;
            }

            friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

        private:

            pointer m_ptr;
            Buckets* m_baseThis;

        };


    public:
        Buckets(Buckets& other)
        {
            this->b__ = b__;
            this->nWidth = nWidth;
            this->nHeight = nHeight;

        }

        Buckets(int nHeight = 1,int nWidth = 1)
        {
            stepHeight = 1.0/(double)(nHeight);
            stepWidth  = 1.0/(double)(nWidth);
            nBucketsHeight = nHeight;
            nBucketsWidth  = nWidth;

            for(int n = 0 ; n < nBucketsHeight ; n++)
            {
                b__.push_back(std::vector<Bucket>());

                for(int i = 0 ; i < nBucketsWidth ; i++)
                {
                    b__[n].push_back(Bucket(b__[n].size(),b__.size()));
                }
            }

            // adding end empty element
            b__.push_back(std::vector<Bucket>());
            b__[b__.size()-1].push_back(Bucket(b__[b__.size()-1].size(),b__.size()));
        }

        std::vector<int> getBucketsSize(){
            std::vector<int> sizes;
            for(int row = 0; row < nBucketsHeight ; row++){
                for(int col = 0; col < nBucketsWidth ; col++){
                    auto s = b__[row][col].size();
                    sizes.push_back(s);
                }
            }
            return sizes;
        };

        Iterator begin() { return Iterator(&b__[0][0],this); }
        Iterator end()   { return Iterator(&b__[b__.size()-1][b__[b__.size()-1].size()-1],this); } // 200 is out of bounds

        void insert(Particle& particle)
        {
            auto& bucket = chooseBucket(particle.x,particle.y);
            bucket.insert(particle);
        }


        std::shared_ptr<std::vector<Particle>> getBucket(int i , int j)
        {
            return b__[i][j].pop();
        }

        std::vector<Particle> getSurroundingBuckets()
        {
            int row = interatorIndex/nBucketsWidth;
            int col = interatorIndex%nBucketsWidth;
            return getSurroundingBuckets(row,col);
        }

        std::vector<Particle> getSurroundingBuckets(int i , int j)
        {
            std::vector<Particle> neighbors;
            for(int n = -1 ; n <= 1 ; n++)
            {
                int row = i + n;
                for(int m = -1 ; m <= 1 ; m++)
                {
                    int col = j + m;

                    if((row >= 0 && col >= 0) && ( row != i || col != j )  && (row < nBucketsHeight && col < nBucketsWidth) )
                    {
                        neighbors.insert(neighbors.end(),b__[row][col].pop()->begin(),b__[row][col].pop()->end());
                    }
                }
            }
            return neighbors;
        }

        std::vector<Particle> getAllBuckets()
        {
            std::vector<Particle> neighbors;
            for(int col = 0 ; col < nBucketsWidth ; col++)
            {
                for(int row = 0 ; row < nBucketsHeight ; row++)
                {
                    neighbors.insert(neighbors.end(),b__[row][col].pop()->begin(),b__[row][col].pop()->end());
                }
            }
            return neighbors;
        }
};