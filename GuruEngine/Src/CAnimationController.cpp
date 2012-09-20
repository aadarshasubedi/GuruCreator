/*
author:  Dan Strycharske
date:	 05/22/08
file:	 PathingClasses.cpp
brief:	 This file holds the interface declarations for all of my classes involved in generating,
			


*/
#include "CAnimationController.h"


/****************************************

			CAnimationParser

****************************************/
CAnimationParser::CAnimationParser()
{
}

CAnimationParser::CAnimationParser( CAnimationController*	pControlPTR ) : mOwnerController(pControlPTR)
{
	TemplateDefinition	tTemplateDefine( XFILE_ANIMATIONSET, "AnimationSet" );
	mTemplateDefines.push_back( tTemplateDefine );

	tTemplateDefine = TemplateDefinition( XFILE_ANIMATION, "Animation" );	
	mTemplateDefines.push_back( tTemplateDefine );

	tTemplateDefine = TemplateDefinition( XFILE_ANIMATIONKEY, "AnimationKey" );	
	mTemplateDefines.push_back( tTemplateDefine );
}

bool		CAnimationParser::CreateAndOpenFileX( char*	pFilename )
{
	//Open the file using non-deprecated stuff.
	fopen_s(&mFilePTR, pFilename, "r" );

	if( mFilePTR )
	{
		GetSingleLine();

		//Check to make sure it's a good xfile.
		if( (strcmp(mReadBuffer, "xof 0303txt 0032") != 0)   &&
			(strcmp(mReadBuffer, "xof 0303txt 0064") != 0)  )
		{
			return false;
		}
		return true;
	}
	return false;
}


bool		CAnimationParser::ParseFile( char* pFilename )
{
	//status bool
	bool status = false;

	status = CreateAndOpenFileX( pFilename);

	////CHECK STATUS TO MAKE SURE ALL WENT WELL.
	if( status )
	{
		status = WholeFileParse();
	}
	////CHECK STATUS TO MAKE SURE ALL WENT WELL.
	if( !status )
	{
		//Something really bad happened while parsing.
		return status;
	}

	return CloseFile();
}


bool		CAnimationParser::CloseFile()
{
	if( mFilePTR )
	{
		fclose( mFilePTR );
		return true;
	}
	return false;
}

//find open bracket
bool		CAnimationParser::BufferContainsOpenBracket()
{
	if( strlen(mReadBuffer) <= 0 )
		return false;

	char* str;
	str = strstr(mReadBuffer, "{");

	if( str ) return true;

	return false;
}

//find closed bracket
bool		CAnimationParser::BufferContainsClosedBracket()
{	
	if( strlen(mReadBuffer) <= 0 )
		return false;

	char* str;
	str = strstr(mReadBuffer, "}");

	if( str ) return true;

	return false;
}

//get single line
char*		CAnimationParser::GetSingleLine()
{
	
	char *str = NULL;

	// Check that we have a valid file pointer
	if(mFilePTR == NULL)
	{
		return false;
	}

	bool isValidLine = false;
	while(!isValidLine)
	{
		// Read a line in from the file, if it returns NULL, then we've reached
		// the end of the file..hence we finish.
		if ((str = fgets(mReadBuffer,MAX_READBUF_SIZE, mFilePTR))==NULL)
		{
			break;
		}

		int iLength = (int)strlen(str); // Length of our string
		int iIndex  = 0;           // Index into our string we've read in

		// Skip white spaces
		while(isspace(str[iIndex]))
		{
			iIndex++;
		}

		// Check that the line wasn't just loads of white spaces...
		if( (iLength - iIndex) <= 0 )
		{
			continue;
		}

		// Here's something you should note, now I'm going to copy the string
		// back into itself, but without the white spaces... as its back into itself, 
		// you would usually use a temp buffer ... but memmov(..) ensures that we 
		// don't overwrite or old data
		memmove( (void*)mReadBuffer, (void*)&mReadBuffer[iIndex], (iLength-iIndex)+1 );
		// +1 on the end, is for the NULL at the end of the string :)

		str = mReadBuffer;           // Set our temp string pointer back to the beginning 
			                            // of the string.
		iLength = (int)strlen(str); // Update our string length

		// Now when we use gets(..) to read our line in...it includes the "\n"
		// character, which we'll strip off now.
		str[iLength-1] = 0;

		TrimStringLeft(str);
		TrimStringRight(str);
		isValidLine = true;
		
	}// End while loop

	// We reached the end of file, hence we return NULL
	if( str == NULL )
		return (NULL);

	return (str);
}

//trim right
char*		CAnimationParser::TrimStringRight( char* str )
{	
	
	// Strip whitespace from right side, note is destructive

	int iLength = (int)strlen(str);

	if (!str || iLength == 0) return(str);

	// Index to end of string
	int iIndex = iLength - 1; // -1 because of the NULL terminator

	// Strip whitespace by overwriting nulls
	while( isspace(str[iIndex]) && iIndex >= 0)
		str[iIndex--] = 0;

	// String doens't need to be moved, so simply return pointer
	// Well all was nice an easy :)
	return(str);
}

//trim left
char*		CAnimationParser::TrimStringLeft( char* str )
{	
	// Strip whitespace from left side, note is destructive
	int iIndex = 0;

	int iLength = (int)strlen(str);

	if (!str || iLength == 0) return(str);

	// Strip whitespace by advancing pointer
	while(isspace(str[iIndex]) && iIndex < iLength)
		str[iIndex++] = 0; // not needed actually

	// Copy string to left
	memmove((void *)str, (void *)&str[iIndex], (iLength - iIndex)+1);

	// Now return pointer our modified string :)
	return(str);
}

//parse animation set
void		CAnimationParser::ParseAnimationSet()
{
	//mem copy over the amount of AnimationSet, and then replace and trim the brackets out.
	memcpy( mReadBuffer, mReadBuffer + strlen("AnimationSet"), MAX_READBUF_SIZE );
	CUtilities::ReplaceChars( mReadBuffer, mReadBuffer, "{", ' ', 0);
	TrimStringRight(mReadBuffer);
	TrimStringLeft(mReadBuffer);

	mOwnerController->AddAnimationSet( CAnimationSet(mReadBuffer) );
}

//parse animation
void		CAnimationParser::ParseAnimation()
{
	memcpy( mReadBuffer, mReadBuffer + strlen("Animation"), MAX_READBUF_SIZE );
	CUtilities::ReplaceChars( mReadBuffer, mReadBuffer, "{", ' ', 0);
	TrimStringRight(mReadBuffer);
	TrimStringLeft(mReadBuffer);

	mOwnerController->GetCurrentAnimationSet()->AddAnimation( CAnimation(mReadBuffer) );

	//Here I can check for the open and closed brackets, and 
	//it also has the bone name reference for this frame, so grab that too.
	GetSingleLine();
	if( BufferContainsOpenBracket() && BufferContainsClosedBracket() )
	{
		//Well, i can strip those two brackets, trim the sides, and have the name instantly in the buffer.
		CUtilities::ReplaceChars( mReadBuffer, mReadBuffer, "{}", ' ', 0 );
		TrimStringLeft(mReadBuffer);
		TrimStringRight(mReadBuffer);

		mOwnerController->GetCurrentAnimationSet()->GetCurrentAnimation()->SetBoneName( mReadBuffer );
	}
}

//parse key frme
void		CAnimationParser::ParseKeyFrame()
{
	GetSingleLine();
	unsigned tKeyFrameType = 0, tNumDataVals;
	sscanf_s( mReadBuffer, "%d", &tKeyFrameType );

	if( tKeyFrameType == 4 )
	{		
		GetSingleLine();
		unsigned tNumKeyFrames = 0;
		sscanf_s( mReadBuffer, "%d", &tNumKeyFrames );
		mOwnerController->GetCurrentAnimationSet()->GetCurrentAnimation()->SetNumKeyFrames( tNumKeyFrames );

		for( unsigned iterKeyframe = 0; iterKeyframe < tNumKeyFrames; ++iterKeyframe )
			{
			//This should put the file pointer onto the key frame line itself.
			GetSingleLine();

			CUtilities::ReplaceChars( mReadBuffer, mReadBuffer, ";,", ' ', 0);
			TrimStringLeft(mReadBuffer);
			TrimStringRight(mReadBuffer);

			//now do the crazy grabby grab for the values.
			SAnimKeyFrame	tKeyFrame;

#ifdef DIRECTX
			D3DXMATRIX	m;
#else
			Matrix4		m;
#endif
			unsigned tTimestamp = 0;

			//sscanf_s( mReadBuffer, "%d %d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", &tTimestamp, &tNumDataVals,
			//		  &m._11, &m._12, &m._13, &m._14, 
			//		  &m._21, &m._22, &m._23, &m._24, 
			//		  &m._31, &m._32, &m._33, &m._34, 
			//		  &m._41, &m._42, &m._43, &m._44 );

			
			//Here I'm converting the timestamp to seconds, not milliseconds.
			tKeyFrame.mTimeStamp = (DWORD)tTimestamp;

			//This member function rips apart a 4x4 matrix and converts it to a vqs which it sets itself to.
			tKeyFrame.mTransform.setFromMatrix( m );
			tKeyFrame.mMatrixTransform = m;

			//Add this keyframe to it's respective animation and set.
			mOwnerController->GetCurrentAnimationSet()->GetCurrentAnimation()->AddKeyFrame( tKeyFrame );

			if( iterKeyframe == (tNumKeyFrames - 1) )
			{
				mOwnerController->GetCurrentAnimationSet()->SetTotalAnimationTime( tTimestamp );
			}
		}
	}
}

//parse whole file
bool		CAnimationParser::WholeFileParse()
{
	unsigned tCurTypeIndex = 0;
	//get the first line.

	while( GetSingleLine() )
	{		
		//Check to see if it's one of our template types.
		if( FindTemplateType(tCurTypeIndex) && BufferContainsOpenBracket() )
		{
			//run the type through a switch and call the appropriate function.
			switch( tCurTypeIndex )
			{
				case XFILE_ANIMATIONSET:
				{
					ParseAnimationSet();
					break;
				}
				case XFILE_ANIMATION:
				{
					ParseAnimation();
					break;
				}
				case XFILE_ANIMATIONKEY:
				{
					ParseKeyFrame();
					break;
				}
				default:
				{
					break;
				}
			}
		}
		else if( BufferContainsClosedBracket() )
		{
			continue;
		}
	}

	return true;
}

//Find template type
bool		CAnimationParser::FindTemplateType( unsigned& rTemplateType )
{
	if( !BufferContainsOpenBracket() )
	{
		return false;
	}

	char tBuffer[MAX_READBUF_SIZE];

	int iIndex=0;
	while( (mReadBuffer[iIndex] != '\n') && 
		   (mReadBuffer[iIndex] != ' ' ) && 
		   (mReadBuffer[iIndex] != 0   ) )
	{
		tBuffer[iIndex] = mReadBuffer[iIndex];
		iIndex++;
	}
	tBuffer[iIndex] = 0; // Null terminate our new string.


	// szBuffer should contain a copy of the first word on the line
	// So lets check if its in our list

	unsigned templateSize = (unsigned)mTemplateDefines.size();
	for(unsigned i=0; i<templateSize; i++)
	{
		if( strcmp(mTemplateDefines[i].mName, tBuffer) == 0 )
		{
			rTemplateType = i;
			return true;
		}
	}
	return false;
}


/****************************************

			CAnimation

****************************************/
CAnimation::CAnimation() : 
	mKeys(0), mCurrentKeyFrame(0), mNumberOfKeyframes(0), mAngle(0.f), mSinAlpha(0.f),
	mCosAlpha(0.f),	mBeta(0.f),	mCosBeta(0.f), mSinBeta(0.f), mA(0.f)
{
	//RecomputeData( GetKeyframe(0).mTransform, GetKeyframe(1).mTransform );
	SetName( "Default" );
}


CAnimation::CAnimation(char* pName ) :
	mKeys(0), mCurrentKeyFrame(0), mNumberOfKeyframes(0), mAngle(0.f), mSinAlpha(0.f),
	mCosAlpha(0.f),	mBeta(0.f),	mCosBeta(0.f), mSinBeta(0.f), mA(0.f)
{
	//RecomputeData( GetKeyframe(0).mTransform, GetKeyframe(1).mTransform );
	SetName( pName );
}


//Set number of key frames 
void	CAnimation::SetNumKeyFrames( unsigned pNumKeyFrames)
{
	mNumberOfKeyframes = pNumKeyFrames;
}

//get number of keyframes
unsigned	CAnimation::GetNumberOfKeyFrames( )
{
	return mNumberOfKeyframes;
}

//set name.
void	CAnimation::SetName( char*	pName )
{
	memcpy( (void*)mName, (void*)pName, MAX_NAME_SIZE );
}

//set bone name.
void	CAnimation::SetBoneName( char*	pName )
{
	memcpy( (void*)mBoneName, (void*)pName, MAX_NAME_SIZE );
}

//add a keyframe
void	CAnimation::AddKeyFrame( const SAnimKeyFrame&	pKeyFrame )
{
	mKeys.push_back( pKeyFrame );
}

//empty
bool	CAnimation::IsEmpty()
{
	return mKeys.empty();
}


char*			CAnimation::GetBoneName()
{
	return mBoneName;
}


#ifdef DIRECTX
LPD3DXFRAME	CAnimation::GetBoneRef()
{
	return mBoneRef;
}


void			CAnimation::SetBoneRef( LPD3DXFRAME pBoneRef )
{
	mBoneRef = pBoneRef;
}


void			CAnimation::SetKeyFrameBoneReferences( LPD3DXFRAME pBoneRef )
{
	//Make sure it's a valid boneref, since some of them aren't.
	if( pBoneRef )
	{
		//Go through all those keyframes and set their references.
		for( unsigned iter = 0; iter < mNumberOfKeyframes; ++iter )
		{
			mKeys[iter].mBonePTR = pBoneRef;
		}
	}
}
#else

void			CAnimation::SetKeyFrameBoneReferences( GuruFrame* pBoneRef ) {}
void			CAnimation::SetBoneRef( GuruFrame* pBoneRef ) {}
GuruFrame*		CAnimation::GetBoneRef() { return mBoneRef; }
#endif


void		CAnimation::GetKeyframeIndicesByTime( DWORD	pCurrentTime, unsigned& pFirstIndex, unsigned& pSecondIndex )
{
	//Go through the keyframes, compare the time to the next time, and if 
	//	it's less than that, we return the previous time.

	for( unsigned iterKey = 1; iterKey < mNumberOfKeyframes; ++iterKey )
	{
		//Check the actual timestamps and shit.
		if( pCurrentTime <= mKeys[iterKey].mTimeStamp )
		{
			//Grab the index prior and return it.
			pFirstIndex = iterKey-1;
			pSecondIndex = iterKey;
			mCurrentKeyFrame = pFirstIndex % mNumberOfKeyframes;
			return;
		}
	}

	//If it gets here that means something really bad happened, so return just the first bone
	//	and possibly assert.
}

SAnimKeyFrame	CAnimation::GetKeyframe( unsigned index )
{
	
	if( index >= 0 && index < mNumberOfKeyframes )
	{
		return mKeys[index];
	}
	else
	{
		return mKeys[0];
	}
}


void			CAnimation::RecomputeData( const vqs&		pKeyframeStart,  const vqs&	pKeyframeEnd )
{
	DQuaternion	q0 = pKeyframeStart.orientation(), qN = pKeyframeEnd.orientation();
	DQuaternion	qBar = q0, qKminus1 = q0, qK = q0;

	//get the angle increment for these keys.
	mAngle = 1.0f;  //TODO: Fix with Quaternion dot product. q0 * qN;
#ifdef DIRECTX
	mAngle = D3DXQuaternionDot( &q0, &qN );
#endif
	//cos of angle.
	mAngle = acos( mAngle );

	mSinAlpha = sin( mAngle );
	mCosAlpha = cos( mAngle );

	mBeta = mAngle / mNumberOfKeyframes;

	mCosBeta = cos(mBeta);

	//sin of angle.
	mSinBeta = sin(mBeta);

	//2*cos angle
	mA = 2 * mCosBeta;
}

void CAnimation::SetCurrentKeyFrame(unsigned int keyFrameindex)
{ 
	mCurrentKeyFrame = keyFrameindex; 
	RecomputeData( GetKeyframe(keyFrameindex-1).mTransform, GetKeyframe(keyFrameindex).mTransform ); 
}



/****************************************

			CAnimationSet

****************************************/
CAnimationSet::CAnimationSet() : mNumberOfAnimations(0), mCurrentAnimation(0), mCurrentAnimationTime(0)
{
	SetName( "DefaultSetName" );
}

CAnimationSet::CAnimationSet( char* pName ) : mNumberOfAnimations(0), mCurrentAnimation(0), mCurrentAnimationTime(0)
{
	SetName( pName );
}


//Get the current animation.
CAnimation*		CAnimationSet::GetCurrentAnimation()
{
	if( !IsEmpty() && mCurrentAnimation >= 0 && mCurrentAnimation < mAnimations.size() )
	{
		return &mAnimations[mCurrentAnimation];
	}

	return NULL;
}

//Add an animation.
void			CAnimationSet::AddAnimation( const CAnimation&	pAnimation )
{
	if( !IsEmpty() )
	{
		++mCurrentAnimation;
	}

	mAnimations.push_back( pAnimation );
	++mNumberOfAnimations;
	
}

//empty
bool			CAnimationSet::IsEmpty()
{
	return mAnimations.empty();
}


//Set name of animation controller
void			CAnimationSet::SetName( char*	pName )
{
	memcpy( (void*)mName, (void*)pName, MAX_NAME_SIZE );
}


unsigned		CAnimationSet::GetNumberOfAnimations()
{
	return mNumberOfAnimations;
}

CAnimation*		CAnimationSet::GetAnimation( int index)
{
	if( !IsEmpty() && index >= 0 && ((unsigned)index < mNumberOfAnimations ) )
	{
		return &mAnimations[index];
	}
	return NULL;
}


//Get animation time.  
DWORD			CAnimationSet::GetTotalAnimationTime()
{
	return	mTotalAnimationTime;
}

//Set animation time.
void			CAnimationSet::SetTotalAnimationTime( DWORD pTotalTime )
{
	mTotalAnimationTime = pTotalTime;
}

//Make sure the animation time is good.
void				CAnimationSet::UpdateCurrentAnimationTime( DWORD pTimeStep )
{
	//Have a temp hold the future time value.
	unsigned tTimeTemp = mCurrentAnimationTime + pTimeStep;

	//Make sure it's not over the total time, if it is slice it down by the total time.
	while( tTimeTemp > mTotalAnimationTime )
	{
		tTimeTemp -= mTotalAnimationTime;
	}

	//So once this is a good value, set it to the current animation time.
	mCurrentAnimationTime = tTimeTemp;
}




/****************************************

			CAnimationController

****************************************/

CAnimationController::CAnimationController() 
: mAnimationSets(0), mAnimationSpeed(1.0f), mSlowestSpeed(0.1f), mFastestSpeed(3.0f), mCurrentAnimationSet(0), 
mNumberAnimationSets(0), mIncremental(true), mPaused(false)
{
	SetName( "MyController" );
	mParserObject = CAnimationParser(this);
}

//Get the current animation set.
CAnimationSet*	CAnimationController::GetCurrentAnimationSet()
{
	if( !IsEmpty() && mCurrentAnimationSet >= 0 && mCurrentAnimationSet < mAnimationSets.size() )
	{
		return &mAnimationSets[mCurrentAnimationSet];
	}

	return NULL;
}


CAnimationSet*	CAnimationController::GetAnimationSetByIndex( unsigned pSetIndex )
{
	if( !IsEmpty() && pSetIndex >= 0 && pSetIndex < mAnimationSets.size() )
	{
		return &mAnimationSets[pSetIndex];
	}

	return NULL;
}


CAnimationParser*	CAnimationController::GetParser()
{
	if( &mParserObject )
	{
		return &mParserObject;
	}

	return NULL;
}


//Add an animation set.
void			CAnimationController::AddAnimationSet( const CAnimationSet&	pAnimationSet )
{
	if( !IsEmpty() )
	{
		++mCurrentAnimationSet;
	}
	mAnimationSets.push_back( pAnimationSet );
	++mNumberAnimationSets;
}


//Set name of animation controller
void			CAnimationController::SetName( char*	pName )
{
	memcpy( (void*)mName, (void*)pName, MAX_NAME_SIZE );
}


//empty
bool			CAnimationController::IsEmpty()
{
	return mAnimationSets.empty();
}


float			CAnimationController::GetAnimationSpeed()
{
	return mAnimationSpeed;
}


#ifdef DIRECTX
void			CAnimationController::SetBoneReferences( LPD3DXFRAME pRootFrame )
{
	//Looping through all of the animations in each animation set, go through the hierarchy
	// and find the frame with the same name and set the refernce.
	CAnimation* tAnimPTR;

	//LPD3DXFRAME tRoot = pRootFrame;

	for( unsigned setsIter = 0; setsIter < mNumberAnimationSets; ++setsIter )
	{
		unsigned size = GetAnimationSetByIndex(setsIter)->GetNumberOfAnimations();
		for( unsigned animIter = 0; animIter < size; ++animIter )
		{
			//pRootFrame = tRoot;

			//Since the bones are the same for both animation sets, try to set both at same time.
			tAnimPTR = GetAnimationSetByIndex(setsIter)->GetAnimation(animIter);

			//
			LPD3DXFRAME	tBoneRef = NULL;
			char* name = tAnimPTR->GetBoneName();
			CUtilities::FindBoneReference( pRootFrame, tBoneRef, name);

			tAnimPTR->SetBoneRef( tBoneRef  );
			tAnimPTR->SetKeyFrameBoneReferences( tBoneRef );

			tAnimPTR->RecomputeData( tAnimPTR->GetKeyframe(0).mTransform, tAnimPTR->GetKeyframe(1).mTransform );
		}
	}
}
#else
#endif


void				CAnimationController::AdvanceAnimationInTime( DWORD	pTimeStep )
{
	//This function will be the major workhorse of the animation controller,
	//	as this is the one that goes through the hierarchy, finds two keyframes
	//	based on time, and calculates an interpoled frame.  The combined transform
	//	matrix/vqs in the bone will be set to this generated keyframe.
	//After all interpolated keyframes have been generated for all the bones, 
	//	go through the hierarchy again and update all of the toRoot transforms.
	//The toRoots will be shifted by the bone offset matrices, and that will
	//	be our final matrix that will be needed by the compute skeleton function.
	//

	//First thing is to go to current animation set, and using the current animation time
	//	increment it and then update the current frame/bone.
	//STEP 1:
	//	Grab current animation set.
	CAnimationSet*	tpAnimSetRef = GetCurrentAnimationSet();

	pTimeStep /= mAnimationSpeed;

	//Loop through all of the animation structs (bones basically) and for each of them.....
	unsigned tNumBonesInAnimation = tpAnimSetRef->GetNumberOfAnimations();
	unsigned tIndexOne=0, tIndexTwo=0;

	tpAnimSetRef->UpdateCurrentAnimationTime( pTimeStep );

	for( unsigned iterBone = 0; iterBone < tNumBonesInAnimation; ++iterBone )
	{
		//	Call a function from animation set that returns a keyframe's index given a current time.
		tpAnimSetRef->GetAnimation(iterBone)-> 
			GetKeyframeIndicesByTime( tpAnimSetRef->GetCurrentAnimationTime(), tIndexOne, tIndexTwo );

		//	Call the incremental interpolate functions to generate interpolated keyframe.
		GenerateKeyFrame(  tIndexOne, tIndexTwo, tpAnimSetRef->GetCurrentAnimationTime() );
	}

	//END LOOP
}


void				CAnimationController::GenerateKeyFrame( unsigned indexOne, unsigned indexTwo, DWORD	pCurrentTime)
{
	//Grab the two keyframes from the current bone of the current animation set, and call SLerp on it.
	CAnimationSet*	tAnimSet = GetCurrentAnimationSet();

	//Here, loop through all of the bones in the animation.
	unsigned numAnims = tAnimSet->GetNumberOfAnimations();

	for( unsigned iterBone = 0; iterBone < numAnims; ++iterBone )
	{
		//Here, check and see if the second index is still the current key frame.
		//If so, pass the bone onto the interpolation, if not, set it to null.
		CAnimation*	tBone = tAnimSet->GetAnimation( iterBone );
		if( tBone )
		{
			SAnimKeyFrame	keyframeOne;
			SAnimKeyFrame	keyframeTwo;

			if( mIncremental )
			{
				keyframeOne = tBone->GetKeyframe( 0 );
			}
			else
			{
				keyframeOne = tBone->GetKeyframe( indexOne );
			}

			keyframeTwo = tBone->GetKeyframe( indexTwo );

			DWORD	temp = pCurrentTime;
			temp -= keyframeOne.mTimeStamp;

			float	tScaledTime = ( (float)temp / (float)(keyframeTwo.mTimeStamp - keyframeOne.mTimeStamp) );

			Extra_Frame*	tBoneFrame = (Extra_Frame*)tBone->GetBoneRef();

			if( tBoneFrame )
			{				
				if( tBone->GetCurrentKeyFrame() >= indexTwo )
				{
					tBone->SetCurrentKeyFrame( indexTwo );
					CUtilities::ILerp( keyframeOne.mTransform, keyframeTwo.mTransform, tBoneFrame->m_vqsTransform, indexTwo );
					CUtilities::ISlerp( keyframeOne.mTransform, keyframeTwo.mTransform, tBoneFrame->m_vqsTransform, tBone, tScaledTime );
				}
				else
				{
					CUtilities::ILerp( keyframeOne.mTransform, keyframeTwo.mTransform, tBoneFrame->m_vqsTransform, indexTwo);
					CUtilities::ISlerp( keyframeOne.mTransform, keyframeTwo.mTransform, tBoneFrame->m_vqsTransform, tBone, tScaledTime );
				}
			}
		}
	}
}


void				CAnimationController::SetCurrentAnimationSet(unsigned index)
{
	if( index >= 0 && index < mNumberAnimationSets )
	{
		mCurrentAnimationSet = index;
	}
	else
	{
		mCurrentAnimationSet = 0;
	}
}

void				CAnimationController::CycleAnimationSets()
{
	++mCurrentAnimationSet %= mNumberAnimationSets;
}


void				CAnimationController::IncreaseAnimationSpeed()
{
	SetAnimationSpeed( mAnimationSpeed + 0.1f );
}

void				CAnimationController::DecreaseAnimationSpeed()
{
	SetAnimationSpeed( mAnimationSpeed - 0.1f );
}

void				CAnimationController::SlowMotion()
{
	mAnimationSpeed = mSlowestSpeed;
}

void				CAnimationController::DefaultSpeed()
{
	mAnimationSpeed = 1.0f;
}

void				CAnimationController::FastMotion()
{
	mAnimationSpeed = mFastestSpeed;
}

void				CAnimationController::SetAnimationSpeed( float	pAnimSpeed)
{
	mAnimationSpeed = pAnimSpeed;
	if( mAnimationSpeed < mSlowestSpeed )
	{
		mAnimationSpeed = mSlowestSpeed;
	}
	if( mAnimationSpeed > mFastestSpeed )
	{
		mAnimationSpeed = mFastestSpeed;
	}
}



