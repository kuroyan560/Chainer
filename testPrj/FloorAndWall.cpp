#include"FloorAndWall.h"
#include"DxLib.h"

int FloorAndWall::FLOOR_GRAPH = 0;
int FloorAndWall::WALL_GRAPH = 0;

float FloorAndWall::LocalX1(const int num)
{
	return camPtr->CamAffectX(floorPos[num].x);
}

float FloorAndWall::LocalX2(const int num)
{
	return camPtr->CamAffectX(floorPos[num].x + 1280);
}

FloorAndWall::FloorAndWall(CamAccessor* Cam) :camPtr{ Cam }
{
	if (!FLOOR_GRAPH)
	{
		FLOOR_GRAPH = LoadGraph("_resource/graphics/floor.png");
		WALL_GRAPH = LoadGraph("_resource/graphics/wall.png");
	}
}

void FloorAndWall::Init(Map* map)
{
	floorPos[0].x = map->GetMapX();
	floorPos[1].x = map->GetMapX() + 1280;
	floorPos[0].y = map->GetMapY() - 64;
	floorPos[1].y = map->GetMapY() - 64;
	floorDraw = false;

	wallPos[0].x = 0 - 320;
	wallPos[1].x = 0 - 320;
	wallPos[0].y = 0;
	wallPos[1].y = 736;
	wallDraw = true;
}

void FloorAndWall::Update()
{
	if (!floorDraw)
	{
		if (LocalX1(0) < 1280)
		{
			floorDraw = true;
		}
	}

	if (wallDraw)
	{
		if (camPtr->CamAffectX(wallPos[0].x + 320) < 0)
		{
			wallDraw = false;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (LocalX2(i) < 0)
		{
			floorPos[i].x += 1280 * 2;
		}

		if (camPtr->CamAffectY(wallPos[i].y + 736) < 0)
		{
			wallPos[i].y += 736 * 2;
		}
		if (720 < camPtr->CamAffectY(wallPos[i].y))
		{
			wallPos[i].y -= 736 * 2;
		}
	}
}

void FloorAndWall::Draw()
{
	if (floorDraw)
	{
		for (int i = 0; i < 2; i++)
		{
			DrawGraph(LocalX1(i), camPtr->CamAffectY(floorPos[i].y), FLOOR_GRAPH, true);
		}
	}
	if (wallDraw)
	{
		for (int i = 0; i < 2; i++)
		{
			DrawGraph(camPtr->CamAffectX(wallPos[i].x), camPtr->CamAffectY(wallPos[i].y), WALL_GRAPH, true);
		}
	}
}

void Floor::Init()
{
	floorPos[0].x = 0;
	floorPos[1].x = 0 + 1280;
	floorPos[0].y = 720 - 64;
	floorPos[1].y = 720 - 64;
	floorDraw = true;
}

void Floor::Draw()
{
	if (floorDraw)
	{
		for (int i = 0; i < 2; i++)
		{
			DrawGraph(LocalX1(i), camPtr->CamAffectY(floorPos[i].y), FLOOR_GRAPH, true);
		}
	}
}
