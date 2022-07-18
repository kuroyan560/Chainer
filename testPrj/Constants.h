#pragma once
#include"Vector2.h"

static class Constants
{
public:
	static const int WIN_WIDTH = 1280;
	static const int WIN_HEIGHT = 720;
	static const float GRAVITY;
	static Vector2<float> GetWinCenter()
	{
		return Vector2<float>{(float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2};
	}
	static Vector2<int>GetWinSize()
	{
		return Vector2<int>{WIN_WIDTH, WIN_HEIGHT};
	}
};
