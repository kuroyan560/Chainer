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

	//�͈͊O�̃`�b�v��
	bool CheckOutOfRangeChip(int x, int y)const;
	//�����蔻����Ƃ�`�b�v��
	bool CheckBlockChip(int chipData)const;

public:
	static int MAP_NUM;
	static int EASY_MAP_NUM;
	New_Map();
	~New_Map();

	void Load(const char* CSVPass);

	Vector2<float> Init(New_Goal* GoalObj);	//�v���C���[�������ʒu��Ԃ�
	void Draw(const CamAccessor& Cam);

	//�v���C���[�̏����ʒu�v�Z
	Vector2<float>PlayerInitPos()const;

	//�����_�̈ʒu�ɂ���}�b�v�`�b�v�𒲂ׂ�i�ǃu���b�N�Ȃ炻�̍�����W��Ԃ��j
	Vector2<float>CheckHitWall(Vector2<float> Pos);
	bool CheckHitDamage(Vector2<float>Pos);

	int GetWidth()const;
	int GetHeight()const;
};