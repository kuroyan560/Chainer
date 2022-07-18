#include "Stage.h"
#include"Constants.h"

//デバッグ用
#include"DxLib.h"

Stage::Stage(const int CollectNum, CollectInfo* CollectInfo, const int CItemNum, Vector2<int>* CItemPos, Map* Map, int GoalX)
{
	collectNum = CollectNum;
	collectInfo = CollectInfo;
	cItemNum = CItemNum;
	cItemPos = CItemPos;
	map = Map;
	goalLine = GoalX;
	playerInitPos.x = 128;
	playerInitPos.y = map->GetMapY() - 128 * 3;

	enemyNum = 0;
	enemysInfo = nullptr;
	collectStage = true;
}

Stage::Stage(const int EnemyNum, EnemyInfo* EnemysInfo, const int CItemNum, Vector2<int>* CItemPos, Map* Map, int GoalX, Vector2<int>CollectItemPos)
{
	enemyNum = EnemyNum;
	enemysInfo = EnemysInfo;
	cItemNum = CItemNum;
	cItemPos = CItemPos;
	map = Map;
	goalLine = GoalX;
	playerInitPos.x = 128;
	playerInitPos.y = map->GetMapY() - 128 * 3;
	collect = false;
	collectItemPos = CollectItemPos;

	collectNum = 0;
	collectInfo = nullptr;
	collectStage = false;

	bestScore = 0;
}

Stage::~Stage()
{
}

void Stage::Draw(CamAccessor* Cam)
{
	//デバッグ用
	//マップチップ基準線
	//for (int x = 0; x < goalLine; x += 32)
	//{
	//	DrawLine(Cam->CamAffectX(x), 0, Cam->CamAffectX(x), 720, GetColor(20, 20, 20), 1);
	//	DrawFormatString(Cam->CamAffectX(x), 0, GetColor(30, 30, 30), "%d", x / 32);
	//}
	//for (int y = 0; y < map->GetMapY(); y += 32)
	//{
	//	DrawLine(0, Cam->CamAffectY(y), 1280, Cam->CamAffectY(y), GetColor(20, 20, 20), 1);
	//	DrawFormatString(0, Cam->CamAffectY(y), GetColor(30, 30, 30), "%d", y / 32);
	//}

	////ゴールライン
	DrawLine(Cam->CamAffectX(goalLine), 0, Cam->CamAffectX(goalLine), 720, GetColor(92, 220, 75), 3);

	////高さの基準線
	//DrawLine(0, Cam->CamAffectY(720 * 2), 1280, Cam->CamAffectY(720 * 2), GetColor(255, 255, 255), 1);
	//DrawLine(0, Cam->CamAffectY(0), 1280, Cam->CamAffectY(0), GetColor(255, 255, 255), 1);
	//DrawLine(Cam->CamAffectX(0), 0, Cam->CamAffectX(0), 720, GetColor(255, 255, 255),1);
	//DrawLine(0, Cam->CamAffectY(0), 1280, Cam->CamAffectY(0), GetColor(255, 0, 0), 3);
	//DrawLine(0, Cam->CamAffectY(720 / 2), 1280, Cam->CamAffectY(720 / 2), GetColor(255, 0, 0), 1);
	//DrawLine(0, Cam->CamAffectY(720 / 2 + 720), 1280, Cam->CamAffectY(720 / 2 + 720), GetColor(255, 0, 0), 1);

	map->Draw(Cam);
}

void Stage::Finalize()
{
}

int Stage::HowManyCollectGet()
{
	int num = 0;
	for (int i = 0; i < collectNum; i++)
	{
		if ((collectInfo + i)->get == true)
		{
			num++;
		}
	}
	return num;
}

bool Stage::CollectComplete()
{
	bool a = true;
	for (int i = 0; i < collectNum; i++)
	{
		if ((collectInfo + i)->get == false)
		{
			a = false;
		}
	}
	return a;
}

