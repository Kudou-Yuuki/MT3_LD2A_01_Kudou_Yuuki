#include "Function.h"
#include <Novice.h>
#include <corecrt_math.h>


static const int kRowHeight = 20;
static const int kColumnWidth = 60;

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i= 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;
	float det = 0.0f;
	for (int i = 0; i < 4; ++i) {
		det += (m.m[0][i] * (m.m[1][(i + 1) % 4] * m.m[2][(i + 2) % 4] * m.m[3][(i + 3) % 4] - m.m[1][(i + 2) % 4] * m.m[2][(i + 1) % 4] * m.m[3][(i + 3) % 4]));
	}
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = (
				(m.m[(i + 1) % 4][(j + 1) % 4] *
				m.m[(i + 2) % 4][(j + 2) % 4] *
				m.m[(i + 3) % 4][(j + 3) % 4]) -
			    (m.m[(i + 1) % 4][(j + 3) % 4] *
				m.m[(i + 2) % 4][(j + 2) % 4] *
				m.m[(i + 3) % 4][(j + 1) % 4])) /
			                       det;
		}
	}
	return result;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	for (int i= 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MakeIdenity4x4() {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j) {
				result.m[i][j] = 1.0f;
			} else {
				result.m[i][j] = 0.0f;
			}
		}
	}
	return result;
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {

	for (int row = 0; row < 4; ++row) {
		for (int colum = 0; colum < 4; ++colum) {
			Novice::ScreenPrintf(x + colum * kColumnWidth, y + (row+1) * kRowHeight, "%3.02f", matrix.m[row][colum]);
		}
	}

	Novice::ScreenPrintf(x, y, "%s", label);

}
