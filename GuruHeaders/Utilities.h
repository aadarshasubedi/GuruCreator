/*
Author:	Dan Strycharske
Project: Inner Knight
Team:	Macrohard
DigiPen Institute of Technology owns this and our souls.

File:	Utilities.h
brief:	A couple of helper functions for converting, creating, and duplicating
		char* into and from wchar*.

*/


#ifndef UTILITIES_H
#define UTILITIES_H

#include <windows.h>
#include <vector>

#ifdef DIRECTX
#include <d3dx9mesh.h>
#endif

const unsigned	MAX_NAME_SIZE		= 32;
const unsigned	MAX_READBUF_SIZE	= 1000;
const float		transitionTime		= 0.25f;
const unsigned	MAX_FILENAME_SIZE	= 50;
const unsigned  MAX_OBJECTS			= 20;
const unsigned  MAX_NUM_BONES		= 100;


class CAnimation;
class vqs;


class CUtilities
{
private:
	CUtilities() {}
	virtual ~CUtilities(void)=0;
public:
	static char* DuplicateCharString(const char* c_str);
	static WCHAR* CreateWideStringFromCharString(const char *c_str);
	static bool FillWideStringFromCharString(const char *c_str,WCHAR *wideString,size_t wideStringLen);
	//replace chars
	static int ReplaceChars(char *str_in, char *str_out, char *replace_chars, char rep_char_with, int case_on);

#ifdef DIRECTX
	static void	FindBoneReference( LPD3DXFRAME pRootFrame, LPD3DXFRAME& pParentFrame, char* pBoneName );

	//static D3DXVECTOR3  StripPositionFromFrame( LPD3DXFRAME pRootFrame );
	static void  ComputeBaseSkeleton( LPD3DXFRAME pRootFrame, LPD3DXFRAME pParentFrame, std::vector<D3DXVECTOR3>& rSkeletonList );
	static void  ComputeVQSSkeleton( LPD3DXFRAME pRootFrame, LPD3DXFRAME pParentFrame, std::vector<D3DXVECTOR3>& rSkeletonList );
#endif
	
	//@todo:  Change these (maybe??) to take in directly the proper data types.
	static void ILerp( vqs&		pKeyframeStart,  vqs&	pKeyframeEnd, vqs&	pKeyframeInterp, unsigned numKeys, float pScaledTime = 0.f );
	static void ISlerp( vqs&	pKeyframeStart,  vqs&	pKeyframeEnd, vqs&	pKeyframeInterp, CAnimation* pBone = NULL, float pScaledTime = 0.f );
	static void IElerp( vqs&	pKeyframeStart,  vqs&	pKeyframeEnd, vqs&	pKeyframeInterp, unsigned numKeys, float pScaledTime = 0.f );
};


#endif

