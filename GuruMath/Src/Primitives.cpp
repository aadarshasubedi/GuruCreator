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

void printCVector3(const CVector3& vector)
{
	cout << "Vector" << endl;
	cout << "X = " << vector.x << endl;
	cout << "Y = " << vector.y << endl;
	cout << "Z = " << vector.z << endl << endl;
}


void printPoint(const Point& point)
{
	cout << "Point" << endl;
	cout << "X = " << point.x() << endl;
	cout << "Y = " << point.y() << endl;
	cout << "Z = " << point.z() << endl;
}


void printRay(const Ray& ray)
{
	cout << "Ray" << endl;
	printPoint(ray.origin());
	printCVector3(ray.dir());
}


void printLine(const Line& line)
{
	cout << "Line" << endl;
	printPoint(line.A());
	printPoint(line.B());
	cout << endl;
}


void printTriangle(const Triangle& triangle)
{
	cout << "Triangle" << endl;
	printPoint(triangle.A());
	printPoint(triangle.B());
	printPoint(triangle.C());
	printCVector3(triangle.getNormal());
	cout << endl;
}


void printPlane(const Plane& plane)
{
	cout << "Plane" << endl;
	printPoint(plane.A());
	printCVector3(plane.normal());
}

void printSphere(const Sphere& sphere)
{
	cout << "Sphere" << endl;
	cout << "Radius: " << sphere.radius() << endl;
	printPoint(sphere.center());
}

void printQuad(const Quad& quad)
{
	cout << "Quad" << endl;
	printPoint(quad.A());
	printCVector3(quad.u());
	printCVector3(quad.v());
	cout << "U half extent: " << quad.eu() << endl;
	cout << "V half extent: " << quad.ev() << endl;
	printCVector3(quad.normal());
}

void printAABB(const AABB& box)
{
	cout << "AABB" << endl;
	printPoint(box.center());
	printCVector3(box.extents());
}


void testPrimitives()
{
	Point A(1,0,4);
	Point B(2,1,0);
	Point C(0,4,1);
	printPoint(A);

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



float distPointToRay(const Point& point, const Ray& ray)
{
	//Form the triangle of distances.  toPoint and projected on ray.
	CVector3 toPoint = point - ray.origin();
	float toDist = toPoint.magnitude();

	//If it projects behind origin, then it's the dist to origin.
	float proj = CVector3::Dot(toPoint, ray.dir().normal());
	if( proj <= 0.0f ) return toDist;

	//Otherwise find the angle and use some trig.
	float angle = CVector3::AngleDot(toPoint, ray.dir());

	return sin(angle) * toDist;	
}

float projDistPointToRay(const Point& point, const Ray& ray)
{
	//Form the triangle of distances.  toPoint and projected on ray.
	CVector3 toPoint = point - ray.origin();

	return CVector3::Dot(toPoint, ray.dir().normal());
}

Point closestPointOnRayToPoint(const Point& point, const Ray& ray)
{
	float dist = projDistPointToRay(point, ray);
	if( dist < 0.0f )
	{
		//This means the closest point on the ray is the origin, since the
		// point is behind the ray.
		dist = 0.0f;
	}
	return ray.origin() + dist * ray.dir().normal();
}


Point closestPointOnSegmentToPoint(const Point& point, const Line& segment)
{
	//Same test as the ray actually, simply limiting the proj distance to 
	// between 0.0 -> 1.0.  0.0 is A, 1.0 is B on segment, so outside this
	// is not on the segment.
	
	//Convert the line into a ray, then use the rays funcs.
	float dist = projDistSegmentToPoint(point, segment);
	if( dist < 0.0f )
	{
		dist = 0.0f;
	}
	else if( dist > 1.0f )
	{
		dist = 1.0f;
	}
	//use converted segment as ray to easily generate the point.
	return segment.A() + dist * (segment.B() - segment.A()).normal();
}


float projDistSegmentToPoint(const Point& point, const Line& segment)
{
	Ray segAsRay(segment.A(), segment.B() - segment.A());
	return projDistPointToRay(point, segAsRay);
}

float distSegmentToPoint(const Point& point, const Line& segment)
{
	Ray segAsRay(segment.A(), segment.B() - segment.A());
	return distPointToRay(point, segAsRay);
}


Point closestPointOnLineToPoint(const Point& point, const Line& line)
{
	//Line works the same as ray or segment, but easier since it has no bounds.
	float dist = projDistPointToLine(point, line);
	return line.A() + dist * (line.B() - line.A()).normal();
}

float distPointToLine(const Point& point, const Line& line)
{
	//Same as segment
	Ray lineAsRay(line.A(), line.B() - line.A());
	return distPointToRay(point, lineAsRay);
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

float projDistPointToLine(const Point& point, const Line& line)
{
	//Same as segment
	Ray lineAsRay(line.A(), line.B() - line.A());
	return projDistPointToRay(point, lineAsRay);
}


float distPointToPlane(const Point& point, const Plane& plane)
{	
	CVector3 toPoint = point - plane.A();
	return CVector3::Dot(toPoint, plane.normal().normal());	
}

Point closestPointOnPlaneToPoint(const Point& point, const Plane& plane)
{
	float d = distPointToPlane(point,plane);
	return (point - d * plane.normal().normal());
}


///////////////////////////////////
///////Triangle functions//////////
///////////////////////////////////
//Closest point
Point closestPointOnTriangleToPoint(const Point& point, const Triangle& triangle)
{
	CVector3 ab = triangle.B() - triangle.A();
	CVector3 ac = triangle.C() - triangle.A();

	float u,v,w;
	baryCoordsClosestPointOnTriangle(point, triangle, u, v, w);

	return triangle.A() + ab * v + ac * w;
}


//Barycentric coords
void baryCoordsClosestPointOnTriangle(const Point& point, const Triangle& triangle, float& u, float& v, float& w)
{
	CVector3 ab = triangle.B() - triangle.A();
	CVector3 ac = triangle.C() - triangle.A();
	
	//Check if p in vertex region of A
	CVector3 ap = point - triangle.A();
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
	CVector3 bp = point - triangle.B();
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
	CVector3 cp = point - triangle.C();
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

	//P definitely is inside the face region, so now grab them bary coords and get the point.
	float denom = 1.0f / (va + vb + vc);
	float outv = vb * denom;
	float outw = vc * denom;

	u = 1.0f - outv - outw;
	v = outv;
	w = outw;
}


//Distance
float distPointToTriangle(const Point& point, const Triangle& triangle)
{
	//Find closest point on the plane represented by the triangle.
	Plane triAsPlane(triangle.A(), triangle.getNormal());	
	return distPointToPlane(point, triAsPlane);
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
bool containsPointInTriangle(const Point& point, const Triangle& triangle )
{
	//Find closest point on the plane represented by the triangle.
	Plane triAsPlane(triangle.A(), triangle.getNormal());	
	//Do cross products method, it's easy and simple
	float dotAB = CVector3::Dot( CVector3::Cross(triangle.B()-triangle.A(), point - triangle.A()), triAsPlane.normal());
	float dotBC = CVector3::Dot( CVector3::Cross(triangle.C()-triangle.B(), point - triangle.B()), triAsPlane.normal());
	float dotCA = CVector3::Dot( CVector3::Cross(triangle.A()-triangle.C(), point - triangle.C()), triAsPlane.normal());

	return ( dotAB > 0.0f && dotBC > 0.0f && dotCA > 0.0f ) ? true : false;
}


//////////////////////////////////////////////////////////////////////


//This only find nearest point on surface, regardless of if point is inside sphere or not.
Point closestPointOnSphereSurfaceToPoint(const Point& point, const Sphere& sphere)
{
	//Get the direction to go towards the point.
	CVector3 dir = point - sphere.center();

	//The nearest point, even if the original point is inside, is just the normalized
	// direction times the radius, to get nearest on the surface.
	return sphere.center() + sphere.radius() * dir.normal();	
}

//This one generates the closest point to the sphere center, meaning if point is inside
// it will return itself.
Point closestPointOnSphereToPoint(const Point& point, const Sphere& sphere)
{
	float dist = distPointToSphereCenter(point, sphere);
	//if the dist is less than the radius, return point.
	if( dist <= sphere.radius() ) { return point; }

	//Since we know this isn't inside the sphere, the point is going to simply be
	// the nearest surface point.
	return closestPointOnSphereSurfaceToPoint(point, sphere);
}

Point closestPointOnSphereToLine(const Line& line, const Sphere& sphere)
{
	//GEt the distance to the center.  If it's less than radius
	CVector3 v = sphere.center() - line.A();

	float projDist = CVector3::Dot(v, line.getVector().normal());

	Point projPt = line.A() + projDist * line.getVector().normal();

	//projPt will lead to real surface point, or it will be the point.
	return closestPointOnSphereToPoint(projPt, sphere);
}

float distPointToSphereSurface(const Point& point, const Sphere& sphere)
{
	//Get distance from point to center.
	CVector3 toPoint = point - sphere.center();
	float dist = toPoint.magnitude();
	
	//If this is less than radius, do radius - dist.
	if( dist <= sphere.radius() )
	{
		return sphere.radius() - dist;
	}

	//Else, dist - radius
	return dist - sphere.radius();
}

float distPointToSphereCenter(const Point& point, const Sphere& sphere)
{
	//Get distance from point to center.
	return (point - sphere.center()).magnitude();
}


Point closestPointOnAABBToPoint(const Point& point, const AABB& box)
{
	//Setup the output point, it will be updated during the loop.
	float outs[3] = { 0,0,0 };

	//Convert to min-max to make more suitable for the algorithm.
	AABB minMaxRep(box);
	minMaxRep.convertToMinMaxPts();
	
	//Create arrays to make the algorithm easier to work with.
	float pts[3] = { point.x(), point.y(), point.z() };
	float mins[3] = { minMaxRep.center().x(), minMaxRep.center().y(), minMaxRep.center().z() };
	float maxs[3] = { minMaxRep.extents().x, minMaxRep.extents().y, minMaxRep.extents().z };

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

	return Point(outs[0], outs[1], outs[2]);
}

float distPointToAABB(const Point& point, const AABB& box)
{
	//Convert to the min-max rep for box.
	AABB minMaxRep(box);
	minMaxRep.convertToMinMaxPts();

	//Using the slab mechanism, find the dist on each axis, then add them up.
	float sqDist = 0.0f;

	float ptArray[3] = { point.x(), point.y(), point.z() };
	float mins[3] = { minMaxRep.center().x(), minMaxRep.center().y(), minMaxRep.center().z() };
	float maxs[3] = { minMaxRep.extents().x, minMaxRep.extents().y, minMaxRep.extents().z };

	for( unsigned int axis = 0; axis < 3; ++axis )
	{
		float v = ptArray[axis];
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


Point closestPointOnQuadToPoint(const Point& point, const Quad& quad);
float distPointToQuad(const Point& point, const Quad& quad);


Point closestPointOnCylinderToPoint(const Point& point, const Cylinder& cylinder);
float distPointToCylinder(const Point& point, const Cylinder& cylinder);


Point closestPointOnCapsuleToPoint(const Point& point, const Capsule& capsule);
float distPointToCapsule(const Point& point, const Capsule& capsule);

bool intersectRaySphere(const Ray& ray, const Sphere& sphere, CVector3& rIntersectPoint, bool findOutPoint, CVector3* outgoingPoint )
{
	//Get centersphere to ray start, ray is expected to be normalized, but do so just in case.
	Point pt = sphere.center();
	CVector3 sphereCenToStart = pt - ray.origin();
	CVector3 normalRayDir = ray.dir();

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
	CVector3 inPoint = ray.origin() + (normalRayDir * (dist - finalDist));

	//The outgoing point of intersection is calculated here, only if the flag to do so is on.
	if( findOutPoint && outgoingPoint != NULL )
	{
		*outgoingPoint = inPoint + (normalRayDir * (2.0f * finalDist));
	}

	rIntersectPoint = inPoint;
	return true;
}

#endif
