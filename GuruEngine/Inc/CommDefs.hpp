/////////////////////////////////////////////////////////////////////////////////////////
/*!
	\file CommDefs.hpp
	
	\brief
	Contains all commonly used #define's
	
	\author
	Tim Woodbury
	
	\date
	January 29, 2007
 */
/////////////////////////////////////////////////////////////////////////////////////////
#if !defined( COMMDEFS_HPP_ )
#define COMMDEFS_HPP_

#define abstract =0 //<- to allow "abstract" tag following pure-virtual methods instead
					//	 of that stupid [virtual] [ret] [id] ([args]) = 0; syntax
#define SAFE_DELETE(ptr){ if(ptr) delete(ptr); ptr = NULL; }//automatically ensures you do not try and delete
											//an invalid pointer

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }


#endif // !defined( COMMDEFS_HPP_ )