#include "New_Map.h"
#include"DxLib.h"
#include<algorithm>
#include"New_MapChip.h"
#include"MyFunc.h"
#include"New_ItemMgr.h"

int New_Map::MAP_NUM = 0;
int New_Map::EASY_MAP_NUM = 0;

New_Map::New_Map()
{
}

New_Map::~New_Map()
{
}

bool New_Map::CheckOutOfRangeChip(int x, int y)const
{
	if (y < 0 || chipNumY <= y || x < 0 || chipNumX <= x)
	{
		return true;
	}
	return false;
}

bool New_Map::CheckBlockChip(int chipData)const
{
	if (chipData == Info_MapChip::TYPE::WALL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void New_Map::Load(const char* CSVPass)
{
	if (!initMapData.empty())
	{
		initMapData.clear();
	}

	FILE* fp;

	fopen_s(&fp, CSVPass, "r");

	if (fp == NULL)
	{
		MessageBox(NULL, "CSV�ǂݍ��݂Ɏ��s���܂���", "", MB_OK);
		DebugBreak();
	}
	else
	{
		initMapData.push_back(std::vector<char>());
		char c = fgetc(fp);
		do
		{
			if (c == '\n')
			{
				initMapData.push_back(std::vector<char>());
			}
			else if (c != ',')
			{
				initMapData.back().push_back(c);
			}
		} while ((c = fgetc(fp)) != EOF);
	}
	fclose(fp);

	initMapData.pop_back();

	chipNumY = initMapData.size();	//�c���̗���擾
	chipNumX = initMapData.front().size();	//�����̗���擾

	bool a;
	a = std::all_of(initMapData.begin(), initMapData.end(), [=](std::vector<char>& vec)
		{
			return vec.size() == chipNumX;	//������������
		});
	if (!a)	//�������Ⴄ�z�񂪂�������G���[
	{
		MessageBox(NULL, "�s���ȃ}�b�v�ł��B���������킹��", "", MB_OK);
		DebugBreak();
	}
}

Vector2<float> New_Map::Init(New_Goal* GoalObj)
{
	Vector2<float> PlayerInitPos = NONE_VECTOR2;
	Vector2<int>GoalPos = { 0,0 };
	if (!mapData.empty())
	{
		mapData.clear();
	}

	for (int y = 0; y < chipNumY; y++)
	{
		mapData.push_back(std::vector<int>());
		for (int x = 0; x < chipNumX; x++)
		{
			int num = MyFunc::ctoi(initMapData[y][x]);

			if (num == Info_MapChip::TYPE::ITEM)	//�A�C�e���̃Z�b�g
			{
				New_ItemMgr::AddItem(x * CHIP_SIZE, y * CHIP_SIZE);
				mapData.back().push_back(Info_MapChip::TYPE::NONE);
			}
			else if (num == Info_MapChip::TYPE::PLAYER_POS)		//�v���C���[�̏������ʒu�Z�b�g�i�Z���^�[�|�X�j
			{
				PlayerInitPos.x = x * CHIP_SIZE;
				PlayerInitPos.y = y * CHIP_SIZE;
				mapData.back().push_back(Info_MapChip::TYPE::NONE);
			}
			else if (num == Info_MapChip::TYPE::GOAL_POS && GoalObj != nullptr)
			{
				GoalObj->pos = { x * CHIP_SIZE,y * CHIP_SIZE };
				GoalPos = GoalObj->pos;
				GoalObj->flag = false;
				mapData.back().push_back(Info_MapChip::TYPE::NONE);
			}
			else	//�}�b�v�`�b�v�Z�b�g
			{
				mapData.back().push_back(num);
			}
		}
	}

	if (PlayerInitPos == NONE_VECTOR2)
	{
		MessageBox(NULL, "�v���C���[�̏������ʒu�A����Y��Ă�ŁB", "", MB_OK);
	}
	if (GoalPos == Vector2<int>{0, 0}&& GoalObj != nullptr)
	{
		MessageBox(NULL, "�S�[���̈ʒu�A����Y��Ă�ŁB", "", MB_OK);
	}

	return PlayerInitPos;
}

void New_Map::Draw(const CamAccessor& Cam)
{
	DrawLine(0, Cam.CamAffectY(GetHeight()+ LINE_THICKNESS/2),
		Constants::WIN_WIDTH, Cam.CamAffectY(GetHeight()+ LINE_THICKNESS/2), GetColor(207, 17, 17), Cam.CamZoomAffect(LINE_THICKNESS));
	float X1, Y1, X2, Y2;
	for (int y = 0; y < chipNumY; y++)
	{
		for (int x = 0; x < chipNumX; x++)
		{
			X1 = Cam.CamAffectX(x * CHIP_SIZE);
			Y1 = Cam.CamAffectY(y * CHIP_SIZE);
			X2 = Cam.CamAffectX((x + 1) * CHIP_SIZE);
			Y2 = Cam.CamAffectY((y + 1) * CHIP_SIZE);

			if (MyFunc::InnerWin(X1, Y1, X2, Y2))
			{
				chip.Draw(mapData[y][x], X1, Y1, X2, Y2);
			}
		}
	}
}

Vector2<float> New_Map::PlayerInitPos()const
{
	float mapWidth = chipNumX * CHIP_SIZE;
	float mapHeight = chipNumY * CHIP_SIZE;
	float floorHeight = CHIP_SIZE * 2;

	return Vector2<float>{mapWidth / 2, mapHeight - floorHeight};

}


int New_Map::GetWidth()const
{
	return chipNumX * CHIP_SIZE;
}

int New_Map::GetHeight()const
{
	return chipNumY * CHIP_SIZE;
}

Vector2<float> New_Map::CheckHitWall(Vector2<float> Pos)
{
	int x = Pos.x / CHIP_SIZE;
	int y = Pos.y / CHIP_SIZE;
	if (!CheckOutOfRangeChip(x, y) && mapData[y][x] == Info_MapChip::TYPE::WALL)	//�u���b�N�Ƃ̔���
	{
		return Vector2<float>{(float)x* CHIP_SIZE, (float)y* CHIP_SIZE};
	}
	return NONE_VECTOR2;
}
bool New_Map::CheckHitDamage(Vector2<float> Pos)
{
	int x = Pos.x / CHIP_SIZE;
	int y = Pos.y / CHIP_SIZE;
	if (!CheckOutOfRangeChip(x, y) && mapData[y][x] == Info_MapChip::TYPE::DAMAGE)	//�u���b�N�Ƃ̔���
	{
		return true;
	}
	return false;
}
