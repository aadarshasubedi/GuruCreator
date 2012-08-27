/************************************************************************
Author:		Dan Strycharske
Date:       02.08.12
File:       GuruAsset.h
Brief:      
/************************************************************************/

#ifndef GURUASSET_H
#define GURUASSET_H


/************************************************************************/
/* Includes
/************************************************************************/
#include <stdlib.h>
#include <vector>
#include "AssetDescriptor.h"

/************************************************************************/
/* Forward Declarations
/************************************************************************/



class Texture
{
public:
	Texture() : mTextureId(0) {}
	Texture(const Texture& tex)
	{
		mTextureId = tex.mTextureId;
	}

	~Texture() {}

	void operator=(const Texture& tex)
	{
		mTextureId = tex.mTextureId;
	}
	unsigned int mTextureId;
};


template <typename T>
class GuruAsset
{
private:
	/************************************************************************/
	/* Private - Member Variables
	/************************************************************************/
	T* mAssetPtr;

	/************************************************************************/
	/* Private - Member methods
	/************************************************************************/

protected:
	/************************************************************************/
	/* Protected - Member Variables
	/************************************************************************/
	AssetType mType;
	/************************************************************************/
	/* Protected - Member methods
	/************************************************************************/

public:
	/************************************************************************
	* Constructors/Destructor
	************************************************************************/
	GuruAsset();
	GuruAsset( const T& object );
	GuruAsset(const GuruAsset<T>& asset);
	~GuruAsset();

	/************************************************************************/
	/* Public - Member methods
	/************************************************************************/
	T* getAsset();

	void SetType(AssetType type) { mType = type; }
	AssetType GetType() const { return mType; }	
	
	/************************************************************************/
	/* Public - Member Variables
	/************************************************************************/
};


template <typename T>
GuruAsset<T>::GuruAsset()
{
	mAssetPtr = new T;
}


template <typename T>
GuruAsset<T>::GuruAsset( const T& object )
{
	mAssetPtr = new T(object);
}


template <typename T>
GuruAsset<T>::GuruAsset(const GuruAsset<T>& asset)
{
	mAssetPtr = new T;
}


template <typename T>
GuruAsset<T>::~GuruAsset() 
{
}


/************************************************************************/
/* Public - Member methods
/************************************************************************/
template <typename T>
T* GuruAsset<T>::getAsset()
{
	return mAssetPtr;
}


#endif
