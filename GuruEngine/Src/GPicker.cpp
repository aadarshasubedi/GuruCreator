
#include "GPicker.h"
#include "Engine.h"


//////////////////////////////////////////////////////////////////////////
// GPicker Definition
//////////////////////////////////////////////////////////////////////////
GPicker::GPicker()
{
	Init();
}


GPicker::~GPicker()
{

}


//This function will be called after the picker is created.  It will set the mode values, and
//  the initial values for the other members.
void GPicker::Init()
{
	mScreenX = 0;
	mScreenY = 0;

	//The current picker ray in view and world space.
	mRayStart = CVector3();
	mRayDirection = CVector3();

	//Type of mode it's in, if it's in select only or select and transform.
	mPickingMode = 0;

	mCurPickedObject = NULL;
}


//Function taking a screen position, forms a ray, shoots it into view or world coords, and returns
//  the first object it hits.  Returns NULL if no object hit.
Entity*		GPicker::PickSingleObject( const int& pX, const int& pY, const std::vector<Entity*>	SceneObjs )
{
	CVector3 temp;
	Ray ray = ComputePickerData( pX, pY );

	//@TODO: Access the array of scene objects from the scene graph, and see which ones are clicked and stack them.
	unsigned int size = 1; //(unsigned)SceneObjs.size();
	
	for( unsigned sphere_iter = 0; sphere_iter < size; ++sphere_iter )
	{
		////@TODO: Get the Entity's spehre
		////if( SceneObjs[sphere_iter]->GetSphere() )
		//{
		//	//@TODO: 
		//	//if( DetectRaySphere( mRayStart, mRayDirection, SceneObjs[sphere_iter]->GetSphere(), mClicked ) )
		//	{
		//		//@TODO: If this is the one and mode is select only return this object.
		//		//mCurPickedObject = SceneObjs[sphere_iter];
		//		return mCurPickedObject;
		//	}
		//}

		////@TODO: 
		////if( SceneObjs[sphere_iter]->GetBox() )
		//{
		//	std::pair<CVector3,CVector3>	pickedBoxPoints;

		//	//@TODO: collide the ray and aabb from the entity.
		//	//if( DetectRayBox( mRayStart, mRayDirection, SceneObjs[sphere_iter]->GetBox(), pickedBoxPoints ) )
		//	{
		//		mClicked = pickedBoxPoints.first;
		//		//@TODO: 
		//		//mCurPickedObject = SceneObjs[sphere_iter];
		//		return mCurPickedObject;
		//	}
		//}
	}

	//This means that there was no detection with any object, so grab the point on the ground plane
	//@TODO: Project with the z = 0 plane
	//DetectRayGround( CVector3(0, 1, 0), 0, mClicked );

	return NULL;
}


//Function that will set return parameters for the start point and direction for the picking, but doesn't
//	actually cast the ray out.
Ray			GPicker::ComputePickerData( const int& pX, const int& pY )
{
	//Take the two points, and get the camera's matrices, and use the inverses to project back a point to view space.
	
	Camera* camera = Engine::GetSingletonPtr()->Module()->GetCamera();

	float matrix[16] = {0};
	camera->glMakeViewMatrix(matrix);

	Matrix4 view;
	view.setAllElements(matrix);

	Matrix4 inv_view;
	view.getInverse(inv_view);

	Matrix4 inv_ndc, inv_screen;
	
	CVector3	pos( (float)pX, (float)pY, 0.f ); 
	CVector3 cam_wrld = camera->Position();

	CVector3		unprojectedPt;
	
	// Now generate the actual unprojected point.

	mRayStart = unprojectedPt;
	mRayDirection = unprojectedPt - cam_wrld;
	mRayDirection.normalize();

	mRay = Ray(mRayStart, mRayDirection);

	return mRay;
}


CVector3		GPicker::UnprojectPoint( const CVector3&	point )
{
	CVector3 unprojectedPt;
	Matrix4 proj, view;

	Camera* camera = Engine::GetSingletonPtr()->Module()->GetCamera();

	//Get projection matrix from the camera.
	proj = camera->GetProjectionMatrix();

	float matrix[16] = {0};
	camera->glMakeViewMatrix(matrix);

	Matrix4 backToWorld;

	//@TODO: Do the actual unprojection from screen space to world space.

	return unprojectedPt;
}
