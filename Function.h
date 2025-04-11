#pragma once

#include "../DirectXGame/math/Matrix4x4.h"

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Inverse(const Matrix4x4& m);
Matrix4x4 Transpose(const Matrix4x4& v);
Matrix4x4 MakeIdenity4x4();
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

