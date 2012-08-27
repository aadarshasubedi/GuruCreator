/************************************************************************
Author:		Dan Strycharske
Date:       02.08.12
File:       AssetManager.h
Brief:      
/************************************************************************/

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H


/************************************************************************/
/* Includes
/************************************************************************/
#include <map>
#include <vector>
#include <stdlib.h>
#include <string>
#include "GuruAsset.h"
#include "AssetDescriptor.h"

typedef GuruAsset<Texture> TextureAsset;
typedef TextureAsset* TextureAssetPtr;

/************************************************************************/
/* Forward Declarations
/************************************************************************/


class AssetManager
{
private:
	/************************************************************************/
	/* Private - Member Variables
	/************************************************************************/

	//The map of assets.  The map corresponds to name => asset.  Multiple assets 
	//can have the same name, so this is actually a multimap.
	std::map< std::string, TextureAssetPtr > mTextureAssets;
	//std::multimap< std::string, SoundAsset > mSoundAssets;	

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
	AssetManager();
	~AssetManager();

	/************************************************************************/
	/* Public - Member methods
	/************************************************************************/
	//Create an asset.  This creates the IRawAsset and IAsset, and attaches
	// the raw asset to the IAsset, and then returns a handle to the asset.
	//This takes an asset descriptor which tells the manager which asset type
	// and how to construct it.
	TextureAssetPtr CreateTextureAsset(const TextureDescriptor& descriptor);
	TextureAssetPtr GetTextureAsset(const std::string& assetName);
	
	//Removes an asset with that name.  By default it will remove textures, but can be specified to remove
	// one or more of them.
	void RemoveAsset( const std::string& assetName, AssetType type = UNDEFINED);
	
	/************************************************************************/
	/* Public - Member Variables
	/************************************************************************/
};


/*************************************************************************
 Asset Access
*************************************************************************/
//Grabs an asset given an asset name and returns a Handler to it.



#endif
