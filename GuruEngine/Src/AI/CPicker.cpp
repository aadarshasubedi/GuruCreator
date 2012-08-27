
/*
Author:	Dan Strycharske
DigiPen Institute of Technology owns this and our souls.

File:	PathingNode.h
brief:	

*/

#include "CPicker.h"
#include "Primitives.h"


CPicker::CPicker()
{
}

CPicker::~CPicker()
{
}

//This function will be called after the picker is created.  It will set the mode values, and
//  the initial values for the other members.
void CPicker::Init()
{
	mScreenX = 0;
	mScreenY = 0;

	//The current picker ray in view and world space.
	mRayStart = D3DXVECTOR3();
	mRayDirection = D3DXVECTOR3();

	//Type of mode it's in, if it's in select only or select and transform.
	mPickingMode = 0;

	mCurPickedObject = NULL;
}

//Function taking a screen position, forms a ray, shoots it into view or world coords, and returns
//  the first object it hits.  Returns NULL if no object hit.
GameObject*		CPicker::PickSceneSingleObject( const int& pX, const int& pY, const std::vector<GameObject*>	SceneObjs )
{
	D3DXVECTOR3 temp;
	ComputePickerData( pX, pY, temp, temp );

	unsigned size = (unsigned)SceneObjs.size();
	for( unsigned sphere_iter = 0; sphere_iter < size; ++sphere_iter )
	{
		if( SceneObjs[sphere_iter]->GetSphere() )
		{
			if( DetectRaySphere( mRayStart, mRayDirection, SceneObjs[sphere_iter]->GetSphere(), mClicked ) )
			{
				mCurPickedObject = SceneObjs[sphere_iter];
				return mCurPickedObject;
			}
		}

		if( SceneObjs[sphere_iter]->GetBox() )
		{
			std::pair<D3DXVECTOR3,D3DXVECTOR3>	pickedBoxPoints;

			if( DetectRayBox( mRayStart, mRayDirection, SceneObjs[sphere_iter]->GetBox(), pickedBoxPoints ) )
			{
				mClicked = pickedBoxPoints.first;
				mCurPickedObject = SceneObjs[sphere_iter];
				return mCurPickedObject;
			}
		}
	}

	//This means that there was no detection with any object, so grab the point on the ground plane
	DetectRayGround( D3DXVECTOR3(0, 1, 0), 0, mClicked );

	return NULL;
}

GameObject*		CPicker::ConnectSceneSingleObject( const int& pX, const int& pY, const std::vector<GameObject*>	SceneObjs )
{
	D3DXVECTOR3 temp;

	if( !mCurPickedObject ) { return NULL; }

	ComputePickerData( pX, pY, temp, temp );

	unsigned size = (unsigned)SceneObjs.size();
	for( unsigned sphere_iter = 0; sphere_iter < size; ++sphere_iter )
	{
		if( SceneObjs[sphere_iter]->GetSphere() )
		{
			if( DetectRaySphere( mRayStart, mRayDirection, SceneObjs[sphere_iter]->GetSphere(), mClicked ) )
			{
				//mCurPickedObject = SceneObjs[sphere_iter];
				return SceneObjs[sphere_iter];
			}
		}
	}

	//This means that there was no detection with any object, so grab the point on the ground plane
	DetectRayGround( D3DXVECTOR3(0, 1, 0), 0, mClicked );

	return NULL;
}


//Function that will set return parameters for the start point and direction for the picking, but doesn't
//	actually cast the ray out.
D3DXVECTOR3			CPicker::ComputePickerData( const int& pX, const int& pY, D3DXVECTOR3& , D3DXVECTOR3&  )
{
	//Take the two points, and get the camera's matrices, and use the inverses to project back a point to view space.

	D3DXMATRIX	inv_view, view;

	//Grab and invert the view matrix.
	CGraphics::GetSingleton().camera->Update();
	CGraphics::GetSingleton().camera->CalculateViewMatrix( &view );
	D3DXMatrixInverse( &inv_view, NULL, &view );

	D3DXMATRIX	inv_ndc, inv_screen;
	D3DXMatrixIdentity( &inv_ndc );
	D3DXMatrixIdentity( &inv_screen );

	///Camera* cam = CGraphics::GetSingleton().camera;

	D3DXVECTOR3	pos( (float)pX, (float)pY, 0.f ), cam_wrld = CGraphics::GetSingleton().camera->GetPosition();

	D3DXVECTOR3		unprojectedPt;

	D3DVIEWPORT9	viewport;
	CGraphics::GetSingleton().device->GetViewport( &viewport );

	D3DXMATRIX		proj;
	proj = CGraphics::GetSingleton().camera->GetProjectionMatrix();

	D3DXMATRIX		world;
	D3DXMatrixIdentity( &world );

	D3DXVec3Unproject( &unprojectedPt, &pos, &viewport, &proj, &view, &world );

	mRayStart = unprojectedPt;
	mRayDirection = unprojectedPt - cam_wrld;
	D3DXVec3Normalize( &mRayDirection, &mRayDirection);

	return mRayStart;
}

bool		CPicker::DetectRaySphere( const D3DXVECTOR3& start, const D3DXVECTOR3& dir, Sphere*	sphere, D3DXVECTOR3&	rSpherePoint )
{
	//Form vector from center of sphere to ray origin.
	//
	//D3DXVECTOR3 orig_cen_vec = sphere.center - ray.origin;
	D3DXVECTOR3		orig_cen_vec = sphere->GetPosition();
	orig_cen_vec -= start;

	////Check to see if ray origin is inside the sphere...  
	//// This way I know I'm only generating one point.
	////
	//float length = orig_cen_vec.length();
	float radius = sphere->GetRadius();

	float length = D3DXVec3Length( &orig_cen_vec );

	//Here, generate the one point easily.
	//float d = length*length +  radius*radius;
	//d = (float)sqrt(d);

	//
	if( length < radius )
	{
		//We started inside, so don't consider this sphere.
		return true;
	}

	else
	{
		//Project this vector onto the ray and get that vector.
		//
		float dot = D3DXVec3Dot( &orig_cen_vec, &dir );
		
		//Check the sign of the dot product with the ray, negative is trivial rejection.
		//
		if( dot < 0.f )
		{
			//Means there are no intersection points.
			return false;
		}
	
		D3DXVECTOR3 projected = dot * dir;

		//Find the difference of these two and form a right triangle
		// inside the circle, with the intersection points being
		// one of the right triangle's points.
		//
		D3DXVECTOR3 diff_vec = orig_cen_vec - projected;
		float d = D3DXVec3Length( &diff_vec );
		if( d > radius )
		{
			//This means the ray shoots past outside the sphere, no intersections.
			return false;
		}

		if( d < radius+EPS && d > radius-EPS )
		{
			//This means ray is tangent to sphere, one intersection point.
			return true;
		}
		
		//Check the magnitude of this new formed vector.  If greater than
		// the sphere's radius, trivial rejection.
		//
		//Use pythagerus to get the amount to add/subtract to get the intersection points.
		//
		float b = (float)sqrt(radius*radius + d*d);

		rSpherePoint = start + (dot-b) * dir;

		//if( rpoints )
		//{
		//	rpoints->first  = start + (dot-b) * dir;
		//	rpoints->second = start + (dot+b) * dir;
		//}
		//return 2;
		return true;	
	}
}


bool		CPicker::DetectRayGround( const D3DXVECTOR3& plane_normal, float dVal, D3DXVECTOR3&	rGroundPoint )
{
	//Use parametric equation of line, and substitute it into the vector 
	// representation of a plane:
	//	n*(P - Q) = 0 and P = M + tV and -Q*n = d
	//
	//	t = -(M*n + d) / V*n
	//  P = M + tV
	//M*n

	float t = ( -D3DXVec3Dot( &plane_normal, &mRayStart ) + dVal);
	t /= D3DXVec3Dot( &mRayDirection, &plane_normal );

	if( t >= 0.f )
	{
		rGroundPoint = mRayStart + t * mRayDirection;
	}

	//Turn the ray into a line, and get the intersection point if any from that.
	//if( IntersectTime( Line3D(ray.origin, ray.direction), TriangleToPlane(tri), t ) )
	//{		
	//	//For a ray, the time of intersection must be greater than zero.
	//	if( t >= 0.f )
	//	{
	//		return true;
	//	}
	//}
	return false;
}


bool		CPicker::DetectRayBox(    const D3DXVECTOR3& start, const D3DXVECTOR3& dir, Box*	box, std::pair<D3DXVECTOR3,D3DXVECTOR3>&	 )
{
	typedef std::pair<float,float> axis_interval;
	//First, calculate the appropriate number of intervals.  Have 
	// a vector of std::pair<float,float>.  Only add intervals that
	// will need to be evaluated.  This means there will be a loop
	// up to 3 that will find the min/max values for the intervals.

	//Loop through all the axes and generate the intervals necessary.
	std::vector< axis_interval > Intervals;

	D3DXVECTOR3	origin = box->GetMinPoint();
	D3DXVECTOR3	extent = box->GetMaxPoint();

	//This loop goes through each coordinate of the ray and determines
	// the ray's intersection interval for that axis.
	for( unsigned i = 0; i < 3; ++i )
	{
		//Determine the t0 and t1 values for this axis.  If the coordinate
		// value of the ray's direction vector is zero, handle that case
		// appropriately here so that the checks at the end will be easier 
		// to accomplish.
		float t0 = 0.f, t1 = 0.f;

		if( dir[i] > -EPS && dir[i] < EPS )
		{
			//Handle this case here.
			//Check to see if the point starts inside the box.  If it does, then
			// don't add this interval (since it's [0,infinity]) but instead
			// continue on with the other intervals.
			//
			//If the point is not inside the box, then we can terminate now.			
			if( origin[i] <= start[i] && start[i] <= extent[i] )
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		else
		{
			//We can do the division by the ray's direction coordinate.
			t0 = origin[i] - start[i];
			t0 /= dir[i];

			//Now get t1.
			t1 = extent[i] - start[i];
			t1 /= dir[i];

			if( t0 < t1 )
			{
				//Add the interval.
				Intervals.push_back( axis_interval(t0,t1) );				
			}
			//If the interval is reversed, this means it's a bad interval.
			// So let the loop fall through and keep going.
		}
	}

	//Now go through the intervals, determining the final interval of intersection.
	//
	unsigned size = (unsigned)Intervals.size();
	float interval_start = 0.f, interval_end = FLT_MAX;
	for( unsigned i = 0; i < size; ++i )
	{
		//Check the max values of the min intervals.
		interval_start = max( interval_start, Intervals[i].first );
		interval_end = min( interval_end, Intervals[i].second );
	}
	
	//One final check to make sure that the interval is not the empty set.
	if( interval_end < interval_start )
	{
		return false;
	}

	//Check the pointer, make sure it's not NULL.  If it isn't, generate the two
	// intersection points.  This should take care if the ray.origin is within
	// the box since start = 0.
	//if( &rBoxPoint )
	//{
	//	rBoxPoint.first = start + interval_start * dir;
	//	rBoxPoint.second = start + interval_end * dir;
	//	return true;
	//}

	return true;


	//throw Unimplemented();

}


D3DXVECTOR3		CPicker::UnprojectPoint( const D3DXVECTOR3&	point )
{
	D3DXVECTOR3		unprojectedPt;

	D3DVIEWPORT9	viewport;
	CGraphics::GetSingleton().device->GetViewport( &viewport );

	D3DXMATRIX		proj, view;
	proj = CGraphics::GetSingleton().camera->GetProjectionMatrix();
	CGraphics::GetSingleton().camera->CalculateViewMatrix( &view );

	D3DXMATRIX		world;
	D3DXMatrixIdentity( &world );

	D3DXVec3Unproject( &unprojectedPt, &point, &viewport, &proj, &view, &world );

	return unprojectedPt;
}

D3DXVECTOR3		CPicker::PickAndCreateWaypoint( int x, int y )
{
	D3DXVECTOR3 temp;
	D3DXVECTOR3	position = ComputePickerData( x, y, temp, temp );

	//This means that there was no detection with any object, so grab the point on the ground plane
	DetectRayGround( D3DXVECTOR3(0, 1, 0), 0, mClicked );
	//mClicked.y += 20.f;

	//PathingArea::GetSingleton().AddNodeToCurrentHierarchy( new PathingNode( mClicked ) );
	return mClicked;
}

void			CPicker::PickAndCreateXObject( int x, int y )
{
	D3DXVECTOR3 temp;
	D3DXVECTOR3	position = ComputePickerData( x, y, temp, temp );

	CGraphics::GetSingleton().CreateXFileObject( (unsigned)CGraphics::GetSingleton().objsList.size(), OBJECT_AIBot, "Enemy", "walk_inPlace.x" );

	unsigned index = (unsigned)CGraphics::GetSingleton().objsList.size();

	CGraphics::GetSingleton().objsList[index-1]->SetPosition( position );	
}

