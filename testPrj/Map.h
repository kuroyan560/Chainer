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

	//�Q�b�^
	float WorldX1(int ChipX);
	float WorldY1(int ChipY);
	float WorldX2(int ChipX);
	float WorldY2(int ChipY);
	int GetMapX();
	int GetMapY();

	void Draw(CamAccessor* Cam);

	//�}�b�v�`�b�v�̔ԍ��擾
	int GetChipNumX(float PosX);
	int GetChipNumY(float PosY);

	//�����_�ɂ�����}�b�v�`�b�v�Ƃ̓����蔻��
	bool GetHitCheckWall(Vector2<float> Pos);
};