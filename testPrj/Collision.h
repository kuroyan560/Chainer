#pragma once
#include"Vector2.h"
static class Collision
{
public:
	static bool LineAndLine(Vector2<float>From_1, Vector2<float>To_1, Vector2<float>From_2, Vector2<float>To_2);
	static bool BoxAndBox(Vector2<float>Pos1, int Size1, Vector2<float>Pos2, int Size2); //íÜêSç¿ïW
};

