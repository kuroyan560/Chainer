#pragma once
#include<vector>
#include"BaseObject.h"
#include"New_MapChip.h"
#include"CamAccessor.h"
#include"New_Goal.h"

class New_Map
{
	const int LINE_THICKNESS = 16;
	std::vector<std::vector<char>>initMapData;
	std::vector<std::vector<int>>mapData;
	New_MapChip chip;

	int chipNumY;
	int chipNumX;

	//範囲外のチップか
	bool CheckOutOfRangeChip(int x, int y)const;
	//当たり判定をとるチップか
	bool CheckBlockChip(int chipData)const;

public:
	static int MAP_NUM;
	static int EASY_MAP_NUM;
	New_Map();
	~New_Map();

	void Load(const char* CSVPass);

	Vector2<float> Init(New_Goal* GoalObj);	//プレイヤー初期化位置を返す
	void Draw(const CamAccessor& Cam);

	//プレイヤーの初期位置計算
	Vector2<float>PlayerInitPos()const;

	//ある一点の位置にあるマップチップを調べる（壁ブロックならその左上座標を返す）
	Vector2<float>CheckHitWall(Vector2<float> Pos);
	bool CheckHitDamage(Vector2<float>Pos);

	int GetWidth()const;
	int GetHeight()const;
};