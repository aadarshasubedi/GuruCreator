
#include "Utilities.h"


/*
Author:	Dan Strycharske
Project: Inner Knight
Team:	Macrohard
DigiPen Institute of Technology owns this and our souls.

File:	Utilities.h
brief:	A couple of helper functions for converting, creating, and duplicating
		char* into and from wchar*.

*/

char* CUtilities::DuplicateCharString(const char* c_str)
{
    if (!c_str)
		return NULL;

	size_t len=strlen(c_str) + 1;
	char *newString = new char[len];
	memcpy(newString, c_str, len*sizeof(char));

	return newString;
}


WCHAR* CUtilities::CreateWideStringFromCharString(const char *c_str)
{
	if (!c_str)
		return NULL;

	size_t len=strlen(c_str)+1;
	WCHAR *wideString=new WCHAR[len];

	if( MultiByteToWideChar( CP_ACP, 0, c_str, -1, wideString, (int)len ) == 0 )
		return NULL;

	return wideString;
}

bool CUtilities::FillWideStringFromCharString(const char *c_str,WCHAR *wideString,size_t wideStringLen)
{
	if (!c_str || !wideString)
		return false;

	if( MultiByteToWideChar( CP_ACP, 0, c_str, -1, wideString, (int)wideStringLen ) == 0 )
		return false;

	return true;
}



int CUtilities::ReplaceChars(char *str_in, char *str_out, char *replace_chars, char rep_char_with, int case_on)
{
	// this function simply replaces the characters from the input string that
	// are listed in replace with the replace char, the results are stored in 
	// string_out, string_in and isn't touched, the number of replacments is 
	// returned. if case_on = 1 then case is checked, other it's case insensitive

	int num_replacements = 0,  // tracks number of characters replaced
		index_in     = 0,      // curr index into input
		index_out    = 0,      // curr index into output
		sindex,                // loop var into strip array
		slength = (int)strlen(replace_chars); // length of strip string

	// do some error checking
	if (!str_in || !str_out || strlen(str_in) == 0)
		return(0);

	// nothing to replace
	if (!replace_chars || strlen(replace_chars)==0)
	{
		strncpy_s(str_out, strlen(str_in), str_in, MAX_READBUF_SIZE);
		return(0);
	} // end if

	// determine if case is important
	if (case_on==1)
	{
		// perform char by char copy
		while(str_in[index_in])
		{
			for (sindex = 0; sindex < slength; sindex++)
				if (str_in[index_in] == replace_chars[sindex])
					{
						// replace it
						str_out[index_out++] = rep_char_with;
						index_in++;
						num_replacements++;
						break;
					} // end if
		     
			// was a replacement performed?, no just copy then
			if (sindex >= slength)
				str_out[index_out++] = str_in[index_in++];

		} // end while
	} // end if case_on
	else
	{
	// perform char by char copy with case insensitivity
	while(str_in[index_in])
		{
		for (sindex = 0; sindex < slength; sindex++)
			if (toupper(str_in[index_in]) == toupper(replace_chars[sindex]))
				{
				// replace it
				str_out[index_out++] = rep_char_with;
				index_in++;
				num_replacements++;
				break;
				} // end if
	     
		// was a strip char found?
		if (sindex >= slength)
			str_out[index_out++] = str_in[index_in++];

		} // end while
	} // end if case_off

	// terminate output string
	str_out[index_out] = 0;

	// return extracts
	return(num_replacements);

} // End ReplaceChars(..)



#ifdef DIRECTX
void	CUtilities::FindBoneReference( LPD3DXFRAME pRootFrame, LPD3DXFRAME& pBoneRef, char* pBoneName )
{
	//Look at the root and see if that's it.....  if both 
	// sibling and child are null, then we're done and we return null.
	if( pRootFrame == NULL )
	{
		return;
	}	

	if(  pRootFrame->Name )
	{
		if( strncmp( pRootFrame->Name, pBoneName, MAX_NAME_SIZE ) == 0)
		{
			//Woot!!  We found the bone.  return it.
			pBoneRef = pRootFrame;
			return;
		}
	}

	//Send it down to the child first.
	if( pRootFrame->pFrameSibling )
	{
		FindBoneReference( pRootFrame->pFrameSibling, pBoneRef, pBoneName );
	}
	if( pRootFrame->pFrameFirstChild )
	{
		FindBoneReference( pRootFrame->pFrameFirstChild, pBoneRef, pBoneName );
	}

}



//D3DXVECTOR3  StripPositionFromFrame( LPD3DXFRAME pRootFrame )
//{
//	Extra_Frame* tExtFrame = (Extra_Frame*)pRootFrame;
//
//	//Grab the combined transform matrix, multiply it along the hierarchy,
//	//	and use that to get it's current position.
//
//}



void  CUtilities::ComputeBaseSkeleton( LPD3DXFRAME pRootFrame, LPD3DXFRAME pParentFrame, std::vector<D3DXVECTOR3>& rSkeletonList )
{
	//This is going to be recursive
	if( pRootFrame == NULL )
	{
		return;
	}

	if( !pRootFrame->pFrameFirstChild && !pRootFrame->pFrameSibling )
	{
		//rSkeletonList.push_back( CUtilities::StripPositionFromFrame(pRootFrame) );
		return;
	}

	//Add this and the parent point to the list
	//rSkeletonList.push_back( CUtilities::StripPositionFromFrame(pRootFrame) );

	if( pParentFrame != NULL )
	{
		Extra_Frame* parent = (Extra_Frame*)pParentFrame;
		Extra_Frame* current = (Extra_Frame*)pRootFrame;

		D3DXMATRIX	mat_parent	= pParentFrame->TransformationMatrix;
		D3DXMatrixMultiply( &pRootFrame->TransformationMatrix, &pRootFrame->TransformationMatrix, &pParentFrame->TransformationMatrix );
		D3DXMATRIX	mat_current = pRootFrame->TransformationMatrix;

		rSkeletonList.push_back( D3DXVECTOR3( mat_parent._41, mat_parent._42, mat_parent._43 ) );
		rSkeletonList.push_back( D3DXVECTOR3( mat_current._41, mat_current._42, mat_current._43 ) );

		parent->m_vqsTransformToRoot.SetVQS( mat_parent );
		current->m_vqsTransformToRoot.SetVQS( mat_current );
	}

	//Now go through the children and siblings....  for siblings, pass the same parent node.

	//For the children, pass this frame as the parent.
	if( pRootFrame->pFrameFirstChild )
	{
		ComputeBaseSkeleton( pRootFrame->pFrameFirstChild, pRootFrame, rSkeletonList );
	}

	if( pRootFrame->pFrameSibling )
	{
		ComputeBaseSkeleton( pRootFrame->pFrameSibling, pParentFrame, rSkeletonList );
	}
}





void  CUtilities::ComputeVQSSkeleton( LPD3DXFRAME pRootFrame, LPD3DXFRAME pParentFrame, std::vector<D3DXVECTOR3>& rSkeletonList )
{
	//This is going to be recursive
	if( pRootFrame == NULL )
	{
		return;
	}

	if( !pRootFrame->pFrameFirstChild && !pRootFrame->pFrameSibling )
	{
		return;
	}


	if( pParentFrame != NULL )
	{
		Extra_Frame* parent = (Extra_Frame*)pParentFrame;
		Extra_Frame* current = (Extra_Frame*)pRootFrame;

		D3DXMATRIX	mat_parent	= pParentFrame->TransformationMatrix;
		D3DXMatrixMultiply( &pRootFrame->TransformationMatrix, &current->m_vqsTransform.toMatrix(), &pParentFrame->TransformationMatrix );
		D3DXMATRIX	mat_current = pRootFrame->TransformationMatrix;

		rSkeletonList.push_back( D3DXVECTOR3( mat_parent._41, mat_parent._42, mat_parent._43 ) );
		rSkeletonList.push_back( D3DXVECTOR3( mat_current._41, mat_current._42, mat_current._43 ) );

		parent->m_vqsTransformToRoot.SetVQS( mat_parent );
		current->m_vqsTransformToRoot.SetVQS( mat_current );
	}

	if( pRootFrame->pFrameFirstChild )
	{
		ComputeVQSSkeleton( pRootFrame->pFrameFirstChild, pRootFrame, rSkeletonList );
	}

	if( pRootFrame->pFrameSibling )
	{
		ComputeVQSSkeleton( pRootFrame->pFrameSibling, pParentFrame, rSkeletonList );
	}
}
#endif
