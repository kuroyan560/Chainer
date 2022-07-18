#pragma once
#define CHIP_SIZE 32

namespace Info_MapChip
{
	const enum TYPE
	{
		NONE, WALL, DAMAGE, ITEM, PLAYER_POS, GOAL_POS, TYPE_NUM
	};
}

class New_MapChip
{
	static const int NOT_CHIP_GRAPH_NUM = 3;
	int graph[Info_MapChip::TYPE::TYPE_NUM - NOT_CHIP_GRAPH_NUM];
public:
	New_MapChip();
	~New_MapChip() {};
	void Draw(int ChipData, float X1, float Y1,float X2,float Y2);
};