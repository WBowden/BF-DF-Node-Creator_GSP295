#pragma once
#include <Windows.h>

struct vec2
{
	float x, y;
	vec2() : x(0), y(0){}
	vec2(float x, float y) : x(x), y(y){}
	void screenCursorMove()
	{
		COORD c = { (short)x, (short)y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	}

	void operator += (const vec2 & v) { x += v.x; y += v.y; }
	void operator -= (const vec2 & v) { x -= v.x; y -= v.y; }
	vec2 operator + (const vec2 & v) { return vec2(x + v.x, y + v.y); }
	vec2 operator - (const vec2 & v) { return vec2(x - v.x, y - v.y); }

	vec2 operator * (const float f) { return vec2(x * f, y * f); }
	vec2 operator / (const float f) { return vec2(x / f, y / f); }
	void operator *= (const float f) { x *= f; y *= f; }
	void operator /= (const float f) { x /= f; y /= f; }
};