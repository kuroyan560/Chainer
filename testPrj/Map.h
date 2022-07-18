#pragma once
#include"Vector2.h"
#include"CamAccessor.h"

class Map
{
private:
	static const int CHIP_TYPE_MAX = 3;
	static int CHIP_GRAPH[CHIP_TYPE_MAX];

	Vector2<int> chipNum;
	char* mapData;
	char GetMapData(Vector2<float> Pos);

public:
	static const int CHIP_SIZE = 32;

	static void Load();

	Map(Vector2<int>ChipNum, char* MapData);
	~Map() {};

	//ゲッタ
	float WorldX1(int ChipX);
	float WorldY1(int ChipY);
	float WorldX2(int ChipX);
	float WorldY2(int ChipY);
	int GetMapX();
	int GetMapY();

	void Draw(CamAccessor* Cam);

	//マップチップの番号取得
	int GetChipNumX(float PosX);
	int GetChipNumY(float PosY);

	//ある一点におけるマップチップとの当たり判定
	bool GetHitCheckWall(Vector2<float> Pos);
};