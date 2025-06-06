#define NOMINMAX
#include <Windows.h>  // もし使っているなら
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif
#include "Function.h"
#include <Novice.h>
#include <cmath>
#include <cassert>
#include <algorithm>
#define M_PI 3.14159265358979323846

static const int kColumnWidth = 60;
static const int kRowHeight = 30;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {

	for (int row = 0; row < 4; ++row) {
		for (int colum = 0; colum < 4; ++colum) {
			Novice::ScreenPrintf(x + colum * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][colum]);
		}
	}

	Novice::ScreenPrintf(x, y, "%s", label);
}

Matrix4x4 MakePrespectiveMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result{};
	float f = 1.0f / std::tanf(fovY * 0.5f);
	result.m[0][0] = f / aspectRatio;
	result.m[1][1] = f;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0.0f;
	return result;
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color) {
		Vector3 min = aabb.Min;
		Vector3 max = aabb.Max;

		Vector3 vertices[8] = {
		    {min.x, min.y, min.z},
            {max.x, min.y, min.z},
            {max.x, max.y, min.z},
            {min.x, max.y, min.z},
            {min.x, min.y, max.z},
            {max.x, min.y, max.z},
            {max.x, max.y, max.z},
            {min.x, max.y, max.z}
        };

		Vector3 screenVertices[8];
		for (int i = 0; i < 8; ++i) {
			Vector3 ndc = Transform(vertices[i], viewProjectionMatrix); // NDCへ
			screenVertices[i] = Transform(ndc, viewPortMatrix);         // スクリーン座標へ
		}

		const int edgeIndices[12][2] = {
		    {0, 1},
            {1, 2},
            {2, 3},
            {3, 0},
            {4, 5},
            {5, 6},
            {6, 7},
            {7, 4},
            {0, 4},
            {1, 5},
            {2, 6},
            {3, 7}
        };

		for (int i = 0; i < 12; ++i) {
			Vector3 p0 = screenVertices[edgeIndices[i][0]];
			Vector3 p1 = screenVertices[edgeIndices[i][1]];
			Novice::DrawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, color);
		}
	}

	







	




bool isCollision(const AABB& aabb1, const AABB& aabb2) {
	return (aabb1.Min.x <= aabb2.Max.x && aabb1.Max.x >= aabb2.Min.x) && (aabb1.Min.y <= aabb2.Max.y && aabb1.Max.y >= aabb2.Min.y) && (aabb1.Min.z <= aabb2.Max.z && aabb1.Max.z >= aabb2.Min.z);
}


Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result{};
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);      
	result.m[2][2] = 1.0f / (farClip - nearClip); 
	result.m[3][0] = -(right + left) / (right - left);
	result.m[3][1] = -(top + bottom) / (top - bottom);
	result.m[3][2] = -nearClip / (farClip - nearClip);
	result.m[3][3] = 1.0f;
	return result;
}
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) { 

	Matrix4x4 result{};
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;
	return result;
}
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, const Vector3& cameraPosition) {
	const float kGridHalfWhidth = 2.0f;
	const uint32_t kSubdvision = 10;
	const float kGridEvery = (kGridHalfWhidth * 2.0f) / float(kSubdvision);

	for (uint32_t xIndex = 0; xIndex <= kSubdvision; ++xIndex) {
		float offset = -kGridHalfWhidth + kGridEvery * float(xIndex);
		Vector3 start = {-kGridHalfWhidth, 0.0f, offset};
		Vector3 end = {kGridHalfWhidth, 0.0f, offset};
		Vector3 ndcStart = Transform(start, viewProjectionMatrix);
		Vector3 ndcEnd = Transform(end, viewProjectionMatrix);
		Vector3 startScreen = Transform(ndcStart, viewportMatrix);
		Vector3 endScreen = Transform(ndcEnd, viewportMatrix);
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, 0xFFFFFFFF);
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdvision; ++zIndex) {
		float offset = -kGridHalfWhidth + kGridEvery * float(zIndex);
		Vector3 start = {offset, 0.0f, -kGridHalfWhidth};
		Vector3 end = {offset, 0.0f, kGridHalfWhidth};
		Vector3 ndcStart = Transform(start, viewProjectionMatrix);
		Vector3 ndcEnd = Transform(end, viewProjectionMatrix);
		Vector3 startScreen = Transform(ndcStart, viewportMatrix);
		Vector3 endScreen = Transform(ndcEnd, viewportMatrix);
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, 0xFFFFFFFF);
	}

	Vector3 cameraPositionScreen = Transform(cameraPosition, viewProjectionMatrix);
}






Matrix4x4 MakeTrancelateMatrix(Vector3 translate) {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeScaleMatrix(Vector3 scale) {
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cosf(radian);
	result.m[1][2] = std::sinf(radian);
	result.m[2][1] = -std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = std::cosf(radian);
	result.m[0][2] = -std::sinf(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = std::cosf(radian);
	result.m[0][1] = std::sinf(radian);
	result.m[1][0] = -std::sinf(radian);
	result.m[1][1] = std::cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int row = 0; row < 4; ++row) {
		for (int colum = 0; colum < 4; ++colum) {
			result.m[row][colum] = m1.m[row][0] * m2.m[0][colum] + m1.m[row][1] * m2.m[1][colum] + m1.m[row][2] * m2.m[2][colum] + m1.m[row][3] * m2.m[3][colum];
		}
	}
	return result;
}

Vector3 Multiply2(Vector3& a, float& w) { 
	Vector3 result;
	result.x = a.x * w;
	result.y = a.y * w;
	result.z = a.z * w;
	return result;
}
Vector3 Multiply(const Vector3& vector, float scalar) { return {vector.x * scalar, vector.y * scalar, vector.z * scalar}; }
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {

	Novice::ScreenPrintf(x, y, "%0.2f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%0.2f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%0.2f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {
	Matrix4x4 result{};

	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZ = Multiply(rotateX, Multiply(rotateY, rotateZ));

	Matrix4x4 ScaleRotateMatrix = Multiply(MakeScaleMatrix(scale), rotateXYZ);
	result = Multiply(ScaleRotateMatrix, MakeTrancelateMatrix(translate));

	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix4x4) {
	Vector3 result;
	result.x = vector.x * matrix4x4.m[0][0] + vector.y * matrix4x4.m[1][0] + vector.z * matrix4x4.m[2][0] + matrix4x4.m[3][0];
	result.y = vector.x * matrix4x4.m[0][1] + vector.y * matrix4x4.m[1][1] + vector.z * matrix4x4.m[2][1] + matrix4x4.m[3][1];
	result.z = vector.x * matrix4x4.m[0][2] + vector.y * matrix4x4.m[1][2] + vector.z * matrix4x4.m[2][2] + matrix4x4.m[3][2];
	float w = vector.x * matrix4x4.m[0][3] + vector.y * matrix4x4.m[1][3] + vector.z * matrix4x4.m[2][3] + matrix4x4.m[3][3];

	// wが0なら変換できない（カメラの視野外など）
	if (w != 0.0f) {
		result.x /= w;
		result.y /= w;
		result.z /= w;
	} else {
		// 代替処理（例：そのまま返す・0で埋める・巨大な数を入れるなど）
		result = {0.0f, 0.0f, 0.0f}; // 安全に落とす場合
	}

	return result;
}
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 i;

	float d =
	    m.m[0][0] *
	        (m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])) -
	    m.m[0][1] *
	        (m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])) +
	    m.m[0][2] *
	        (m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) -
	    m.m[0][3] *
	        (m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	i.m[0][0] =
	    (m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])) / d;
	i.m[0][1] =
	    -(m.m[0][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[0][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])) / d;
	i.m[0][2] =
	    (m.m[0][1] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) + m.m[0][3] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1])) / d;
	i.m[0][3] =
	    -(m.m[0][1] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) + m.m[0][3] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1])) / d;

	i.m[1][0] =
	    -(m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])) / d;
	i.m[1][1] =
	    (m.m[0][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])) / d;
	i.m[1][2] =
	    -(m.m[0][0] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0])) / d;
	i.m[1][3] =
	    (m.m[0][0] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][1] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0])) / d;

	i.m[2][0] =
	    (m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) / d;
	i.m[2][1] =
	    -(m.m[0][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) / d;
	i.m[2][2] =
	    (m.m[0][0] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0])) / d;
	i.m[2][3] =
	    -(m.m[0][0] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0])) / d;

	i.m[3][0] =
	    -(m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) / d;
	i.m[3][1] =
	    (m.m[0][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[0][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) / d;
	i.m[3][2] =
	    -(m.m[0][0] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]) + m.m[0][2] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0])) / d;
	i.m[3][3] =
	    (m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0])) / d;

	return i;
}
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color) {
	const uint32_t kSubdivision = 20;
	const float kLonEvery = 2.0f * float(M_PI) / float(kSubdivision);
	const float kLatEvery = float(M_PI) / float(kSubdivision);

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;
		float latNext = lat + kLatEvery;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = kLonEvery * lonIndex;
			float lonNext = lon + kLonEvery;

		
			Vector3 a = {
			    sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon), sphere.center.y + sphere.radius * std::sinf(lat), sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon)};
			Vector3 b = {
			    sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lonNext), sphere.center.y + sphere.radius * std::sinf(lat),
			    sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lonNext)};
			Vector3 c = {
			    sphere.center.x + sphere.radius * std::cosf(latNext) * std::cosf(lon), sphere.center.y + sphere.radius * std::sinf(latNext),
			    sphere.center.z + sphere.radius * std::cosf(latNext) * std::sinf(lon)};

			Vector3 screenA = Transform(Transform(a, viewProjectionMatrix), viewPortMatrix);
			Vector3 screenB = Transform(Transform(b, viewProjectionMatrix), viewPortMatrix);
			Vector3 screenC = Transform(Transform(c, viewProjectionMatrix), viewPortMatrix);

			
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y), color);
			Novice::DrawLine(int(screenC.x), int(screenC.y), int(screenA.x), int(screenA.y), color);
		}
	}
}

Vector3 Add(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }

Vector3 Subtract(const Vector3& v1, const Vector3& v2) { return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2) { 

	float distanceSquared =
		(sphere1.center.x - sphere2.center.x) * (sphere1.center.x - sphere2.center.x) + 
		(sphere1.center.y - sphere2.center.y) * (sphere1.center.y - sphere2.center.y) +
	    (sphere1.center.z - sphere2.center.z) * (sphere1.center.z - sphere2.center.z);
	float radiusSum = sphere1.radius + sphere2.radius;
	return distanceSquared <= radiusSum * radiusSum;
}

bool IsCollision(const Sphere& sphere1, const Plane& plane) { 
	
	float distance = plane.normal.x * sphere1.center.x + plane.normal.y * sphere1.center.y + plane.normal.z * sphere1.center.z - plane.distance;
	return std::abs(distance) <= sphere1.radius;


}
float Dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
bool IsCollision(const Segment& segment, const Plane& plane) {
	Vector3 n = Normalize(plane.normal);

	float distStart = Dot(n, segment.origin) - plane.distance;

	Vector3 endPoint = {segment.origin.x + segment.diff.x, segment.origin.y + segment.diff.y, segment.origin.z + segment.diff.z};
	float distEnd = Dot(n, endPoint) - plane.distance;

	const float epsilon = 1e-6f;
	if (std::abs(distStart) < epsilon || std::abs(distEnd) < epsilon) {
		return true;
	}

	if (distStart * distEnd < 0.0f) {
		return true;
	}

	return false;
}
Vector3 Perpendicular(const Vector3& v1) { 

	Vector3 result;
	if (v1.x != 0.0f || v1.y != 0.0f) {
		result.x = -v1.y;
		result.y = v1.x;
		result.z = 0.0f;
	} else {
		result.x = 0.0f;
		result.y = -v1.z;
		result.z = v1.y;
	}
	return result;


}
Vector3 Normalize(const Vector3& vector) { 

	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	if (length == 0.0f) {
		return {0.0f, 0.0f, 0.0f}; // Avoid division by zero
	}
	return {vector.x / length, vector.y / length, vector.z / length};


}
Vector3 Multiply(float scalar, const Vector3& vector) { return {vector.x * scalar, vector.y * scalar, vector.z * scalar}; }

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color) {
	// 平面の中心位置
	Vector3 center = Multiply(plane.normal, plane.distance);

	// u, vを計算
	Vector3 u = Perpendicular(plane.normal);
	Vector3 v = Cross(plane.normal, u);

	// 正規化（Normalizeなしなので手動で）
	float uLen = sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
	float vLen = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	u = Multiply(1.0f / uLen, u);
	v = Multiply(1.0f / vLen, v);

	// サイズをかける
	float size = 2.0f;
	u = Multiply(size, u);
	v = Multiply(size, v);

	// 4つの頂点を作る
	Vector3 points[4];
	points[0] = Add(center, Add(u, v));                       // +u +v
	points[1] = Add(center, Subtract(u, v));                  // +u -v
	points[2] = Add(center, Subtract(Multiply(-1.0f, u), v)); // -u -v
	points[3] = Add(center, Add(Multiply(-1.0f, u), v));      // -u +v

	// 変換
	for (int i = 0; i < 4; ++i) {
		points[i] = Transform(Transform(points[i], viewProjectionMatrix), viewPortMatrix);
	}

	// 線描画（矩形になるように順に繋ぐ）
	for (int i = 0; i < 4; ++i) {
		int next = (i + 1) % 4;
		Novice::DrawLine((int)points[i].x, (int)points[i].y, (int)points[next].x, (int)points[next].y, color);
	}
}



Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	float lengthSquared = v2.x * v2.x + v2.y * v2.y + v2.z * v2.z;
	if (lengthSquared == 0.0f) {
		return {0.0f, 0.0f, 0.0f}; // Avoid division by zero
	}
	float scale = dot / lengthSquared;
	return {v2.x * scale, v2.y * scale, v2.z * scale};
} 

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 segmentToPoint = Subtract(point, segment.origin);
	float t = (segmentToPoint.x * segment.diff.x + segmentToPoint.y * segment.diff.y + segmentToPoint.z * segment.diff.z) /
	          (segment.diff.x * segment.diff.x + segment.diff.y * segment.diff.y + segment.diff.z * segment.diff.z);
	if (t < 0.0f) {
		return segment.origin; // Closest point is the start of the segment
	} else if (t > 1.0f) {
		return Add(segment.origin, segment.diff); // Closest point is the end of the segment
	} else {
		return Add(segment.origin, {segment.diff.x * t, segment.diff.y * t, segment.diff.z * t}); // Closest point is within the segment
	}
}
Vector3 Multiply(const Vector3 vector1, const Vector3& vector2) {
	Vector3 result;
	result.x = vector1.x * vector2.x;
	result.y = vector1.y * vector2.y;
	result.z = vector1.z * vector2.z;
	return result;

}
bool IsCollision(const Triangle& triangle, const Segment& segment) {
	const Vector3& v0 = triangle.vertices[0];
	const Vector3& v1 = triangle.vertices[1];
	const Vector3& v2 = triangle.vertices[2];

	Vector3 edge1 = Subtract(v1, v0);
	Vector3 edge2 = Subtract(v2, v0);
	Vector3 dir = segment.diff; // ✅ 正しく方向ベクトルを取得
	Vector3 pvec = Cross(dir, edge2);
	float det = Dot(edge1, pvec);

	if (fabsf(det) < 1e-6f)
		return false; // 平行

	float invDet = 1.0f / det;
	Vector3 tvec = Subtract(segment.origin, v0);
	float u = Dot(tvec, pvec) * invDet;
	if (u < 0.0f || u > 1.0f)
		return false;

	Vector3 qvec = Cross(tvec, edge1);
	float v = Dot(dir, qvec) * invDet;
	if (v < 0.0f || u + v > 1.0f)
		return false;

	float t = Dot(edge2, qvec) * invDet;

	if (t < 0.0f || t > 1.0f)
		return false;

	return true;
}
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color) {

	Vector3 screenVertices[3];
	for (int i = 0; i < 3; ++i) {
		screenVertices[i] = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewPortMatrix);
	}
	// 三角形の辺を描画
	for (int i = 0; i < 3; ++i) {
		int next = (i + 1) % 3;
		Novice::DrawLine((int)screenVertices[i].x, (int)screenVertices[i].y, (int)screenVertices[next].x, (int)screenVertices[next].y, color);
	}
}

bool isCollision(const AABB& aabb, const Sphere& sphere) {

	Vector3 closestPoint = {

	    std::max(aabb.Min.x, std::min(sphere.center.x, aabb.Max.x)), std::max(aabb.Min.y, std::min(sphere.center.y, aabb.Max.y)), std::max(aabb.Min.z, std::min(sphere.center.z, aabb.Max.z))};

	Vector3 diff = Subtract(closestPoint, sphere.center);
	float distanceSquared = Dot(diff, diff);

	return distanceSquared <= (sphere.radius * sphere.radius);
}
bool isCollision(const AABB& aabb, const Segment& segment) {
	Vector3 start = segment.origin;
	Vector3 dir = segment.diff; // 方向ベクトル（終点 = origin + diff）

	float tMin = 0.0f;
	float tMax = 1.0f;

	for (int i = 0; i < 3; ++i) {
		float s = (&start.x)[i];
		float d = (&dir.x)[i];
		float min = (&aabb.Min.x)[i];
		float max = (&aabb.Max.x)[i];

		if (fabsf(d) < 1e-6f) {
			// 平行 → 範囲外なら衝突なし
			if (s < min || s > max) {
				return false;
			}
		} else {
			float t1 = (min - s) / d;
			float t2 = (max - s) / d;
			if (t1 > t2)
				std::swap(t1, t2);

			tMin = std::max(tMin, t1);
			tMax = std::min(tMax, t2);

			// 線分の範囲外（0〜1）なら衝突しない
			if (tMin > tMax || tMax < 0.0f || tMin > 1.0f) {
				return false;
			}
		}
	}

	return true;
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return {v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t};


}

void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewPortMatrix, uint32_t color) {

	const int kSubdivision = 32; // 分割数（滑らかさ）

	auto Bezier = [](float t, const Vector3& p0, const Vector3& p1, const Vector3& p2) -> Vector3 {
		float u = 1.0f - t;
		return u * u * p0 + 2 * u * t * p1 + t * t * p2;
	};


	Vector3 previous = Bezier(0.0f, controlPoint0, controlPoint1, controlPoint2);
	for (int i = 1; i <= kSubdivision; ++i) {
		float t = static_cast<float>(i) / kSubdivision;
		Vector3 current = Bezier(t, controlPoint0, controlPoint1, controlPoint2);

		// 座標変換
		Vector3 prevScreen = Transform(Transform(previous, viewProjectionMatrix), viewPortMatrix);
		Vector3 currScreen = Transform(Transform(current, viewProjectionMatrix), viewPortMatrix);

		// 描画（仮の関数：Line）
		Novice::DrawLine(static_cast<int>(prevScreen.x), static_cast<int>(prevScreen.y), static_cast<int>(currScreen.x), static_cast<int>(currScreen.y), color);

		previous = current;
	}
}
