#pragma once
#include"Enemy.h"
#include<vector>
#include"Map.h"
#include"CollectItem.h"

class Stage
{
	//�X�e�[�W�Ɋւ�������i�[
	//�G�Ɋւ���
	int enemyNum;
	EnemyInfo* enemysInfo;

	//�`���[�W�A�C�e���Ɋւ���
	int cItemNum;
	Vector2<int>* cItemPos;

	//�}�b�v���
	Map* map;

	//�S�[���̍��WX
	int goalLine;

	//����
	Vector2<float> playerInitPos;

	//���W�A�C�e�����������
	bool collect;
	Vector2<int> collectItemPos;

	//�T���^
	int collectNum;
	CollectInfo* collectInfo;
	bool collectStage;

	//�X�R�A�A�^�b�N�^
	int bestScore;

public:
	Stage(const int CollectNum, CollectInfo* CollectInfo, const int CItemNum, Vector2<int>* CItemPos, Map* Map, int GoalX);
	Stage(const int EnemyNum, EnemyInfo* EnemysInfo, const int CItemNum, Vector2<int>* CItemPos, Map* Map, int GoalX, Vector2<int>CollectItemPos);
	~Stage();

	//��{�֐�
	void Draw(CamAccessor* Cam);
	void Finalize();

	int GetGoalLine()const { return goalLine; }
	Map* GetMapData()const { if (map != nullptr)return map; }
	Vector2<float>GetPlayerInitPos()const { return playerInitPos; }
	int GetEnemyNum()const { return enemyNum; }
	EnemyInfo* GetEnemysInfo()const { if (enemysInfo != nullptr)return enemysInfo; }
	int GetCItemNum()const { return cItemNum; }
	Vector2<int>* GetCItemPos()const { return cItemPos; }
	Vector2<int> GetCollectPos()const { return collectItemPos; }
	
	int GetBestScore()const { return bestScore; }
	void SetBestScore(int Score) { bestScore = Score; }

	bool CollectStage() { return collectStage; }
	int GetCollectNum() { return collectNum; }
	int HowManyCollectGet();
	bool* GetCollect() { return &collect; }
	CollectInfo* GetCollectInfo() { return collectInfo; }
	bool CollectComplete();
};