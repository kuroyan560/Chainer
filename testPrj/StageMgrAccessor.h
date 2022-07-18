#pragma once
#include"Enemy.h"
#include"Map.h"
#include"EnemyMgr.h"
#include"ChargeItemMgr.h"
#include"CollectItemMgr.h"
//シーンごとに用意されたステージマネージャーへのアクセスのための窓口インターフェース

class StMgrForSelect
{
public:
	virtual ~StMgrForSelect() {};

	//選択されたステージ番号セッタ
	virtual void SetNowStage(int NowStage) = 0;

	//ステージの数ゲッタ
	virtual int MaxStageNum()const = 0;
	//そのステージのアイテムは取ったかゲッタ
	virtual bool GetItemFlag(int StageNum)const = 0;
	//そのステージはクリア済かどうかゲッタ
	virtual bool GetClearFlag(int StageNum)const = 0;

	//そのステージは探索用ステージかどうかゲッタ
	virtual bool SearchStage(int StageNum)const = 0;
	virtual int GetCollectItemNum(int StageNum)const = 0;
	virtual int MaxCollectItemNum(int StageNum)const = 0;

	virtual bool CanGoToExtra()const = 0;

	virtual int GetBestScore()const = 0;
};

class StMgrForGame
{
public:
	virtual ~StMgrForGame() {};

	virtual void TagInit() = 0;
	virtual void TagDraw() = 0;

	//基本関数
	virtual void Set(EnemyMgr* EnemyMgr, ChargeItemMgr* ChargeItemMgr, CollectItemMgr* CollectItemMgr) = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw(CamAccessor* Cam) = 0;
	virtual void Finalize() = 0;

	virtual Map* GetMapData()const = 0;
	virtual Vector2<float> GetPlayerInitPos()const = 0;
	virtual int GetMapHeight()const = 0;
	virtual bool* GetCollect() = 0;
	virtual Vector2<int> GetCollectPos()const = 0;
	virtual int GetCollectNum()const = 0;

	virtual void RaiseGoalFlag() = 0;

	virtual bool CollectStage() = 0;

	virtual int GetNowHaveCollect()const = 0;

	virtual bool NowExtraStage()const = 0;

	virtual void SetBestScore(int Score) = 0;
};