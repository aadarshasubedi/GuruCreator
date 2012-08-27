/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef PARTICLESORTER_H_INCLUDED
#define PARTICLESORTER_H_INCLUDED


#include <vector>
#include <algorithm>
#include <functional>


class ParticleSorter
{
    public:

        //This class contains a node and a sort function.  Very simple interface.


    //void Sort( const psorter_list& particles, psorter_list out_particles[], unsigned masks[], unsigned num_outs );
};


class ParticleSortNode
{

public:
    ParticleSortNode(void* in_data, float in_value);

    void* data;
    float value;

    inline
    void operator =( const ParticleSortNode& rhs )
    {
        data = rhs.data;
        value = rhs.value;
    }

    bool operator< (const ParticleSortNode& rhs )
    {
        return (value < rhs.value);
    }
};
typedef std::vector<ParticleSortNode> psorter_list;


bool SortParticleDepth (const ParticleSortNode& rhs, const ParticleSortNode& lhs);


#endif // PARTICLESORTER_H_INCLUDED
