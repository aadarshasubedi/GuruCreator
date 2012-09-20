/*
Author:		Dan Strycharske
Date:       01.31.12
File:       Primitives.cpp
Brief:      File containing all the declarations for many of the needed primitive and shape classes for the simulation.
*/
#include "Primitives.h"

#ifdef _DEBUG
#include <assert.h>
#endif


#ifdef _DEBUG
///////////////////////////////////////////////////
using namespace std;

void printCVector3(const CVector3& vec)
{
	cout << "CVector3" << endl;
	cout << "X = " << vec.x << endl;
	cout << "Y = " << vec.y << endl;
	cout << "Z = " << vec.z << endl;
}


void printRay(const Ray& ray)
{
	cout << "Ray" << endl;
	printCVector3(ray.origin());
	printCVector3(ray.dir());
}


void printLine(const Line& line)
{
	cout << "Line" << endl;
	printCVector3(line.A());
	printCVector3(line.B());
	cout << endl;
}


void printTriangle(const Triangle& triangle)
{
	cout << "Triangle" << endl;
	printCVector3(triangle.A());
	printCVector3(triangle.B());
	printCVector3(triangle.C());
	printCVector3(triangle.getNormal());
	cout << endl;
}


void printPlane(const Plane& plane)
{
	cout << "Plane" << endl;
	printCVector3(plane.A());
	printCVector3(plane.normal());
}

void printSphere(const Sphere& sphere)
{
	cout << "Sphere" << endl;
	cout << "Radius: " << sphere.radius() << endl;
	printCVector3(sphere.center());
}

void printQuad(const Quad& quad)
{
	cout << "Quad" << endl;
	printCVector3(quad.A());
	printCVector3(quad.u());
	printCVector3(quad.v());
	cout << "U half extent: " << quad.eu() << endl;
	cout << "V half extent: " << quad.ev() << endl;
	printCVector3(quad.normal());
}

void printAABB(const AABB& box)
{
	cout << "AABB" << endl;
	printCVector3(box.center());
	printCVector3(box.extents());
}


void testPrimitives()
{
	CVector3 A(1,0,4);
	CVector3 B(2,1,0);
	CVector3 C(0,4,1);
	printCVector3(A);

	CVector3 v1 = B - A;
	printCVector3(v1);

	Ray ray(A, v1);
	Ray ray2(B, principleAxes[0]);
	printRay(ray);

	Line line(B,C);
	printLine(line);

	Triangle triangle(A,B,C);
	printTriangle(triangle);
	CVector3 norm = triangle.getNormal();
	printCVector3(norm);

	Plane plane(A, C-B);
	printPlane(plane);

	Sphere sphere(20.0f, B);
	printSphere(sphere);

	Quad quad(A, B, C);
	printQuad(quad);

	AABB box(A, 5, 5, 5);
	printAABB(box);
}
#endif


float distCVector3ToRay(const CVector3& vec, const Ray& ray)
{
	//Form the triangle of distances.  toCVector3 and projected on ray.
	CVector3 toCVector3 = vec - ray.origin();
	float toDist = toCVector3.magnitude();

	//If it projects behind origin, then it's the dist to origin.
	float proj = CVector3::Dot(toCVector3, ray.dir().normal());
	if( proj <= 0.0f ) return toDist;

	//Otherwise find the angle and use some trig.
	float angle = CVector3::AngleDot(toCVector3, ray.dir());

	return sin(angle) * toDist;	
}

float projDistCVector3ToRay(const CVector3& vec, const Ray& ray)
{
	//Form the triangle of distances.  toCVector3 and projected on ray.
	CVector3 toCVector3 = vec - ray.origin();

	return CVector3::Dot(toCVector3, ray.dir().normal());
}

CVector3 closestCVector3OnRayToCVector3(const CVector3& vec, const Ray& ray)
{
	float dist = projDistCVector3ToRay(vec, ray);
	if( dist < 0.0f )
	{
		//This means the closest CVector3 on the ray is the origin, since the
		// CVector3 is behind the ray.
		dist = 0.0f;
	}
	return ray.origin() + dist * ray.dir().normal();
}


CVector3 closestCVector3OnSegmentToCVector3(const CVector3& CVector3, const Line& segment)
{
	//Same test as the ray actually, simply limiting the proj distance to 
	// between 0.0 -> 1.0.  0.0 is A, 1.0 is B on segment, so outside this
	// is not on the segment.
	
	//Convert the line into a ray, then use the rays funcs.
	float dist = projDistSegmentToCVector3(CVector3, segment);
	if( dist < 0.0f )
	{
		dist = 0.0f;
	}
	else if( dist > 1.0f )
	{
		dist = 1.0f;
	}
	//use converted segment as ray to easily generate the CVector3.
	return segment.A() + dist * (segment.B() - segment.A()).normal();
}


float projDistSegmentToCVector3(const CVector3& CVector3, const Line& segment)
{
	Ray segAsRay(segment.A(), segment.B() - segment.A());
	return projDistCVector3ToRay(CVector3, segAsRay);
}

float distSegmentToCVector3(const CVector3& CVector3, const Line& segment)
{
	Ray segAsRay(segment.A(), segment.B() - segment.A());
	return distCVector3ToRay(CVector3, segAsRay);
}


CVector3 closestCVector3OnLineToCVector3(const CVector3& CVector3, const Line& line)
{
	//Line works the same as ray or segment, but easier since it has no bounds.
	float dist = projDistCVector3ToLine(CVector3, line);
	return line.A() + dist * (line.B() - line.A()).normal();
}

float distCVector3ToLine(const CVector3& CVector3, const Line& line)
{
	//Same as segment
	Ray lineAsRay(line.A(), line.B() - line.A());
	return distCVector3ToRay(CVector3, lineAsRay);
}

float distSphereToLine(const Sphere& sphere, const Line& line)
{
	//use center - A = v
	//proj onto line.
	//use ratios to get distance, subtract the radius.
	CVector3 v = sphere.center() - line.A();

	float projDist = CVector3::Dot(v, line.getVector().normal());
	float mag = v.magnitude();

	float dist = mag*mag - projDist*projDist;

	return sqrt(dist);
}

float projDistCVector3ToLine(const CVector3& CVector3, const Line& line)
{
	//Same as segment
	Ray lineAsRay(line.A(), line.B() - line.A());
	return projDistCVector3ToRay(CVector3, lineAsRay);
}


float distCVector3ToPlane(const CVector3& vec, const Plane& plane)
{	
	CVector3 toCVector3 = vec - plane.A();
	return CVector3::Dot(toCVector3, plane.normal().normal());	
}

CVector3 closestCVector3OnPlaneToCVector3(const CVector3& vec, const Plane& plane)
{
	float d = distCVector3ToPlane(vec,plane);
	return (vec - d * plane.normal().normal());
}


///////////////////////////////////
///////Triangle functions//////////
///////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnTriangleToCVector3(const CVector3& vec, const Triangle& triangle)
{
	CVector3 ab = triangle.B() - triangle.A();
	CVector3 ac = triangle.C() - triangle.A();

	float u,v,w;
	baryCoordsClosestCVector3OnTriangle(vec, triangle, u, v, w);

	return triangle.A() + ab * v + ac * w;
}


//Barycentric coords
void baryCoordsClosestCVector3OnTriangle(const CVector3& vec, const Triangle& triangle, float& u, float& v, float& w)
{
	CVector3 ab = triangle.B() - triangle.A();
	CVector3 ac = triangle.C() - triangle.A();
	
	//Check if p in vertex region of A
	CVector3 ap = vec - triangle.A();
	float d1 = CVector3::Dot(ab, ap);
	float d2 = CVector3::Dot(ac, ap);
	if( d1 >= 0.0f && d2 <= 0.0f )
	{
		//This means the coords match up for A, so u = 1.0, v = w = 0.0f
		u = 1.0f;
		v = w = 0.0f;
		return;
	}

	//Check if p in vertex region of B
	CVector3 bp = vec - triangle.B();
	float d3 = CVector3::Dot(ab, bp);
	float d4 = CVector3::Dot(ac, bp);
	if( d3 >= 0.0f && d4 <= d3 )
	{
		//This means the coords match up for B, so u = 1.0, v = w = 0.0f
		v = 1.0f;
		u = w = 0.0f;
		return;
	}

	//Check in edge region of AB
	float vc = d1*d4 - d3*d2;
	if( vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		//bary coords are (1-v, v, 0)
		float outv = d1 / (d1 - d3);
		u = 1.0f - outv;
		v = outv;
		w = 0.0f;
		return;
	}

	//Check if p in vertex region of C
	CVector3 cp = vec - triangle.C();
	float d5 = CVector3::Dot(ab, cp);
	float d6 = CVector3::Dot(ac, cp);
	if( d6 >= 0.0f && d5 <= d6 )
	{
		//This means the coords match up for A, so u = 1.0, v = w = 0.0f
		w = 1.0f;
		u = v = 0.0f;
		return;
	}

	//Check if p in edge region of AC
	float vb = d5*d2 - d1*d6;
	if( vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f )
	{
		//bary coords (0, 1-v, v)
		float outv = d2 / (d2 - d6);
		u = 1.0f - outv;
		v = 0.0f;
		w = outv;
		return;
	}

	//Check if p in edge region of BC
	float va = d3*d6 - d5*d4;
	if( va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f )
	{
		//(0, 1-v, v)
		float outv = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		u = 0.0f;
		v = 1.0f - outv;
		w = outv;
		return;
	}

	//P definitely is inside the face region, so now grab them bary coords and get the CVector3.
	float denom = 1.0f / (va + vb + vc);
	float outv = vb * denom;
	float outw = vc * denom;

	u = 1.0f - outv - outw;
	v = outv;
	w = outw;
}


//Distance
float distCVector3ToTriangle(const CVector3& vec, const Triangle& triangle)
{
	//Find closest CVector3 on the plane represented by the triangle.
	Plane triAsPlane(triangle.A(), triangle.getNormal());	
	return distCVector3ToPlane(vec, triAsPlane);
}

float distRayToTriangle(const Ray& ray, const Triangle& triangle)
{
	//@TODO: implement
	return (&ray && &triangle) ? 0.0f : 1.0f;
}

float distPlaneToTriangle(const Plane& plane, const Triangle& triangle)
{
	//@TODO: implement
	return (&plane && &triangle) ? 0.0f : 1.0f;
}

float distLineToTriangle(const Line& line, const Triangle& triangle)
{
	//@TODO: implement
	return (&line && &triangle) ? 0.0f : 1.0f;
}


float distSphereToTriangle(const Sphere& sphere, const Triangle& triangle)
{
	//@TODO: implement
	return (&sphere && &triangle) ? 0.0f : 1.0f;
}


float distAABBToTriangle(const AABB& aabb, const Triangle& triangle)
{
	//@TODO: implement
	return (&aabb && &triangle) ? 0.0f : 1.0f;
}


float distQuadToTriangle(const Quad& quad, const Triangle& triangle)
{
	//@TODO: implement
	return (&quad && &triangle) ? 0.0f : 1.0f;
}



//Intersection/Containment
bool containsCVector3InTriangle(const CVector3& vec, const Triangle& triangle )
{
	//Find closest CVector3 on the plane represented by the triangle.
	Plane triAsPlane(triangle.A(), triangle.getNormal());	
	//Do cross products method, it's easy and simple
	float dotAB = CVector3::Dot( CVector3::Cross(triangle.B()-triangle.A(), vec - triangle.A()), triAsPlane.normal());
	float dotBC = CVector3::Dot( CVector3::Cross(triangle.C()-triangle.B(), vec - triangle.B()), triAsPlane.normal());
	float dotCA = CVector3::Dot( CVector3::Cross(triangle.A()-triangle.C(), vec - triangle.C()), triAsPlane.normal());

	return ( dotAB > 0.0f && dotBC > 0.0f && dotCA > 0.0f ) ? true : false;
}


//////////////////////////////////////////////////////////////////////


//This only find nearest CVector3 on surface, regardless of if CVector3 is inside sphere or not.
CVector3 closestCVector3OnSphereSurfaceToCVector3(const CVector3& vec, const Sphere& sphere)
{
	//Get the direction to go towards the CVector3.
	CVector3 dir = vec - sphere.center();

	//The nearest CVector3, even if the original CVector3 is inside, is just the normalized
	// direction times the radius, to get nearest on the surface.
	return sphere.center() + sphere.radius() * dir.normal();	
}

//This one generates the closest CVector3 to the sphere center, meaning if CVector3 is inside
// it will return itself.
CVector3 closestCVector3OnSphereToCVector3(const CVector3& vec, const Sphere& sphere)
{
	float dist = distCVector3ToSphereCenter(vec, sphere);
	//if the dist is less than the radius, return CVector3.
	if( dist <= sphere.radius() ) { return vec; }

	//Since we know this isn't inside the sphere, the CVector3 is going to simply be
	// the nearest surface CVector3.
	return closestCVector3OnSphereSurfaceToCVector3(vec, sphere);
}

CVector3 closestCVector3OnSphereToLine(const Line& line, const Sphere& sphere)
{
	//GEt the distance to the center.  If it's less than radius
	CVector3 v = sphere.center() - line.A();

	float projDist = CVector3::Dot(v, line.getVector().normal());

	CVector3 projPt = line.A() + projDist * line.getVector().normal();

	//projPt will lead to real surface CVector3, or it will be the CVector3.
	return closestCVector3OnSphereToCVector3(projPt, sphere);
}

float distCVector3ToSphereSurface(const CVector3& vec, const Sphere& sphere)
{
	//Get distance from CVector3 to center.
	CVector3 toCVector3 = vec - sphere.center();
	float dist = toCVector3.magnitude();
	
	//If this is less than radius, do radius - dist.
	if( dist <= sphere.radius() )
	{
		return sphere.radius() - dist;
	}

	//Else, dist - radius
	return dist - sphere.radius();
}

float distCVector3ToSphereCenter(const CVector3& vec, const Sphere& sphere)
{
	//Get distance from CVector3 to center.
	return (vec - sphere.center()).magnitude();
}


CVector3 closestCVector3OnAABBToCVector3(const CVector3& vec, const AABB& box)
{
	//Setup the output CVector3, it will be updated during the loop.
	float outs[3] = { 0,0,0 };

	//Convert to min-max to make more suitable for the algorithm.
	AABB minMaxRep(box);
	minMaxRep.convertToMinMaxPts();
	
	//Create arrays to make the algorithm easier to work with.
	float pts[3] = { vec.x, vec.y, vec.z };
	float mins[3] = { minMaxRep.mCenter.x, minMaxRep.mCenter.y, minMaxRep.mCenter.z };
	float maxs[3] = { minMaxRep.mHalfExtents.x, minMaxRep.mHalfExtents.y, minMaxRep.mHalfExtents.z };

	for( unsigned int axis = 0; axis < 3; ++axis )
	{
		float v = pts[axis];
		if( v < mins[axis] ) 
		{
			v = mins[axis];
		}
		if( v > maxs[axis] ) 
		{
			v = maxs[axis];
		}
		outs[axis] = v;
	}

	return CVector3(outs[0], outs[1], outs[2]);
}

float distCVector3ToAABB(const CVector3& vec, const AABB& box)
{
	//Convert to the min-max rep for box.
	AABB minMaxRep(box);
	minMaxRep.convertToMinMaxPts();

	//Using the slab mechanism, find the dist on each axis, then add them up.
	float sqDist = 0.0f;
	
	float pts[3] = { vec.x, vec.y, vec.z };
	float mins[3] = { minMaxRep.mCenter.x, minMaxRep.mCenter.y, minMaxRep.mCenter.z };
	float maxs[3] = { minMaxRep.mHalfExtents.x, minMaxRep.mHalfExtents.y, minMaxRep.mHalfExtents.z };

	for( unsigned int axis = 0; axis < 3; ++axis )
	{
		float v = pts[axis];
		if( v < mins[axis] ) 
		{
			sqDist += (mins[axis] - v) * (mins[axis] - v);
		}
		if( v > maxs[axis] ) 
		{
			sqDist += (v - maxs[axis]) * (v - maxs[axis]);
		}
	}
	return sqrt(sqDist);
}


CVector3 closestCVector3OnQuadToCVector3(const CVector3& vec, const Quad& quad);
float distCVector3ToQuad(const CVector3& vec, const Quad& quad);


CVector3 closestCVector3OnCylinderToCVector3(const CVector3& vec, const Cylinder& cylinder);
float distCVector3ToCylinder(const CVector3& vec, const Cylinder& cylinder);


CVector3 closestCVector3OnCapsuleToCVector3(const CVector3& vec, const Capsule& capsule);
float distCVector3ToCapsule(const CVector3& vec, const Capsule& capsule);

bool intersectRaySphere(const Ray& ray, const Sphere& sphere, CVector3& rIntersectCVector3, bool findOutCVector3, CVector3* outgoingCVector3 )
{
	//Get centersphere to ray start, ray is expected to be normalized, but do so just in case.
	CVector3 pt = sphere.center();
	CVector3 sphereCenToStart = pt - ray.origin();
	CVector3 normalRayDir = ray.dir();
	normalRayDir.normalize();

	//Project sphere to start on normal ray.
	float dot = CVector3::Dot(normalRayDir, sphereCenToStart);
	CVector3 distVec = normalRayDir * dot;
	float dist = distVec.magnitude();
	float toSphereDist = sphereCenToStart.magnitude();

	//Early out
	if( dist <= 0.0f )
	{
		return false;
	}

	if( toSphereDist < sphere.radius())
	{
		//We started inside.
	}

	//Get separation distance of these two vectors at sphere center.  If greater than radius, early out.
	float separation = toSphereDist*toSphereDist - dist*dist;
	float realSep = std::sqrt(separation);
	if( realSep > sphere.radius())
	{
		return false;
	}

	//Now use sphere radius to find the distance to sphere surface.
	float finalDist = std::sqrt(sphere.radius() * sphere.radius() - separation);
	CVector3 inCVector3 = ray.origin() + (normalRayDir * (dist - finalDist));

	//The outgoing CVector3 of intersection is calculated here, only if the flag to do so is on.
	if( findOutCVector3 && outgoingCVector3 != NULL )
	{
		*outgoingCVector3 = inCVector3 + (normalRayDir * (2.0f * finalDist));
	}

	rIntersectCVector3 = inCVector3;
	return true;
}

