/************************************************************************
Author:		Dan Strycharske
Date:       02.08.12
File:       AssetDescriptor.h
Brief:      
/************************************************************************/

#ifndef ASSETDESCRIPTOR_H
#define ASSETDESCRIPTOR_H


/************************************************************************/
/* Includes
/************************************************************************/

#include <stdlib.h>
#include <string>


enum AssetType
{
	UNDEFINED = 0,
	TEXTURE = 1,
	AUDIO = (1 << 1),
	SHADER = (1 << 2),
	SCRIPT = (1 << 3),
	MODEL = (1 << 4), 
	ANIMATION = (1 << 5)
};

/************************************************************************/
/* Forward Declarations
/************************************************************************/


class AssetDescriptor
{
private:
	/************************************************************************/
	/* Private - Member Variables
	/************************************************************************/

	/************************************************************************/
	/* Private - Member methods
	/************************************************************************/

	/*************************************************************************
	 Raw Asset Interface
	*************************************************************************/

protected:
	/************************************************************************/
	/* Protected - Member Variables
	/************************************************************************/
	
	/************************************************************************/
	/* Protected - Member methods
	/************************************************************************/

public:
	/************************************************************************
	* Constructors/Destructor
	************************************************************************/
	AssetDescriptor(AssetType type = UNDEFINED) : mName(""), mType(type) {}
	AssetDescriptor(std::string filename, AssetType type = UNDEFINED) : mName(filename), mType(type) {}
	virtual ~AssetDescriptor() {}

	std::string mName;
	AssetType mType;

	/************************************************************************/
	/* Public - Member methods
	/************************************************************************/
	inline void SetName(const std::string& name) { mName = name; }
	inline void SetType(AssetType type) { mType = type; }

	inline std::string GetName() const { return mName; }
	inline AssetType GetType() const { return mType; }


	/************************************************************************/
	/* Public - Member Variables
	/************************************************************************/
};


#include "SDL.h"
/************************************************************************/
/* TextureDescriptor
	Really, a texture as far as an asset is concerned is the file to load in,
	which also generate the name (minus file extension.)  The raw asset needs
	more data such as texel format (R8G8B8A8, etc), file extension, flags for 
	compression settings, etc.
/************************************************************************/
class TextureDescriptor : public AssetDescriptor
{
public:
	/************************************************************************/
	/* Private - Member Variables
	/************************************************************************/
	//For the asset, determines filename.  Type is predetermined as TEXTURE.
	//For some systems, the filename with extension might be enough to parse
	// directly.
	std::string mFilename;

	//For the rawAsset, determines how to load the texture into memory.
	SDL_PixelFormat mPixelFormat;	
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mNumColors;
	int mFlags;
	void* mPixels;
	unsigned int mId;

	/************************************************************************/
	/* Private - Member methods
	/************************************************************************/

protected:
	/************************************************************************/
	/* Protected - Member Variables
	/************************************************************************/

	/************************************************************************/
	/* Protected - Member methods
	/************************************************************************/

public:
	/************************************************************************
	* Constructors/Destructor
	************************************************************************/
	TextureDescriptor();
	TextureDescriptor(std::string filename);
	~TextureDescriptor();

	void init(std::string filename = "");

	/************************************************************************/
	/* Public - Member methods
	/************************************************************************/
	
	/************************************************************************/
	/* Public - Member Variables
	/************************************************************************/
};


#endif
