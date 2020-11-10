#include "Math.h"

void Math::NormalizeAngles(Vector& angle)
{
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

Vector Math::CalcAngle(const Vector& src, const Vector& dst)
{
	Vector angle = Vector();
	Vector delta = Vector((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));

	float hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	angle.x = atan(delta.z / hyp) * (float)(180.0 / M_PI);
	angle.y = atan(delta.y / delta.x) * (float)(180.0 / M_PI);
	angle.z = 0;
	if (delta.x >= 0.0) angle.y += 180.0f;

	return angle;
}
#define DegToRad(val1) ((val1 * M_PI) / 180)
double Math::GetFov(const Vector& viewAngle, const Vector& aimAngle, float distance) {
	Vector delta = aimAngle - viewAngle;
	NormalizeAngles(delta);
	float pitch = (float)sin(DegToRad(delta.x)) * distance;
	float yaw = (float)sin(DegToRad(delta.y)) * distance;

	return sqrt(powf(pitch, 2.0) + powf(yaw, 2.0));
}
double Math::GetFov2(const Vector& viewAngle, const Vector& aimAngle)
{
	Vector delta = aimAngle - viewAngle;
	NormalizeAngles(delta);

	return sqrt(pow(delta.x, 2.0f) + pow(delta.y, 2.0f));
}
