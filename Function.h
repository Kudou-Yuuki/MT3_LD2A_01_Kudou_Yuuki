
#pragma once
#include "../DirectXGame/math/Matrix4x4.h"
#include "../DirectXGame/math/Vector3.h"
#include <cstdint>

struct Sphere {
	Vector3 center;
	float radius;
};
struct Plane {
	Vector3 normal;
	float distance;
};
struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};

struct Triangle {
	Vector3 vertices[3];
};
Vector3 Multiply(const Vector3 vector1, const Vector3& vector2);

	void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 Multiply2(Vector3& a, float& w);
Vector3 Multiply(const Vector3& vector, float scalar);

Vector3 Cross(const Vector3& v1, const Vector3& v2);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix4x4);
Matrix4x4 Inverse(const Matrix4x4& m);

Vector3 Subtract(const Vector3& v1, const Vector3& v2); // Add this overload for Vector3 subtraction
Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate);

Matrix4x4 MakePrespectiveMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, const Vector3& cameraPosition);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color);
Vector3 Project(const Vector3& vector, const Vector3& normal);
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
 
Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);
bool IsCollision(const Sphere& sphere1, const Plane& plane);
bool IsCollision(const Segment& segment, const Plane& plane);
bool IsCollision(const Triangle&triangle , const Segment& segment);

Vector3 Perpendicular(const Vector3& v1);
float Dot(const Vector3& a, const Vector3& b);
Vector3 Normalize(const Vector3& vector);

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color);


void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color);


	struct AABB {
	Vector3 Min;
	Vector3 Max;
};

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color);

bool isCollision(const AABB& aabb1, const AABB& aabb2);

bool isCollision(const AABB& aabb, const Sphere &sphere);