#include "Map.h"
#include"DxLib.h"
#include"Constants.h"

int Map::CHIP_GRAPH[CHIP_TYPE_MAX] = { 0 };

void Map::Load()
{
	LoadDivGraph("_resource/graphics/mapchip.png", CHIP_TYPE_MAX, CHIP_TYPE_MAX, 1, CHIP_SIZE, CHIP_SIZE, CHIP_GRAPH);
}

Map::Map(Vector2<int> ChipNum, char* MapData)
{
	chipNum = ChipNum;
	mapData = MapData;
}

float Map::WorldX1(int ChipX)
{
	return CHIP_SIZE * ChipX;
}

float Map::WorldY1(int ChipY)
{
	return CHIP_SIZE * ChipY;
}

float Map::WorldX2(int ChipX)
{
	return WorldX1(ChipX) + CHIP_SIZE;
}

float Map::WorldY2(int ChipY)
{
	return WorldY1(ChipY) + CHIP_SIZE;
}

int Map::GetMapX()
{
	return (chipNum.x - 1) * CHIP_SIZE;
}

int Map::GetMapY()
{
	return (chipNum.y - 1) * CHIP_SIZE;
}

void Map::Draw(CamAccessor* Cam)
{
	for (int y = 0; y < chipNum.y; y++)
	{
		for (int x = 0; x < chipNum.x; x++)
		{
			if (-CHIP_SIZE < Cam->CamAffectX(WorldX1(x)) && Cam->CamAffectX(WorldX2(x)) < Constants::WIN_WIDTH + CHIP_SIZE
				&& -CHIP_SIZE < Cam->CamAffectY(WorldY1(y)) && Cam->CamAffectY(WorldY2(y)) < Constants::WIN_HEIGHT + CHIP_SIZE)
			{
				//マップチップの描画
				DrawGraph(Cam->CamAffectX(WorldX1(x)), Cam->CamAffectY(WorldY1(y)), CHIP_GRAPH[*(mapData + y * chipNum.x + x)], true);
			}
		}
	}
}

char Map::GetMapData(Vector2<float> Pos)
{
	int x = Pos.x / CHIP_SIZE;
	int y = Pos.y / CHIP_SIZE;

	if (0 <= x && x < chipNum.x && 0 <= y && y < chipNum.y)
	{
		return *(mapData + y * chipNum.x + x);
	}

	return 0;
}

int Map::GetChipNumX(float PosX)
{
	return (int)(PosX / CHIP_SIZE);
}

int Map::GetChipNumY(float PosY)
{
	return (int)(PosY / CHIP_SIZE);
}

bool Map::GetHitCheckWall(Vector2<float> Pos)
{
	if (GetMapData(Pos) == 0)
	{
		return false;
	}
	return true;;
}
