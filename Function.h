
#pragma once
#include "../DirectXGame/math/Matrix4x4.h"
#include "../DirectXGame/math/Vector3.h"
#include <cstdint>
struct Sphere {
	Vector3 center;
	float radius;

};

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);


Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix4x4);
Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate);

Matrix4x4 MakePrespectiveMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

//void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Vector3& cameraPosition, uint32_t color);
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, const Vector3& cameraPosition);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color);
