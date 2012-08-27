/*
author:  Dan Strycharske
date:	 05/22/08
file:	 AnimationController.h
brief:	 

*/

#ifndef CANIMATIONCONTROLLER_H
#define CANIMATIONCONTROLLER_H


#include "Structs.h"
#include "Utilities.h"
#include "CommDefs.hpp"
#include <vector>

#define XFILE_ANIMATIONKEY			2			// AnimationKey
#define XFILE_ANIMATION				1			// Animation
#define XFILE_ANIMATIONSET			0			// AnimationSet



class  CAnimationParser;

struct TemplateDefinition;
struct SAnimKeyFrame;
class  CAnimation;
class  CAnimationSet;
class  CAnimationController;

//This will take two keyframe indices and reference to the keyframe about to be set.
void ILerp(  vqs&	pKeyframeStart,  vqs&	pKeyframeEnd, vqs&	pKeyframeInterp, float pScaledTime );
void ISlerp( vqs&	pKeyframeStart,  vqs&	pKeyframeEnd, vqs&	pKeyframeInterp, float pScaledTime );
void IElerp( vqs&	pKeyframeStart,  vqs&	pKeyframeEnd, vqs&	pKeyframeInterp, float pScaledTime );

//void (*SLerp_funcPTR)( const vqs&, const vqs&, vqs& ) = NULL;


struct TemplateDefinition
{
	TemplateDefinition( unsigned id, char* name) : mID(id), mName(name) {}
	unsigned		mID;
	char*			mName;
};
typedef std::vector<TemplateDefinition>  TEMPDEFINES;


struct SAnimKeyFrame
{

	SAnimKeyFrame() : mBonePTR(NULL), mTimeStamp(0), mTransform(), mMatrixTransform() {}

	//The bone pointer.
#ifdef DIRECTX
	LPD3DXFRAME		mBonePTR;
	//TEMPORARY
	//@todo: remove this when vqs is working.
	D3DXMATRIX		mMatrixTransform;
#else
	GuruFrame*		mBonePTR;
	Matrix4			mMatrixTransform;
#endif

	//Time stamp for this transform
	DWORD			mTimeStamp;

	//The transform data itself, represented by the VQS structure - 
	//  vector, quaternion, scalar.
	vqs				mTransform;

};



class CAnimationParser
{
protected:
	//pointer to the current file being opened.
	FILE*			mFilePTR;

	//the dynamic read buffer for the parsing.
	char			mReadBuffer[MAX_READBUF_SIZE];

	//The definitions for the different template types I'll be loading in.
	//  This will be for the switch statement.
	TEMPDEFINES		mTemplateDefines;

	//A pointer to the owner controller for this parser.
	CAnimationController*	mOwnerController;

public:
	CAnimationParser();
	CAnimationParser( CAnimationController*	pControlPTR );

	//These are the simple functions that get called by whoever wants to parse a file.
	bool		 CreateAndOpenFileX( char*	pFilename );
	bool		 ParseFile( char* pFilename );
	bool		 CloseFile();

protected:
	//find open bracket
	bool			BufferContainsOpenBracket();

	//find closed bracket
	bool			BufferContainsClosedBracket();

	//get single line
	char*			GetSingleLine();

	//trim right
	char*			TrimStringRight( char* str );

	//trim left
	char*			TrimStringLeft( char* str );

	//parse animation set
	void			ParseAnimationSet();

	//parse animation
	void			ParseAnimation();

	//parse key frame
	void			ParseKeyFrame();

	//parse whole file
	bool			WholeFileParse();

	//Find template type
	bool			FindTemplateType( unsigned& rTemplateType );

};


class CAnimation
{
	typedef std::vector<SAnimKeyFrame>	KEYFRAMES;

public:
	CAnimation();
	CAnimation(char* pName );

	//Set number of key frames 
	void			SetNumKeyFrames( unsigned pNumKeyFrames);

	//Get number of key frames
	unsigned		GetNumberOfKeyFrames();

	//set name.
	void			SetName( char*	pName );

	//set bone name
	void			SetBoneName( char*	pName );
	
	//add a keyframe
	void			AddKeyFrame( const SAnimKeyFrame&	pKeyFrame );
	
	//empty
	bool			IsEmpty();

#ifdef DIRECTX
	//
	LPD3DXFRAME		GetBoneRef();

	void			SetBoneRef( LPD3DXFRAME pBoneRef );

	void			SetKeyFrameBoneReferences( LPD3DXFRAME pBoneRef );
#else
	GuruFrame*		GetBoneRef();

	void			SetBoneRef( GuruFrame* pBoneRef );

	void			SetKeyFrameBoneReferences( GuruFrame* pBoneRef );
#endif

	char*			GetBoneName();

	//Gets the first keyframe that this time lies between.  This 
	//	will also update the current animation time and loop it if necessary.
	void			GetKeyframeIndicesByTime( DWORD pCurrentTime, unsigned& pFirstIndex, unsigned& pSecondIndex );

	SAnimKeyFrame	GetKeyframe(	unsigned index );

	unsigned		GetCurrentKeyFrame()		{ return mCurrentKeyFrame; }

	void			SetCurrentKeyFrame( unsigned keyFrameindex );

	void			RecomputeData( const vqs&		pKeyframeStart,  const vqs&	pKeyframeEnd );


	//These are left public since I really want them to be changeable and accessable easily.
	float 			mAngle;
	//cos of angle.

	float 			mSinAlpha;
	float 			mCosAlpha;

	float 			mBeta;

	float 			mCosBeta;

	//sin of angle.
	float 			mSinBeta;

	//2*cos angle
	float 			mA;

private:
	//Needs to hold the keyframes...
	KEYFRAMES		mKeys;

	//the current key frame.
	unsigned		mCurrentKeyFrame;

	//it's name.
	char			mName[MAX_NAME_SIZE];

	char			mBoneName[MAX_NAME_SIZE];

	//number of keyframes
	unsigned		mNumberOfKeyframes;

#ifdef DIRECTX
	//frame reference pointer
	LPD3DXFRAME		mBoneRef;
#else
	GuruFrame*		mBoneRef;
#endif

	//Here I'm going to store the incremental data for the animation for the iSlerp.

};



class CAnimationSet
{
	typedef std::vector<CAnimation>		ANIMATIONS;

public:
	CAnimationSet();
	CAnimationSet(char* pName);

	//Get the current animation.
	CAnimation*		GetCurrentAnimation();

	//Add an animation.
	void			AddAnimation( const CAnimation&	pAnimation );

	//empty
	bool			IsEmpty();

	//set the name
	void			SetName( char* pName );

	//get size
	unsigned		GetNumberOfAnimations();

	//gets an animation pointer within the current set.
	CAnimation*		GetAnimation( int index );

	//Get animation time.  
	DWORD			GetTotalAnimationTime();

	//Set animation time.
	void			SetTotalAnimationTime( DWORD pTotalTime );

	//Grabs the current animation time, so that I can check to loop and stuff.
	DWORD			GetCurrentAnimationTime()	{ return mCurrentAnimationTime; }

	//This will do all of the time updating stuff so that when necessary
	//	all I have to do is call GetCurrentAnimationTime and know it's valid.
	void			UpdateCurrentAnimationTime( DWORD pTimeStep );

private:
	//name
	char			mName[MAX_NAME_SIZE];

	//number of animations
	unsigned		mNumberOfAnimations;

	//current animation
	unsigned		mCurrentAnimation;

	//the list of animations
	ANIMATIONS		mAnimations;

	//Total length of animation.
	DWORD			mTotalAnimationTime;
	
	//This keeps track of the current time in the animation.
	DWORD			mCurrentAnimationTime;
};





class CAnimationController
{
	typedef std::vector<CAnimationSet>	ANIMATIONSETS;

public:
	CAnimationController();

	//Get the current animation set.
	CAnimationSet*		GetCurrentAnimationSet();

	//gets an animation set by index.
	CAnimationSet*		GetAnimationSetByIndex( unsigned pSetIndex );

	//Add an animation set.
	void				AddAnimationSet( const CAnimationSet&	pAnimationSet );

	//Set name of animation controller
	void				SetName( char*	pName );

	//empty
	bool				IsEmpty();

	float				GetAnimationSpeed();

	CAnimationParser*	GetParser();

#ifdef DIRECTX
	//This function takes in a frame pointer, and updates 
	// the animationSets.
	void				SetBoneReferences( LPD3DXFRAME pRootFrame );
#else
	void				SetBoneReferences( GuruFrame* pRootFrame );
#endif
	//This function will be the major workhorse of the animation controller,
	// as this is the one that goes through the hierarchy, finds two keyframes
	// based on time, and calculates an interpoled frame.  The combined transform
	// matrix/vqs in the bone will be set to this generated keyframe.
	//After all interpolated keyframes have been generated for all the bones, 
	// go through the hierarchy again and update all of the toRoot transforms.
	//The toRoots will be shifted by the bone offset matrices, and that will
	// be our final matrix that will be needed by the compute skeleton function.
	void				AdvanceAnimationInTime( DWORD pTimeStep );

	void				GenerateKeyFrame( unsigned indexOne, unsigned indexTwo, DWORD	pCurrentTime );

	bool				IsUsingIncremental()	{ return mIncremental; }
	void				ToggleIncremental()		{ mIncremental = !mIncremental; }

	bool				IsPaused()				{ return mPaused; }
	void				TogglePauseMode()		{ mPaused = !mPaused; }

	void				SetCurrentAnimationSet(unsigned index);

	void				CycleAnimationSets();

	void				IncreaseAnimationSpeed();
	void				DecreaseAnimationSpeed();
	void				SlowMotion();
	void				DefaultSpeed();
	void				FastMotion();

	void				SetAnimationSpeed(float pAnimSpeed);

private:
	//controller name.
	char				mName[MAX_NAME_SIZE];

	//list of animationsets
	ANIMATIONSETS		mAnimationSets;

	//animation speeds
	float				mAnimationSpeed;
	float				mFastestSpeed;
	float				mSlowestSpeed;

	//current animation set
	unsigned			mCurrentAnimationSet;

	//number of animation sets.
	unsigned			mNumberAnimationSets;
	
	//The animation parser for this animation controller.
	CAnimationParser	mParserObject;

	bool				mIncremental;

	bool				mPaused;

};


#endif


