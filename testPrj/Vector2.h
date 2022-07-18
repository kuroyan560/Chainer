#pragma once
#include<math.h>
#include"DxLib.h"
#define NONE_VECTOR2 Vector2<float>{0.0,0.0}
template<typename T>
struct Vector2
{
	T x;
	T y;

	Vector2<T> operator()(const Vector2<T>& obj)
	{
		this->x = obj.x;
		this->y = obj.y;
	}

	Vector2<T> operator+(const Vector2<T>& obj)const
	{
		return Vector2<T>{ this->x + obj.x, this->y + obj.y };
	}

	Vector2<T>& operator+=(const Vector2<T>& obj)
	{
		*this = *this + obj;
		return *this;
	}

	Vector2<T> operator-(const Vector2<T>& obj)const
	{
		return Vector2<T>{this->x - obj.x, this->y - obj.y};
	}

	Vector2<T> operator-=(const Vector2<T>& obj)
	{
		*this = *this - obj;
		return *this;
	}

	Vector2<T> operator-()const
	{
		return Vector2<T>{-this->x, -this->y};
	}

	Vector2<T> operator*(const float& num)const
	{
		return Vector2<T>{this->x* num, this->y* num};
	}

	Vector2<T> operator*(const Vector2<T>& obj)
	{
		return Vector2<T>{this->x* obj.x, this->y* obj.y};
	}

	Vector2<T> operator/(const int& num)const
	{
		return Vector2<T>{this->x / num, this->y / num};
	}

	bool operator==(const Vector2<T>& obj)const
	{
		return (this->x == obj.x && this->y == obj.y);
	}

	bool operator!=(const Vector2<T>& obj)const
	{
		return (this->x != obj.x && this->y != obj.y);
	}

};

static float GetVecSize(Vector2<float> vector)
{
	double size = vector.x * vector.x + vector.y * vector.y;
	size = sqrt(size);

	return size;
}

static Vector2<float>CreateVector(Vector2<float>from, Vector2<float>to)
{
	Vector2<float>Vec;
	Vec.x = to.x - from.x;
	Vec.y = to.y - from.y;
	return Vec;
}

static Vector2<float>CreateVector(float Rad)
{
	Vector2<float>Vec;
	Vec.x = cos(Rad);
	Vec.y = sin(Rad);
	return Vec;
}

static float GetDistance(Vector2<float>from, Vector2<float>to)
{
	return GetVecSize(CreateVector(from,to));
}

static Vector2<float> GetVec(Vector2<float> vector)
{
	double size = GetVecSize(vector);
	Vector2<float>Vec = { vector.x / size,vector.y / size };
	return Vec;
}

static Vector2<float>GetVec2(Vector2<float>from, Vector2<float>to)
{
	return GetVec(CreateVector(from,to));
}

static float GetAngle(Vector2<float>Vec)
{
	Vector2<float>vec = GetVec(Vec);
	float angle = acos(vec.x);
	angle *= 180 / DX_PI;
	if (Vec.y < 0)angle = 360.0 - angle;
	return angle;
}

static float GetRad(Vector2<float>Vec)
{
	Vector2<float>vec = GetVec(Vec);
	float angle = acos(vec.x);
	if (Vec.y < 0)angle = DX_PI * 2 - angle;
	return angle;
}

//内積を求める
static float GetDot(Vector2<float>vec1, Vector2<float>vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

//外積を求める
static float GetCross(Vector2<float>vec1, Vector2<float>vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

//ベクトルを回転させる
static Vector2<float> SpinVector2(Vector2<float>Vec, float SpinRad, float Size = 1.0f)
{
	float rad = GetRad(Vec);
	Vector2<float>vec = CreateVector(rad + SpinRad);
	vec.x *= Size;
	vec.y *= Size;
	return vec;
}