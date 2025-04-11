#include "Function.h"
#include <Novice.h>
#include <corecrt_math.h>

static const int kColumnWidth = 60;

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return {
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	};
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return {
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	};
}

Vector3 Multiply(float scalar, const Vector3& v) { 
	return {
		scalar * v.x,
		scalar * v.y,
		scalar * v.z
	}; 
}

float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Length(const Vector3& v) { 
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	if (length == 0.0f) {
		return {0.0f, 0.0f, 0.0f};
	}
	return {v.x / length, v.y / length, v.z / length};
}
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {

	Novice::ScreenPrintf(x, y, "%0.2f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%0.2f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%0.2f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}
