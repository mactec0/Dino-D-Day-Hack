#pragma once  
#include <cmath> 

#define DEG2RAD(a) a*(M_PI/180)
#define RAD2DEG(a) a*(180/M_PI)
#define M_PI 3.14159265358979323846

class Vector
{
public:
	float x, y, z;
	inline void Init(float ix, float iy, float iz)
	{
		x = ix; y = iy; z = iz;
	}
	Vector() { x = 0; y = 0; z = 0; };
	Vector(float X, float Y, float Z) { x = X; y = Y; z = Z; };
	Vector(float *vec) { x = vec[0]; y = vec[1]; z = vec[2]; };
	float operator[](int i) const { if (i == 1) return x; if (i == 2) return y; return z; };
	float& operator[](int i) { if (i == 1) return x; if (i == 2) return y; return z; };

	bool operator==(const Vector& v) { return true; }
	bool operator!=(const Vector& v) { return true; }

	inline Vector operator-(const Vector& v) { return Vector(x - v.x, y - v.y, z - v.z); }
	inline Vector operator+(const Vector& v) { return Vector(x + v.x, y + v.y, z + v.z); }
	inline Vector operator*(const float n) { return Vector(x*n, y*n, z*n); }
	inline Vector operator-() { return Vector(-x, -y, -z); }
	float LengthSqr(void) { return (x*x + y * y + z * z); }
	float Length(void) { return (x*x + y * y + z * z); }
	float Length2D(void) { return sqrtf(x*x + y * y); }
	float VectorLength(void) { return sqrt(x*x + y * y + z * z); }
	inline Vector& operator+=(const Vector &v) { x += v.x; y += v.y; z += v.z; return *this; }
	inline Vector& operator-=(const Vector &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	inline Vector& operator/=(const Vector &v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
	inline Vector& operator/=(const float v) { x /= v; y /= v; z /= v; return *this; }
	inline Vector& operator*=(const float v) { x *= v; y *= v; z *= v; return *this; }
	void NormalizeInPlace() {
		Vector& v = *this;
		float iradius = 1.f / (this->VectorLength() + FLT_EPSILON);  
		v *= iradius;
	}
	inline Vector Cross(Vector a) { return Vector(y*a.z - z * a.y, z*a.x - x * a.z, x*a.y - y * a.x); }
	inline Vector Up() {
		float sp, sy, sr, cp, cy, cr;
		sp = sin(DEG2RAD(x));
		cp = cos(DEG2RAD(x));

		sy = sin(DEG2RAD(y));
		cy = cos(DEG2RAD(y));

		sr = sin(DEG2RAD(z));
		cr = cos(DEG2RAD(z));

		return Vector(cr*sp*cy + -sr * -sy, cr*sp*sy + -sr * cy, cr*cp);
	}
	inline Vector Forward() {
		float sp, sy, cp, cy;

		sp = sin(DEG2RAD(x));
		cp = cos(DEG2RAD(x));

		sy = sin(DEG2RAD(y));
		cy = cos(DEG2RAD(y));

		return Vector(cp*cy, cp*sy, -sp);
	}
	inline Vector Right() {
		float sp, sy, sr, cp, cy, cr;

		sp = sin(DEG2RAD(x));
		cp = cos(DEG2RAD(x));

		sy = sin(DEG2RAD(y));
		cy = cos(DEG2RAD(y));

		sr = sin(DEG2RAD(z));
		cr = cos(DEG2RAD(z));

		return Vector(1 * sr*sp*cy + -1 * cr*-sy, -1 * sr*sp*sy + -1 * cr*cy, -1 * sr*cr);
	}

	Vector toAngles() {
		Vector angles;
		float tmp, yaw, pitch;
		if (y == 0 && x == 0) {
			yaw = 0;
			if (z > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else {
			yaw = (atan2(y, x) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(x*x + y * y);
			pitch = (atan2(-z, tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles.x = pitch;
		angles.y = yaw;
		angles.z = 0;
		return angles;
	}

};

