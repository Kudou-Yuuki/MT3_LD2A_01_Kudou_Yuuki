
#pragma once
#include "../DirectXGame/math/Matrix4x4.h"
#include "../DirectXGame/math/Vector3.h"

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate);
