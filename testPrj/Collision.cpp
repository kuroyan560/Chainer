#include "Collision.h"

bool Collision::LineAndLine(Vector2<float>From_1, Vector2<float>To_1, Vector2<float>From_2, Vector2<float>To_2)
{
	Vector2<float>a, b, c;
	//Vec1メインのグループ
	a = To_2 - From_2;
	b = From_1 - From_2;
	c = To_1 - From_2;

	if (0 <= GetCross(a, b) * GetCross(a, c))
	{
		return false;
	}

	//Vec2メインのグループ
	a = To_1 - From_1;
	b = From_2 - From_1;
	c = To_2 - From_1;

	if (0 <= GetCross(a, b) * GetCross(a, c))
	{
		return false;
	}

	return true;
}

bool Collision::BoxAndBox(Vector2<float> Pos1, int Size1, Vector2<float> Pos2, int Size2)
{
	float X1_1 = Pos1.x - Size1 / 2;
	float Y1_1 = Pos1.y - Size1 / 2;
	float X2_1 = Pos1.x + Size1 / 2;
	float Y2_1 = Pos1.y + Size1 / 2;

	float X1_2 = Pos2.x - Size2 / 2;
	float Y1_2 = Pos2.y - Size2 / 2;
	float X2_2 = Pos2.x + Size2 / 2;
	float Y2_2 = Pos2.y + Size2 / 2;

	if (X2_1 < X1_2 || X2_2 < X1_1 || Y2_1 < Y1_2 || Y2_2 < Y1_1)
	{
		return false;
	}
	return true;
}
