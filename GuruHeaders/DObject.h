/************************************************************************
Author:		Dan Strycharske
Date:       02.05.12
File:       DObject.h
Brief:      
/************************************************************************/

#ifndef DOBJECT_H_INCLUDED
#define DOBJECT_H_INCLUDED


//#include "CRenderModule.h"
#include <string>


//Object class definiton.  Low level naming, identification, serialzing, and cloning interface.
const unsigned MAX_NAME_SIZE = 64;


class DObject
{
//Base object has identifier, name, controller, and systems for copying it.
public:
	DObject();
	virtual ~DObject( ) {}
	DObject( const DObject& ) {}
		
	const unsigned long int& GetGUID() const { return mGuid; }

protected:
    unsigned long int mGuid;
	//////////////////////////////////////////////////////////////////////

//Copying system.
public:
    DObject* Copy() const;
    static char mCharToAppend;

//Serialization system.
public:
	virtual DObject* Deserialize() {return NULL;}
		
	//Various serialize functions.  Some may write to a file, a stream, a buffer, etc.
	virtual bool Serialize( std::fstream*  ) {return false;}
	virtual bool Serialize( std::iostream* ) {return false;}
	virtual bool Serialize( FILE* ) {return false;}
	virtual bool Serialize( char* , size_t ) {return false;}
};


#endif // DOBJECT_H_INCLUDED
