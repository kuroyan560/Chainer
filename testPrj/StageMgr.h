#pragma once
#include"Stage.h"
#include<vector>
#include"StageMgrAccessor.h"
#include"ChargeItemMgr.h"

class StageMgr :public StMgrForSelect, public StMgrForGame
{
private:
	static int STAGE_GRAPH;

	//�X�e�[�W�̔z��
	std::vector<Stage>*stages;

	//�N���A�t���O
	std::vector<bool>clearFlag;

	//�I�𒆂̃X�e�[�W
	int nowStage;

	//�X�e�[�W���\�L
	static const int TAG_GRAPH_WIDTH = 320;
	static const int NUM_GRAPH_WIDTH = 80;
	static const int TAG_DRAW_POSY = 320;
	static const float TAG_EASE_SPEED;
	Vector2<int>tagPos;
	int num[2];
	float stageTagEase;

	//�`���[�g���A���p
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

	//��{�֐�
	virtual void Set(EnemyMgr* EnemyMgr,ChargeItemMgr* ChargeItemMgr, CollectItemMgr* CollectItemMgr)override;
	virtual void Init()override;
	virtual void Update()override;
	virtual void Draw(CamAccessor* Cam)override;
	virtual void Finalize()override;

	//�Z�b�^
	virtual void SetNowStage(int NowStage)override { nowStage = NowStage; }				//�Z���N�g�V�[������

	//�Q�b�^
	virtual int MaxStageNum()const override { return stages->size(); }					//�Z���N�g�V�[������
	virtual bool GetItemFlag(int StageNum)const override { return *stages->at(nowStage).GetCollect(); }	//�Z���N�g�V�[������
	virtual bool GetClearFlag(int StageNum)const override { return clearFlag[StageNum]; }	//�Z���N�g�V�[������
	virtual bool SearchStage(int StageNum)const override;	//�Z���N�g�V�[������

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


	//�G�N�X�g��
	virtual bool NowExtraStage()const override;

	virtual void SetBestScore(int Score)override;
	virtual int GetBestScore()const override { return stages->at(nowStage).GetBestScore(); }
};