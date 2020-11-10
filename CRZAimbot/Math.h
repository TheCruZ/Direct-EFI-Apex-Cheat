#pragma once
#include <math.h>
#include "vector.h"


namespace Math {
	void NormalizeAngles(Vector& angle);
	double GetFov(const Vector& viewAngle, const Vector& aimAngle, float distance);
	double GetFov2(const Vector& viewAngle, const Vector& aimAngle);
	Vector CalcAngle(const Vector& src, const Vector& dst);
}