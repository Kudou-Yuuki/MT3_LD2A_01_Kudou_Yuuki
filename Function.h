#pragma once  
#include "../External/KamataEngine/include/math/Vector3.h"  

using KamataEngine::Vector3;  

Vector3 Add(const Vector3& v1, const Vector3& v2);  
Vector3 Subtract(const Vector3& v1, const Vector3& v2);  
Vector3 Multiply(float scalar, const Vector3& v);  
float Dot(const Vector3& v1, const Vector3& v2);  
float Length(const Vector3& v);  
Vector3 Normalize(const Vector3& v);  
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
