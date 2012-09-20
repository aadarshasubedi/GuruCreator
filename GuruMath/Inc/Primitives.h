/*
Author:		Dan Strycharske
Date:       01.31.12
File:       Primitives.h
Brief:      File containing all the declarations for many of the needed primitive and shape classes for the simulation.
*/

#ifndef GURU_PRIMITIVES_H
#define GURU_PRIMITIVES_H


#include "Vector.h"
#include "Utility.h"


class CVector3;
class Ray;
class Line;
class Plane;
class Triangle;
class Quad;
class Sphere;
class AABB;
//class OBB;
class Capsule;
class Cylinder;


#ifdef _DEBUG
#include <iostream>

void printCVector3(const CVector3& vector);
void printCVector3(const CVector3& CVector3);
void printRay(const Ray& ray);
void printLine(const Line& line);
void printTriangle(const Triangle& triangle);
void printPlane(const Plane& plane);
void printSphere(const Sphere& sphere);
void printQuad(const Quad& quad);
void printAABB(const AABB& box);

void testPrimitives();
#endif

//
////There are many primitive types that need to be represented in the simulation.  From here on out, when I say 'Physics' I mean anything that is simulated
//// and requires the system to handle and control interactions and their reactions.
//
//class CVector3
//{
//public:
//	CVector3(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
//	CVector3(const CVector3& pt);
//	CVector3(const CVector3& v);
//	CVector3(CVector3 v);
//	~CVector3();
//
//	//Setters/Getters.
//	inline const float& x() const { return mX; }
//	inline float& x() { return mX; }
//	inline void x(const float& value) { mX = value; }	
//	inline void setX(const float& value) { mX = value; }
//
//	inline const float& y() const { return mY; }
//	inline float& y() { return mY; }
//	inline void y(const float& value) { mY = value; }
//	inline void setY(const float& value) { mY = value; }
//
//	inline const float& z() const { return mZ; }
//	inline float& z() { return mZ; }
//	inline void z(const float& value) { mZ = value; }
//	inline void setZ(const float& value) { mZ = value; }
//
//	//This function takes a second CVector3 and forms a ray from the two CVector3s.
//	Ray makeRay(const CVector3& B);
//
//	////Member operators.
//	operator CVector3() { return CVector3(mX, mY, mZ); }
//
//private:
//	//CVector3 is simple x,y,z, like vector but w/o many of the properties.
//	float mX, mY, mZ;
//};
//CVector3 operator+(const CVector3& pt, const CVector3& vec);
//CVector3 operator-(const CVector3& pt, const CVector3& vec);
//CVector3 operator-(const CVector3& A, const CVector3& B);
//bool operator==(const CVector3& A, const CVector3& B);
//

/////////////////////////////////////////////////////////


class Ray
{
public:
	Ray();
	Ray(const CVector3& pt, const CVector3& vec );
	~Ray();

	//Usual getters/setters.
	inline const CVector3& origin() const { return mOrigin; }
	inline CVector3& origin() { return mOrigin; }
	inline void origin(const CVector3& pt) { mOrigin = pt; }
	inline void setOrigin(const CVector3& pt) { mOrigin = pt; }

	inline const CVector3& dir() const { return mDir; }
	inline void setDirection(const CVector3& vec) { mDir = vec; }

	//English friendly versions.
	bool pointIsOrigin(const CVector3& vec);
	bool pointBehindOrigin(const CVector3& vec);
	bool pointInFrontOfOrigin(const CVector3& vec);
	float pointProjectedDistance(const CVector3& vec);
	bool vectorParallel(const Vector3& vec);
	bool vectorPerpendicular(const Vector3& vec);
	bool rayIsSame(const Ray& ray);

	//Uses the direction vector to generate another CVector3 at origin + mDir
	Line makeLine();

private:
	float projectPoint(const CVector3& vec);
	bool determineBehind(const CVector3& vec);

private:
	//Defined by a CVector3 and vector.  The vector is infinite in that direction.
	CVector3 mOrigin;
	CVector3 mDir;
};


/////////////////////////////////////////////////////////


//Doubles also as the LineSegment.  The utility functions are named appropriately to distinguish them, 
// but in implementation they are defined by the same data, that data is simply used differently for various
// things.
class Line
{
public:
	Line() : mA(), mB() {}
	Line(const CVector3& A, const CVector3& B) : mA(A), mB(B) {}

	CVector3 getVector() const { return mB - mA; }

	//Getters and setters.
	inline const CVector3& A() const { return mA; }
	inline CVector3& A() { return mA; }
	inline void A(const CVector3& pt) { mA = pt; }

	inline const CVector3& B() const { return mB; }
	inline CVector3& B() { return mB; }
	inline void B(const CVector3& pt) { mB = pt; }

	//dir = B - A, and A is origin.
	Ray makeRay();

private:
	//The two CVector3s that define a Line.  The line is infinite in both directions.
	CVector3 mA, mB;
};


/////////////////////////////////////////////////////////


class Triangle
{
public:
	Triangle();
	Triangle(const CVector3& a, const CVector3& b, const CVector3& c);
	~Triangle();

	//Getters and setters.
	//Getters and setters.
	inline const CVector3& A() const { return mA; }
	inline CVector3& A() { return mA; }
	inline void A(const CVector3& pt) { mA = pt; }
	inline void setA(const CVector3& pt) { mA = pt; }

	inline const CVector3& B() const { return mB; }
	inline CVector3& B() { return mB; }
	inline void B(const CVector3& pt) { mB = pt; }
	inline void setB(const CVector3& pt) { mB = pt; }

	inline const CVector3& C() const { return mC; }
	inline CVector3& C() { return mC; }
	inline void C(const CVector3& pt) { mC = pt; }
	inline void setC(const CVector3& pt) { mC = pt; }

	//Retrieves the normal for the triangle.  Does not store it.  Normalized.
	CVector3 getNormal() const;

	//This function takes 3 Line objects that it will fill in with the appropriate CVector3s.
	void formLines(Line& one, Line& two, Line& three);

	//Get a plane from the triangle.  Use A as the CVector3 on the plane.
	Plane getPlane();

private:
	//Triangle is 3 CVector3s.  Additionally, the lines can be formed from these three CVector3s as helper functions.	
	//@NOTE: Triangles ASSUME COUNTER CLOCKWISE ORDERING!!
	CVector3 mA, mB, mC;
};


/////////////////////////////////////////////////////////


class Plane
{
public:
	////Constructors/Destructor
	Plane();
	Plane(const CVector3& a, const CVector3& norm);
	virtual ~Plane();

	////Getters and Setters
	inline const CVector3& A() const { return mA; }
	inline CVector3& A() { return mA; }
	inline void A(const CVector3& pt) { mA = pt; }
	inline void setA(const CVector3& pt) { mA = pt; }

	inline const CVector3& normal() const { return mNormal; }
	inline CVector3& normal() { return mNormal; }
	inline void normal(const CVector3& vec) { mNormal = vec; }
	inline void setNormal(const CVector3& vec) { mNormal = vec; }

private:
	int determinePointOrientation(const CVector3& pt); //This function return -1 for below, 0 for on, 1 for above plane.
	float projectPoint(const CVector3& pt); //projects the vector pt - A onto the normal.

protected:
	CVector3 mA;
	CVector3 mNormal;
};


class Quad : public Plane
{
public:
	Quad();
	Quad(const CVector3& center, const CVector3& u, const CVector3& v, bool pointRepresentation = false);
	~Quad();

	//Getters and Setters
	inline const CVector3& u() const { return mU; }
	inline CVector3& u() { return mU; }
	inline void u(const CVector3& vec) { mU = vec; }
	inline void setU(const CVector3& vec) { mU = vec; }

	inline const CVector3& v() const { return mV; }
	inline CVector3& v() { return mV; }
	inline void v(const CVector3& vec) { mV = vec; }
	inline void setV(const CVector3& vec) { mV = vec; }

	inline const float& eu() const { return mEu; }
	inline float& eu() { return mEu; }
	inline void eu(const float& u) { mEu = u; }
	inline void setEu(const float& u) { mEu = u; }

	inline const float& ev() const { return mEv; }
	inline float& ev() { return mEv; }
	inline void ev(const float& v) { mEv = v; }
	inline void setEv(const float& v) { mEv = v; }

private:
	//Need only define one more CVector3 from a plane.  This assumes the CVector3
	// coming from the Plane is used as either the min or maxpt.  By default it is
	// the min CVector3.
	CVector3 mU, mV;
	float mEu, mEv;	
};


class Sphere
{
public:
	Sphere();
	Sphere(const float& radius, const CVector3& center);
	~Sphere();

	//Getters and Setters
	inline const CVector3& center() const { return mCenter; }
	inline CVector3 center() { return mCenter; }
	inline void center(const CVector3& pt) { mCenter = pt; }
	inline void setCenter(const CVector3& pt) { mCenter = pt; }

	inline const float& radius() const { return mRadius; }
	inline float& radius() { return mRadius; }
	inline void radius(const float& r) { mRadius = r; }
	inline void setRadius(const float& r) { mRadius = r; }

	//English friendly funcs.
	bool pointOnSurface(const CVector3& CVector3);
	bool pointOutside(const CVector3& CVector3);
	bool pointInside(const CVector3& CVector3);
	float pointDistanceToSurface(const CVector3& CVector3);
	
	bool lineOnSurface(const Line& line);
	bool lineFullyOutside(const Line& line);
	bool lineFullyInside(const Line& line);
	float lineDistanceToSurface(const Line& line);

	bool triangleOnSurface(const Triangle& triangle);
	bool triangleFullyOutside(const Triangle& triangle);
	bool triangleFullyInside(const Triangle& triangle);
	float triangleDistanceToSurface(const Triangle& triangle);

private:
	//Radius and center position.
	CVector3 mCenter;
	float mRadius;
};


//The AABB has many definitions, but this uses the lowest memory footprint method
class AABB
{
public:
	////Constructors/Destructor
	AABB();
	AABB(const CVector3& center, float h_width, float h_height, float h_depth);
	AABB(const CVector3& center, const CVector3& h_extents, bool minMaxRep = false);
	AABB(const CVector3& center, float* h_extents); //assumes that h_extents is at least 3 elements, will only use the first 3.
	~AABB();

	////Getter and Setters
	inline const CVector3& center() const { return mCenter; }
	inline CVector3& center() { return mCenter; }
	inline void center(const CVector3& pt) { mCenter = pt; }
	inline void setCenter(const CVector3& pt) { mCenter = pt; }

	inline const CVector3& extents() const { return mHalfExtents; }
	inline CVector3& extents() { return mHalfExtents; }
	inline void extents(const CVector3& vec) { mHalfExtents = vec; }
	inline void setExtents(const CVector3& vec) { mHalfExtents = vec; }
	
	inline const float& width() const { return mHalfExtents.x; }
	inline float& width() { return mHalfExtents.x; }
	inline void width(const float& width) { mHalfExtents.x = width; }
	inline void setWidth(const float& width) { mHalfExtents.x = width; }

	inline const float& height() const { return mHalfExtents.y; }
	inline float& height() { return mHalfExtents.y; }
	inline void height(const float& height) { mHalfExtents.y = height; }
	inline void setHeight(const float& height) { mHalfExtents.y = height; }

	inline const float& depth() const { return mHalfExtents.z; }
	inline float& depth() { return mHalfExtents.z; }
	inline void depth(const float& depth) { mHalfExtents.z = depth; }
	inline void setDepth(const float& depth) { mHalfExtents.z = depth; }

	////Converters
	//Using the data to the class, all the various types can be represented, but
	// it would be up to the user to use them carefully in any other than the default
	// form.  Also, anything within the Guru system itself will assume these are default
	// formed, that is center + half extents representation.  Any other representation
	// is to be used in Application level code only, and there will be issues if something
	// user fed into the engine isn't formed properly.
	AABB convertToFullExtents();
	AABB convertToMinMaxPts();
	//The slab is used in the engine during detection, but it does not alter or create a new
	// AABB so it is safe to use anywhere.
	void convertToSlab(float* minVals, float* maxVals);  //assumes allocated to 6 elements.

private:
	void AABB::convertToSlab(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax);

public:
	CVector3 mCenter;
	CVector3 mHalfExtents; //3 floats wrapped in a vector.  order: w, h, d (x,y,z)
};



//Intersection, smallest distance, closest CVector3 and other primitive-primitive calculations.

//////////////////////////////////////////////////////////////////////
///////Ray functions//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnRayToCVector3(const CVector3& CVector3, const Ray& ray);
CVector3 closestCVector3OnRayToRay(const Ray& ray, const Ray& A);
//Distance
float distCVector3ToRay(const CVector3& CVector3, const Ray& ray);
float projDistCVector3ToRay(const CVector3& CVector3, const Ray& ray);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Segment functions//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnSegmentToCVector3(const CVector3& CVector3, const Line& segment);
CVector3 closestCVector3OnSegmentToSegment(const Line& segment, const Line& A);
CVector3 closestCVector3OnSegmentToRay(const Line& segment, const Ray& ray);
CVector3 closestCVector3OnSegmentToLine(const Line& segment, const Line& line);
CVector3 closestCVector3OnSegmentToPlane(const Line& segment, const Plane& plane);
//Bary coords
//Distance
float distSegmentToCVector3(const CVector3& CVector3, const Line& segment);
float projDistSegmentToCVector3(const CVector3& CVector3, const Line& segment);
float distSegmentToSegment(const Line& segment, const Line& A);
float distSegmentToRay(const Line& segment, const Ray& ray);
float distSegmentToLine(const Line& segment, const Line& line);
float distSegmentToPlane(const Line& segment, const Plane& plane);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Line functions/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnLineToCVector3(const CVector3& CVector3, const Line& line);
//Distance
float distCVector3ToLine(const CVector3& CVector3, const Line& line);
float distSphereToLine(const Sphere& sphere, const Line& line);
float projDistCVector3ToLine(const CVector3& CVector3, const Line& line);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Plane functions////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnPlaneToCVector3(const CVector3& CVector3, const Plane& plane);
//Distance
float distCVector3ToPlane(const CVector3& CVector3, const Plane& plane);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Triangle functions/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnTriangleToCVector3(const CVector3& CVector3, const Triangle& triangle);
//Barycentric coords
void baryCoordsClosestCVector3OnTriangle(const CVector3& CVector3, const Triangle& triangle, float& u, float& v, float& w);
//Distance
float distRayToTriangle(const Ray& ray, const Triangle& triangle);
float distCVector3ToTriangle(const CVector3& CVector3, const Triangle& triangle);
float distPlaneToTriangle(const Plane& plane, const Triangle& triangle);
float distLineToTriangle(const Line& line, const Triangle& triangle);
float distSphereToTriangle(const Sphere& sphere, const Triangle& triangle);
float distAABBToTriangle(const AABB& aabb, const Triangle& triangle);
float distQuadToTriangle(const Quad& quad, const Triangle& triangle);
//Intersection/Containment
bool containsCVector3InTriangle(const CVector3& CVector3, const Triangle& triangle );
bool rayIntersectsTriangle(const Ray& ray, const Triangle& triangle);
bool lineIntersectsTriangle(const Line& line, const Triangle& triangle);
bool segmentIntersectsTriangle(const Line& segment, const Triangle& triangle);
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Sphere functions///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnSphereSurfaceToCVector3(const CVector3& CVector3, const Sphere& sphere);
CVector3 closestCVector3OnSphereToCVector3(const CVector3& CVector3, const Sphere& sphere);
CVector3 closestCVector3OnSphereToLine(const Line& line, const Sphere& sphere);
//Distance
float distCVector3ToSphereSurface(const CVector3& CVector3, const Sphere& sphere);
float distCVector3ToSphereCenter(const CVector3& CVector3, const Sphere& sphere);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////AABB functions/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnAABBToCVector3(const CVector3& CVector3, const AABB& box);
//Distance
float distCVector3ToAABB(const CVector3& CVector3, const AABB& box);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Quad functions/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnQuadToCVector3(const CVector3& CVector3, const Quad& quad);
//Distance
float distCVector3ToQuad(const CVector3& CVector3, const Quad& quad);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Cylinder functions/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnCylinderToCVector3(const CVector3& CVector3, const Cylinder& cylinder);
//Distance
float distCVector3ToCylinder(const CVector3& CVector3, const Cylinder& cylinder);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Capsule functions//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest CVector3
CVector3 closestCVector3OnCapsuleToCVector3(const CVector3& CVector3, const Capsule& capsule);
//Distance
float distCVector3ToCapsule(const CVector3& CVector3, const Capsule& capsule);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////

//OVERLAPPING TESTS

bool intersectRaySphere(const Ray& ray, const Sphere& sphere, CVector3& rIntersectCVector3, bool findOutCVector3 = false, CVector3* outgoingCVector3 = NULL );


#endif //GURU_PRIMITIVES_H
