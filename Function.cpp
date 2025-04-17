#include "Function.h"
#include <Novice.h>
#include <cmath>

static const int kColumnWidth = 60;
static const int kRowHeight = 30;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {

	for (int row = 0; row < 4; ++row) {
		for (int colum = 0; colum < 4; ++colum) {
			Novice::ScreenPrintf(x + colum * kColumnWidth, y + (row + 1) * kRowHeight, "%3.02f", matrix.m[row][colum]);
		}
	}

	Novice::ScreenPrintf(x, y, "%s", label);
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int row = 0; row < 4; ++row) {
		for (int colum = 0; colum < 4; ++colum) {
			result.m[row][colum] = m1.m[row][0] * m2.m[0][colum] + m1.m[row][1] * m2.m[1][colum] + m1.m[row][2] * m2.m[2][colum] + m1.m[row][3] * m2.m[3][colum];
		}
	}
	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cosf(radian);
	result.m[1][2] = -std::sinf(radian);
	result.m[2][1] = std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) { 
	Matrix4x4 result;
	result.m[0][0] = std::cosf(radian);
	result.m[0][2] = std::sinf(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = -std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cosf(radian);
	result.m[0][1] = -std::sinf(radian);
	result.m[1][0] = std::sinf(radian);
	result.m[1][1] = std::cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}