/************************************************************************
Author:		Dan Strycharske
Date:       02.08.12
File:       AssetManager.cpp
Brief:      
/************************************************************************/

#include "AssetManager.h"
#include "Helper.h"
#include <algorithm>


/*************************************************************************
 Constructor
*************************************************************************/
AssetManager::AssetManager() : mTextureAssets()
{
}


/*************************************************************************
 Destructor
*************************************************************************/
AssetManager::~AssetManager()
{
}


/*************************************************************************
 Asset Removal/Deletion
*************************************************************************/
void AssetManager::RemoveAsset(const std::string& assetName, AssetType type)
{
	//This is a map of all the assets with this name.  If type is undefined, we
	// kill them all.  If not, then we find the specific one.  Either way, we need
	// to go through the loop.
	//typedef std::map<std::string,TextureAssetPtr> texMap;
	//typedef std::map<std::string,TextureAssetPtr>::iterator texMapIter;
	//std::pair<texMapIter, texMapIter> deathRow = mTextureAssets.equal_range(assetName);

	//TextureAssetPtr deathRow = std::for_each( begin(mTextureAssets), end(mTextureAssets), [&]( std::pair<std::string, TextureAssetPtr> asset)
	//{		
	//	if( asset.second->GetType() == type || type == UNDEFINED )
	//	{			
	//		return asset.second;
	//	}
	//});
	
}


TextureAssetPtr AssetManager::CreateTextureAsset(const TextureDescriptor& descriptor)
{
	//This is the OpenGL part that loads the texture in and returns the raw asset.
	//The RawAsset will be a RawTexture.  loadTexture assumes that getRawTextureData has
	// been called on this texture descriptor.
	GLuint textureId;
	
	loadTextureFromDescriptor(textureId, descriptor);
	
	//@TODO: Implement the setter and raw texture asset.  raw tex asset is just the data
	//       gotten from the getRawTextureData, so make members/setters/getters and that's
	//		 pretty much it.
	//		 
	auto texture = new TextureAsset;
	
	//Set the texture id as this is really what a texture is as far as the high level application layer
	// is concerned.  Use 
	texture->getAsset()->mTextureId = textureId;
	
	mTextureAssets.insert( std::make_pair(descriptor.mName, texture) );	
	return texture;
}


TextureAssetPtr AssetManager::GetTextureAsset(const std::string& assetName)
{
	auto asset = mTextureAssets.find(assetName);

	return asset->second;
}
