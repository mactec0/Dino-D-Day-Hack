#pragma once
#include <math.h>
#include "vector.h"
#include <cmath> 

float Dist3D(Vector start, Vector end) {
	return hypot(hypot(start.x - end.x, start.y - end.y), start.z - end.z);
}


Vector CalcAngle(Vector src, Vector dst) {
	Vector delta, vAngle;
	delta.x = src.x - dst.x;
	delta.y = src.y - dst.y;
	delta.z = src.z - dst.z;
	double hyp = delta.Length2D();

	vAngle.x = (float)(atan(delta.z / hyp) * 57.295779513082f);
	vAngle.y = (float)(atan(delta.y / delta.x) * 57.295779513082f);
	vAngle.z = 0.0f;

	if (delta.x >= 0.0)
		vAngle.y += 180.0f;
	return vAngle;
}



void Normalize(Vector &angles) {
	if (isnan(angles.x) || isinf(angles.x))
		angles.x = 0;

	if (isnan(angles.y) || isinf(angles.y))
		angles.y = 0;

	if (isnan(angles.z) || isinf(angles.z))
		angles.z = 0;

	if (angles.x > 89)angles.x = 89;
	if (angles.x < -89)angles.x = -89;
	while (angles.y > 180)angles.y -= 360;
	while (angles.y < -180)angles.y += 360;

	angles.z = 0;
}


void CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove) {
	Vector viewangles = pCmd->viewangles;
	float f1;
	float f2;

	Normalize(viewangles);
	float deltaView = viewangles.y - vOldAngles.y;

	if (vOldAngles.y >= 0.f)
		f1 = vOldAngles.y;
	else
		f1 = 360.0f + vOldAngles.y;

	if (viewangles.y >= 0.0f)
		f2 = viewangles.y;
	else
		f2 = 360.0f + viewangles.y;

	if (f2 >= f1)
		deltaView = 360.0f - abs(f1 - f2);
	else
		deltaView = abs(f2 - f1);

	deltaView = 360.0f - deltaView;

	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->fowardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;

}


bool WorldToScreen(Vector In, Vector& Out) {
	Matrix3x4_t ViewMatrix = EngineClient->GetMatrix();

	Out.x = ViewMatrix.Matrix[0] * In.x + ViewMatrix.Matrix[1] * In.y + ViewMatrix.Matrix[2] * In.z + ViewMatrix.Matrix[3];

	Out.y = ViewMatrix.Matrix[4] * In.x + ViewMatrix.Matrix[5] * In.y + ViewMatrix.Matrix[6] * In.z + ViewMatrix.Matrix[7];

	Out.z = ViewMatrix.Matrix[12] * In.x + ViewMatrix.Matrix[13] * In.y + ViewMatrix.Matrix[14] * In.z + ViewMatrix.Matrix[15];

	if (Out.z < 0.01f)
		return false;

	float Inverse = 1.f / Out.z;

	Out.x *= Inverse;
	Out.y *= Inverse;

	int Width, Height;

	EngineClient->GetScreenSize(Width, Height);

	auto X = Width / 2;
	auto Y = Height / 2;

	X += 0.5 * Out.x * Width + 0.5;
	Y -= 0.5 * Out.y * Height + 0.5;

	Out.x = X;
	Out.y = Y;

	return true;
}
