#pragma once
#include"Enemy.h"
#include<vector>
#include"Map.h"
#include"CollectItem.h"

class Stage
{
	//ステージに関する情報を格納
	//敵に関して
	int enemyNum;
	EnemyInfo* enemysInfo;

	//チャージアイテムに関して
	int cItemNum;
	Vector2<int>* cItemPos;

	//マップ情報
	Map* map;

	//ゴールの座標X
	int goalLine;

	//下限
	Vector2<float> playerInitPos;

	//収集アイテムを取ったか
	bool collect;
	Vector2<int> collectItemPos;

	//探索型
	int collectNum;
	CollectInfo* collectInfo;
	bool collectStage;

	//スコアアタック型
	int bestScore;

public:
	Stage(const int CollectNum, CollectInfo* CollectInfo, const int CItemNum, Vector2<int>* CItemPos, Map* Map, int GoalX);
	Stage(const int EnemyNum, EnemyInfo* EnemysInfo, const int CItemNum, Vector2<int>* CItemPos, Map* Map, int GoalX, Vector2<int>CollectItemPos);
	~Stage();

	//基本関数
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