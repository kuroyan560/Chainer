#include "New_Goal.h"
#include"New_Player.h"
#include"Collision.h"
#include"DxLib.h"

New_Goal::New_Goal()
{
	graph = LoadGraph("_resource/graphics/goal.png");
}

void New_Goal::HitCheck(Vector2<float> OpeWorldPos, Vector2<float> CpuWorldPos)
{
	Vector2<float>Center = { (float)pos.x + SIZE / 2,(float)pos.y + SIZE / 2 };
	if (Collision::BoxAndBox(Center, SIZE, OpeWorldPos, New_Player::SIZE)
		/*|| Collision::BoxAndBox(Center, SIZE, CpuWorldPos, New_Player::SIZE)*/)
	{
		flag = true;
	}
}

void New_Goal::Draw(CamAccessor* Cam)
{
	float X1 = Cam->CamAffectX(pos.x);
	float Y1 = Cam->CamAffectY(pos.y);
	float X2 = X1 + Cam->CamZoomAffect(SIZE);
	float Y2 = Y1 + Cam->CamZoomAffect(SIZE);
	DrawExtendGraph(X1, Y1, X2, Y2, graph, true);
}
