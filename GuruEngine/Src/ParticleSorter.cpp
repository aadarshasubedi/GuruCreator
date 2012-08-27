/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       ParticleSorter.cpp
Brief:      
/************************************************************************/

#include "ParticleSorter.h"


ParticleSortNode::ParticleSortNode(void* in_data, float in_value)
:   data(in_data), value(in_value)
{
}


bool SortParticleDepth( const ParticleSortNode& lhs, const ParticleSortNode& rhs )
{
    return (lhs.value < rhs.value) ? true : false;
}
