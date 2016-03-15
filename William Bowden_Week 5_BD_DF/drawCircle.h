#pragma once
#include "vec2.h"

vec2 circle[] =
{
	vec2(1, 0),
	vec2(.7, .7),
	vec2(0, 1),
	vec2(-.7, .7),
	vec2(-1, 0),
	vec2(-.7, -.7),
	vec2(0, -1),
	vec2(.7, -.7),
	vec2(1, 0)
};

const int CIRCLE_POINTS = sizeof(circle) / sizeof(circle[0]);