#include "Function.h"
#include <Novice.h>
#include <corecrt_math.h>

static const int kColumnWidth = 60;
static const int kRowHeight = 20;
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix4x4) {
	Vector3 result;
	
	result.x = vector.x * matrix4x4.m[0][0] + vector.y * matrix4x4.m[1][0] + vector.z * matrix4x4.m[2][0] + matrix4x4.m[3][0];
	result.y = vector.x * matrix4x4.m[0][1] + vector.y * matrix4x4.m[1][1] + vector.z * matrix4x4.m[2][1] + matrix4x4.m[3][1];
	result.z = vector.x * matrix4x4.m[0][2] + vector.y * matrix4x4.m[1][2] + vector.z * matrix4x4.m[2][2] + matrix4x4.m[3][2];
	return result;
}


Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 matrix = {
	    1.0f, 0.0f,0.0f,0.0f,
	    0.0f, 1.0f,0.0f,0.0f,
	    0.0f, 0.0f,1.0f,0.0f,
	    translate.x, translate.y, translate.z, 1.0f 
	};
	return matrix;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 matrix = {scale.x, 0.0f, 0.0f, 0.0f, 0.0f, scale.y, 0.0f, 0.0f, 0.0f, 0.0f, scale.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	return matrix;
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {

	Novice::ScreenPrintf(x, y, "%0.2f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%0.2f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%0.2f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {

	for (int row = 0; row < 4; ++row) {
		for (int colum = 0; colum < 4; ++colum) {
			Novice::ScreenPrintf(x + colum * kColumnWidth, y + (row + 2) * kRowHeight, "%3.02f", matrix.m[row][colum]);
		}
	}

	Novice::ScreenPrintf(x, y, "%s", label);
}