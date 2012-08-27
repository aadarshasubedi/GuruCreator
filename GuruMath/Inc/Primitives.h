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


class Point;
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
void printPoint(const Point& point);
void printRay(const Ray& ray);
void printLine(const Line& line);
void printTriangle(const Triangle& triangle);
void printPlane(const Plane& plane);
void printSphere(const Sphere& sphere);
void printQuad(const Quad& quad);
void printAABB(const AABB& box);

void testPrimitives();
#endif


//There are many primitive types that need to be represented in the simulation.  From here on out, when I say 'Physics' I mean anything that is simulated
// and requires the system to handle and control interactions and their reactions.

class Point
{
public:
	Point(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	Point(const Point& pt);
	Point(const CVector3& v);
	Point(CVector3 v);
	~Point();

	//Setters/Getters.
	inline const float& x() const { return mX; }
	inline float& x() { return mX; }
	inline void x(const float& value) { mX = value; }	
	inline void setX(const float& value) { mX = value; }

	inline const float& y() const { return mY; }
	inline float& y() { return mY; }
	inline void y(const float& value) { mY = value; }
	inline void setY(const float& value) { mY = value; }

	inline const float& z() const { return mZ; }
	inline float& z() { return mZ; }
	inline void z(const float& value) { mZ = value; }
	inline void setZ(const float& value) { mZ = value; }

	//This function takes a second point and forms a ray from the two points.
	Ray makeRay(const Point& B);

	////Member operators.
	operator CVector3() { return CVector3(mX, mY, mZ); }

private:
	//point is simple x,y,z, like vector but w/o many of the properties.
	float mX, mY, mZ;
};
Point operator+(const Point& pt, const CVector3& vec);
Point operator-(const Point& pt, const CVector3& vec);
CVector3 operator-(const Point& A, const Point& B);
Point operator*( const Matrix<>& mat);
bool operator==(const Point& A, const Point& B);


/////////////////////////////////////////////////////////


class Ray
{
public:
	Ray();
	Ray(const Point& pt, const CVector3& vec );
	~Ray();

	//Usual getters/setters.
	inline const Point& origin() const { return mOrigin; }
	inline Point& origin() { return mOrigin; }
	inline void origin(const Point& pt) { mOrigin = pt; }
	inline void setOrigin(const Point& pt) { mOrigin = pt; }

	inline const CVector3& dir() const { return mDir; }
	inline CVector3& dir() { return mDir; }
	inline void dir(const CVector3& vec) { mDir = vec; }
	inline void setOrigin(const CVector3& vec) { mDir = vec; }

	////OPERATORS
	//Point
	bool operator==(const Point& point); //point is origin
	bool operator<(const Point& point);  //point in front of origin, along mDir
	bool operator>(const Point& point);  //point behind origin, along -mDir
	float operator*(const Point& point); //project distance of (point - origin) * mDir

	//Ray
	bool operator==(const Ray& ray); //is ray parallel.
	bool operator^(const Ray& ray);  //is ray perpendicular.
	bool operator<(const Ray& ray);  //is ray pointing away from this ray, not parallel.
	bool operator>(const Ray& ray);  //is ray pointing same direction, not parallel.

	//CVector3
	bool operator==(const CVector3& vec); //is vector parallel
	bool operator^(const CVector3& vec);  //is vector perpendicular
	bool operator<(const CVector3& vec);  //is vector pointing away, not parallel
	bool operator>(const CVector3& vec);  //is vector point same way, no parallel
	
	//LINES
	//in > line : in fully contains line
	//in == line : line straddles/on in
	//in < line : in fully disowns line
	//in * line : closest linear distance
	bool operator==(const Line& line);
	bool operator<(const Line& line);
	bool operator>(const Line& line);
	float operator*(const Line& line); 

	//PLANES
	//in > plane : in fully above plane
	//in == plane : plane somehow crosses in
	//in < plane : in fully under plane
	//in * plane : closest linear distance between plane and in
	bool operator==(const Plane& plane);
	bool operator<(const Plane& plane); 
	bool operator>(const Plane& plane); 
	float operator*(const Plane& plane);

	//TRIANGLES
	//in > triangle : in fully above triangle
	//in == triangle : in straddles triangle
	//in < triangle : in fully below triangle
	//in * triangle : closest linear distance between in and triangle
	bool operator==(const Triangle& tri);
	bool operator<(const Triangle& tri); 
	bool operator>(const Triangle& tri); 
	float operator*(const Triangle& tri);

	//SPHERES
	//in > sphere : in fully outside sphere
	//in == sphere : in straddles surface of sphere
	//in < sphere : in fully inside sphere
	//in * sphere : closest linear distance between sphere and in
	bool operator==(const Sphere& sphere);
	bool operator<(const Sphere& sphere); 
	bool operator>(const Sphere& sphere); 
	float operator*(const Sphere& sphere);

	//English friendly versions.
	bool pointIsOrigin(const Point& point);
	bool pointBehindOrigin(const Point& point);
	bool pointInFrontOfOrigin(const Point& point);
	float pointProjectedDistance(const Point& point);
	bool vectorParallel(const Vector3& vec);
	bool vectorPerpendicular(const Vector3& vec);
	bool rayIsSame(const Ray& ray);

	//Uses the direction vector to generate another point at origin + mDir
	Line makeLine();

private:
	float projectPoint(const Point& point);
	bool determineBehind(const Point& point);

private:
	//Defined by a point and vector.  The vector is infinite in that direction.
	Point mOrigin;
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
	Line(const Point& A, const Point& B) : mA(A), mB(B) {}

	CVector3 getVector() const { return mB - mA; }

	//Getters and setters.
	inline const Point& A() const { return mA; }
	inline Point& A() { return mA; }
	inline void A(const Point& pt) { mA = pt; }

	inline const Point& B() const { return mB; }
	inline Point& B() { return mB; }
	inline void B(const Point& pt) { mB = pt; }

	////OPERATORS
	//Point
	//point is on line.
	bool operator==(const Point& point);
	//point is on line behind A, going away from B.
	bool operator>(const Point& point);
	//point is on line in front of B, going away from A
	bool operator<(const Point& point);
	//barycentric coords of the point in respect to A, B.  
	//Since for two points the coords u, v must satisfy u + v = 1.0, this only needs to return one, the other can be generated.
	//returns 'u'
	//Only valid if the point is on the line.  Otherwise the values will be undefined.
	float operator*(const Point& point);

	//RAY
	bool operator==(const Ray& ray); //is ray parallel
	bool operator^(const Ray& ray); //is ray perpendicular

	//dir = B - A, and A is origin.
	Ray makeRay();

private:
	//The two points that define a Line.  The line is infinite in both directions.
	Point mA, mB;
};


/////////////////////////////////////////////////////////


class Triangle
{
public:
	Triangle();
	Triangle(const Point& a, const Point& b, const Point& c);
	~Triangle();

	//Getters and setters.
	//Getters and setters.
	inline const Point& A() const { return mA; }
	inline Point& A() { return mA; }
	inline void A(const Point& pt) { mA = pt; }
	inline void setA(const Point& pt) { mA = pt; }

	inline const Point& B() const { return mB; }
	inline Point& B() { return mB; }
	inline void B(const Point& pt) { mB = pt; }
	inline void setB(const Point& pt) { mB = pt; }

	inline const Point& C() const { return mC; }
	inline Point& C() { return mC; }
	inline void C(const Point& pt) { mC = pt; }
	inline void setC(const Point& pt) { mC = pt; }

	////Operators
	//Point
	bool operator==(const Point& pt);
	bool operator>(const Point& pt);
	bool operator<(const Point& pt);
	float operator*(const Point& pt);

	//Ray
	bool operator==(const Ray& ray);
	bool operator^(const Ray& ray);
	bool operator|(const Ray& ray);
	float operator*(const Ray& ray);

	//Line
	bool operator==(const Line& line);
	bool operator>(const Line& line);
	bool operator<(const Line& line);
	float operator*(const Line& line);

	//Plane
	bool operator==(const Plane& plane);
	//bool operator^(const Plane& pt);
	//bool operator|(const Plane& pt);
	float operator*(const Plane& plane);

	//Retrieves the normal for the triangle.  Does not store it.  Normalized.
	CVector3 getNormal() const;

	//This function takes 3 Line objects that it will fill in with the appropriate points.
	void formLines(Line& one, Line& two, Line& three);

	//Get a plane from the triangle.  Use A as the point on the plane.
	Plane getPlane();

private:
	//Triangle is 3 points.  Additionally, the lines can be formed from these three points as helper functions.	
	//@NOTE: Triangles ASSUME COUNTER CLOCKWISE ORDERING!!
	Point mA, mB, mC;
};


/////////////////////////////////////////////////////////


class Plane
{
public:
	////Constructors/Destructor
	Plane();
	Plane(const Point& a, const CVector3& norm);
	virtual ~Plane();

	////Getters and Setters
	inline const Point& A() const { return mA; }
	inline Point& A() { return mA; }
	inline void A(const Point& pt) { mA = pt; }
	inline void setA(const Point& pt) { mA = pt; }

	inline const CVector3& normal() const { return mNormal; }
	inline CVector3& normal() { return mNormal; }
	inline void normal(const CVector3& vec) { mNormal = vec; }
	inline void setNormal(const CVector3& vec) { mNormal = vec; }

	////Operators

	//Check functions	
	bool operator==(const Point& pt); //is point pt on the plane
	bool operator<(const Point& pt); //is point pt below the plane.
	bool operator>(const Point& pt); //is point pt above the plane.
	float operator*(const Point& pt); //distance along normal of pt from the plane

	//English looking names for the above operators.
	bool pointOnPlane(const Point& pt) { return (*this == pt); }
	bool pointAbovePlane(const Point& pt) { return (*this > pt); }
	bool pointBelowPlane(const Point& pt) { return (*this < pt); }
	float pointDistanceToPlane(const Point& pt) { return (*this * pt); }

private:
	int determinePointOrientation(const Point& pt); //This function return -1 for below, 0 for on, 1 for above plane.
	float projectPoint(const Point& pt); //projects the vector pt - A onto the normal.

protected:
	Point mA;
	CVector3 mNormal;
};


class Quad : public Plane
{
public:
	Quad();
	Quad(const Point& center, const CVector3& u, const CVector3& v);
	Quad(const Point& A, const Point& B, const Point& C);
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
	//Need only define one more point from a plane.  This assumes the point
	// coming from the Plane is used as either the min or maxpt.  By default it is
	// the min point.
	CVector3 mU, mV;
	float mEu, mEv;	
};


class Sphere
{
public:
	Sphere();
	Sphere(const float& radius, const Point& center);
	~Sphere();

	//Getters and Setters
	inline const Point& center() const { return mCenter; }
	inline Point center() { return mCenter; }
	inline void center(const Point& pt) { mCenter = pt; }
	inline void setCenter(const Point& pt) { mCenter = pt; }

	inline const float& radius() const { return mRadius; }
	inline float& radius() { return mRadius; }
	inline void radius(const float& r) { mRadius = r; }
	inline void setRadius(const float& r) { mRadius = r; }

	//Operators
	//POINTS
	bool operator==(const Point& point); //point is on sphere surface.
	bool operator<(const Point& point); //point is outside sphere.
	bool operator>(const Point& point); //point is inside sphere.
	//point distance to sphere surface.  Negative means inside sphere.  
	//Add/Subtract radius for distance to center.
	float operator*(const Point& point); 

	//LINES
	bool operator==(const Line& line); //both points lie on the sphere surface.
	bool operator<(const Line& line); //line is fully outside of sphere.
	bool operator>(const Line& line); //line is fully inside of sphere.
	//shortest linear distance to the sphere surface.  Negative if fully contained.
	//Add/Subtract radius for distance to center.
	float operator*(const Line& line); 

	//TRIANGLES
	bool operator==(const Triangle& tri); //all points lie on the sphere surface.
	bool operator<(const Triangle& tri); //tri is fully outside of sphere.
	bool operator>(const Triangle& tri); //tri is fully inside of sphere.
	//shortest linear distance to the sphere surface.  Negative if fully contained.
	//Add/Subtract radius for distance to center.
	float operator*(const Triangle& tri); 

	//English friendly funcs.
	bool pointOnSurface(const Point& point);
	bool pointOutside(const Point& point);
	bool pointInside(const Point& point);
	float pointDistanceToSurface(const Point& point);
	
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
	Point mCenter;
	float mRadius;
};


//The AABB has many definitions, but this uses the lowest memory footprint method
class AABB
{
public:
	////Constructors/Destructor
	AABB();
	AABB(const Point& center, float h_width, float h_height, float h_depth);
	AABB(const Point& center, const CVector3& h_extents);
	AABB(const Point& minPt, const Point& maxPt);
	AABB(const Point& center, float* h_extents); //assumes that h_extents is at least 3 elements, will only use the first 3.
	~AABB();

	////Getter and Setters
	inline const Point& center() const { return mCenter; }
	inline Point& center() { return mCenter; }
	inline void center(const Point& pt) { mCenter = pt; }
	inline void setCenter(const Point& pt) { mCenter = pt; }

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

	////Operators 
	//POINTS
	//box > point : box contains point
	//box == point : point is on box
	//box < point : box does not contain point
	//box * point : nearest distance from point to box surface
	bool operator==(const Point& point);
	bool operator<(const Point& point);
	bool operator>(const Point& point);
	float operator*(const Point& point);

	//RAYS
	//box > ray : box fully in front of ray
	//box == ray : box is colliding with ray
	//box < ray : box fully behind ray
	//box * ray : closest linear distance
	bool operator==(const Ray& ray);
	bool operator<(const Ray& ray);
	bool operator>(const Ray& ray);
	float operator*(const Ray& ray); 

	//LINES
	//box > line : box fully contains line
	//box == line : line straddles/on surface
	//box < line : box fully disowns line
	//box * line : closest linear distance
	bool operator==(const Line& line);
	bool operator<(const Line& line);
	bool operator>(const Line& line);
	float operator*(const Line& line); 

	//PLANES
	//box > plane : box fully above plane
	//box == plane : plane somehow crosses box
	//box < plane : box fully under plane
	//box * plane : closest linear distance between plane and box
	bool operator==(const Plane& plane);
	bool operator<(const Plane& plane); 
	bool operator>(const Plane& plane); 
	float operator*(const Plane& plane);

	//TRIANGLES
	//box > triangle : box fully above triangle
	//box == triangle : box straddles triangle
	//box < triangle : box fully below triangle
	//box * triangle : closest linear distance between box and triangle
	bool operator==(const Triangle& tri);
	bool operator<(const Triangle& tri); 
	bool operator>(const Triangle& tri); 
	float operator*(const Triangle& tri);

	//SPHERES
	//box > sphere : box fully outside sphere
	//box == sphere : box straddles/penetrates sphere
	//box < sphere : box fully inside sphere
	//box * sphere : closest linear distance between sphere and box
	bool operator==(const Sphere& sphere);
	bool operator<(const Sphere& sphere); 
	bool operator>(const Sphere& sphere); 
	float operator*(const Sphere& sphere);

private:
	void AABB::convertToSlab(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax);

private:
	Point mCenter;
	CVector3 mHalfExtents; //3 floats wrapped in a vector.  order: w, h, d (x,y,z)
};



//Intersection, smallest distance, closest point and other primitive-primitive calculations.

//////////////////////////////////////////////////////////////////////
///////Ray functions//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnRayToPoint(const Point& point, const Ray& ray);
Point closestPointOnRayToRay(const Ray& ray, const Ray& A);
//Distance
float distPointToRay(const Point& point, const Ray& ray);
float projDistPointToRay(const Point& point, const Ray& ray);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Segment functions//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnSegmentToPoint(const Point& point, const Line& segment);
Point closestPointOnSegmentToSegment(const Line& segment, const Line& A);
Point closestPointOnSegmentToRay(const Line& segment, const Ray& ray);
Point closestPointOnSegmentToLine(const Line& segment, const Line& line);
Point closestPointOnSegmentToPlane(const Line& segment, const Plane& plane);
//Bary coords
//Distance
float distSegmentToPoint(const Point& point, const Line& segment);
float projDistSegmentToPoint(const Point& point, const Line& segment);
float distSegmentToSegment(const Line& segment, const Line& A);
float distSegmentToRay(const Line& segment, const Ray& ray);
float distSegmentToLine(const Line& segment, const Line& line);
float distSegmentToPlane(const Line& segment, const Plane& plane);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Line functions/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnLineToPoint(const Point& point, const Line& line);
//Distance
float distPointToLine(const Point& point, const Line& line);
float distSphereToLine(const Sphere& sphere, const Line& line);
float projDistPointToLine(const Point& point, const Line& line);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Plane functions////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnPlaneToPoint(const Point& point, const Plane& plane);
//Distance
float distPointToPlane(const Point& point, const Plane& plane);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Triangle functions/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnTriangleToPoint(const Point& point, const Triangle& triangle);
//Barycentric coords
void baryCoordsClosestPointOnTriangle(const Point& point, const Triangle& triangle, float& u, float& v, float& w);
//Distance
float distRayToTriangle(const Ray& ray, const Triangle& triangle);
float distPointToTriangle(const Point& point, const Triangle& triangle);
float distPlaneToTriangle(const Plane& plane, const Triangle& triangle);
float distLineToTriangle(const Line& line, const Triangle& triangle);
float distSphereToTriangle(const Sphere& sphere, const Triangle& triangle);
float distAABBToTriangle(const AABB& aabb, const Triangle& triangle);
float distQuadToTriangle(const Quad& quad, const Triangle& triangle);
//Intersection/Containment
bool containsPointInTriangle(const Point& point, const Triangle& triangle );
bool rayIntersectsTriangle(const Ray& ray, const Triangle& triangle);
bool lineIntersectsTriangle(const Line& line, const Triangle& triangle);
bool segmentIntersectsTriangle(const Line& segment, const Triangle& triangle);
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Sphere functions///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnSphereSurfaceToPoint(const Point& point, const Sphere& sphere);
Point closestPointOnSphereToPoint(const Point& point, const Sphere& sphere);
Point closestPointOnSphereToLine(const Line& line, const Sphere& sphere);
//Distance
float distPointToSphereSurface(const Point& point, const Sphere& sphere);
float distPointToSphereCenter(const Point& point, const Sphere& sphere);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////AABB functions/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnAABBToPoint(const Point& point, const AABB& box);
//Distance
float distPointToAABB(const Point& point, const AABB& box);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Quad functions/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnQuadToPoint(const Point& point, const Quad& quad);
//Distance
float distPointToQuad(const Point& point, const Quad& quad);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Cylinder functions/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnCylinderToPoint(const Point& point, const Cylinder& cylinder);
//Distance
float distPointToCylinder(const Point& point, const Cylinder& cylinder);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///////Capsule functions//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//Closest point
Point closestPointOnCapsuleToPoint(const Point& point, const Capsule& capsule);
//Distance
float distPointToCapsule(const Point& point, const Capsule& capsule);
//Intersection/Containment
//////////////////////////////////////////////////////////////////////

//OVERLAPPING TESTS

bool intersectRaySphere(const Ray& ray, const Sphere& sphere, CVector3& rIntersectPoint, bool findOutPoint = false, CVector3* outgoingPoint = NULL );


#endif //GURU_PRIMITIVES_H
