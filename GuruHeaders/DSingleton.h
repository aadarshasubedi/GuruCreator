/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef DSINGLETON_H_INCLUDED
#define DSINGLETON_H_INCLUDED


#include <iostream>
#include <cassert>


template<typename T>
class DSingleton
{
public:
   static T* ms_singleton;

    DSingleton()
    {
        assert(!ms_singleton);
        //use a cunning trick to get the singleton pointing to the start of
        //the whole, rather than the start of the Singleton part of the object
        //int offset = (int)(T*)1 - (int)(DSingleton <T>*)(T*)1;
        //ms_singleton = (T*)((int)this + offset);

        ms_singleton = static_cast<T*>(this);
    }
    virtual ~DSingleton()
    {
        assert(ms_singleton);
        ms_singleton=0;
    }
    static T& GetSingleton()
    {
        assert(ms_singleton);
        return *ms_singleton;
    }
    static T* GetSingletonPtr()
    {
        assert(ms_singleton);
        return ms_singleton;
    }
};
template <typename T> T* DSingleton <T>::ms_singleton = 0;


#endif // DSINGLETON_H_INCLUDED
