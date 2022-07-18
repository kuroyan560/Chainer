#pragma once
#include"Stage.h"
#include<vector>
#include"StageMgrAccessor.h"
#include"ChargeItemMgr.h"

class StageMgr :public StMgrForSelect, public StMgrForGame
{
private:
	static int STAGE_GRAPH;

	//ステージの配列
	std::vector<Stage>*stages;

	//クリアフラグ
	std::vector<bool>clearFlag;

	//選択中のステージ
	int nowStage;

	//ステージ名表記
	static const int TAG_GRAPH_WIDTH = 320;
	static const int NUM_GRAPH_WIDTH = 80;
	static const int TAG_DRAW_POSY = 320;
	static const float TAG_EASE_SPEED;
	Vector2<int>tagPos;
	int num[2];
	float stageTagEase;

	//チュートリアル用
	/*static const float MOVIE_SIZE;
	static int TUTORIAL_MOVIE[3];
	Vector2<float> tutorialMoviePos[3];
	const char* TUTORIAL_STRING[3];*/
	static LoopImage TUTORIAL_A;
	static LoopImage TUTORIAL_X;
	static LoopImage TUTORIAL_X_FAST;
	//static LoopImage TUTORIAL_STICK;
	static LoopImage TUTORIAL_RT;
	static Image TUTORIAL_ARROW;

public:
	StageMgr(std::vector<Stage>*Stages);
	~StageMgr() {};

	virtual void TagInit()override;
	virtual void TagDraw()override;

	//基本関数
	virtual void Set(EnemyMgr* EnemyMgr,ChargeItemMgr* ChargeItemMgr, CollectItemMgr* CollectItemMgr)override;
	virtual void Init()override;
	virtual void Update()override;
	virtual void Draw(CamAccessor* Cam)override;
	virtual void Finalize()override;

	//セッタ
	virtual void SetNowStage(int NowStage)override { nowStage = NowStage; }				//セレクトシーン向け

	//ゲッタ
	virtual int MaxStageNum()const override { return stages->size(); }					//セレクトシーン向け
	virtual bool GetItemFlag(int StageNum)const override { return *stages->at(nowStage).GetCollect(); }	//セレクトシーン向け
	virtual bool GetClearFlag(int StageNum)const override { return clearFlag[StageNum]; }	//セレクトシーン向け
	virtual bool SearchStage(int StageNum)const override;	//セレクトシーン向け

	virtual bool CanGoToExtra()const override;

	virtual int GetCollectItemNum(int StageNum)const override { return stages->at(nowStage).HowManyCollectGet(); }
	virtual int MaxCollectItemNum(int StageNum)const override { return stages->at(nowStage).GetCollectNum(); }

	virtual Map* GetMapData()const override { return stages->at(nowStage).GetMapData(); }
	virtual Vector2<float>GetPlayerInitPos()const override { return stages->at(nowStage).GetPlayerInitPos(); }
	virtual int GetMapHeight()const override { return stages->at(nowStage).GetMapData()->GetMapY(); }

	virtual bool* GetCollect()override { return stages->at(nowStage).GetCollect();}
	virtual Vector2<int>GetCollectPos()const override { return stages->at(nowStage).GetCollectPos(); }
	virtual int GetCollectNum()const override { return stages->at(nowStage).GetCollectNum(); }

	virtual void RaiseGoalFlag();

	bool CollectStage()override { return stages->at(nowStage).CollectStage(); }
	virtual int GetNowHaveCollect()const override{ return stages->at(nowStage).HowManyCollectGet(); }


	//エクストラ
	virtual bool NowExtraStage()const override;

	virtual void SetBestScore(int Score)override;
	virtual int GetBestScore()const override { return stages->at(nowStage).GetBestScore(); }
};