/************************************************************************
Author:		Dan Strycharske
Date:       02.05.12
File:       DObject.cpp
Brief:      
/************************************************************************/

#include "DObject.h"


//Base object has identifier, name, and systems for copying it.
//@todo:  Add streaming, smart pointer, and run time type systems.

static unsigned long globalid = 0;

DObject::DObject( ) : mGuid( ++globalid )
{
}


//Copying system.
DObject* DObject::Copy() const
{
    return NULL;
}
