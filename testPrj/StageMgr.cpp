#include "StageMgr.h"
#include"GrobalGraphs.h"

int StageMgr::STAGE_GRAPH = 0;
const float StageMgr::TAG_EASE_SPEED = 0.015F;
//const float StageMgr::MOVIE_SIZE = 0.25f;
//int StageMgr::TUTORIAL_MOVIE[3] = { 0 };
LoopImage StageMgr::TUTORIAL_A;
LoopImage StageMgr::TUTORIAL_X;
LoopImage StageMgr::TUTORIAL_X_FAST;
//LoopImage StageMgr::TUTORIAL_STICK;
LoopImage StageMgr::TUTORIAL_RT;
Image StageMgr::TUTORIAL_ARROW;

StageMgr::StageMgr(std::vector<Stage>* Stages)
{
	if (!STAGE_GRAPH)
	{
		STAGE_GRAPH = LoadGraph("_resource/graphics/stageName.png");

		/*TUTORIAL_MOVIE[0] = LoadGraph("_resource/movies/tutorial1.mp4");
		TUTORIAL_MOVIE[1] = LoadGraph("_resource/movies/tutorial2.mp4");
		TUTORIAL_MOVIE[2] = LoadGraph("_resource/movies/tutorial3.mp4");*/

		TUTORIAL_A.Init("_resource/graphics/press_a.png", true, 2, 25, { 2,1 }, { 128,128 });
		TUTORIAL_X.Init("_resource/graphics/press_x.png", true, 2, 25, { 2,1 }, { 128,128 });
		TUTORIAL_X_FAST.Init("_resource/graphics/press_x.png", true, 2, 10, { 2,1 }, { 128,128 });
		//TUTORIAL_STICK.Init("_resource/graphics/tilt_stick.png", true, 2, 25, { 2,1 }, { 128,128 });
		TUTORIAL_RT.Init("_resource/graphics/press_RT.png", true, 2, 25, { 2,1 }, { 128,128 });

		TUTORIAL_ARROW.Init("_resource/graphics/tutorial_arrow.png", true);
	}

	stages = Stages;

	//tutorialMoviePos[0] = { 560, 129 - 16 };
	//tutorialMoviePos[1] = { 3600, 545 - 16 };
	//tutorialMoviePos[2] = { 4384, 417 - 16 };
	//TUTORIAL_STRING[0] = "Aでジャンプ";
	//TUTORIAL_STRING[1] = "Xでチェイン";
	//TUTORIAL_STRING[2] = "RTでドロップ";

	for (int i = 0; i < Stages->size(); i++)
	{
		clearFlag.push_back(false);
	}
}

void StageMgr::TagInit()
{
	stageTagEase = 0.0F;
	if (nowStage + 1 < 10)
	{
		tagPos.x = Constants::WIN_WIDTH - TAG_GRAPH_WIDTH - NUM_GRAPH_WIDTH;
	}
	else
	{
		tagPos.x = Constants::WIN_WIDTH - TAG_GRAPH_WIDTH - NUM_GRAPH_WIDTH * 2;
		int a = nowStage + 1;
		num[1] = a % 10;
		num[0] = a - num[1];
	}

}

void StageMgr::Set(EnemyMgr* EnemyMgr, ChargeItemMgr* ChargeItemMgr, CollectItemMgr* CollectItemMgr)
{
	EnemyMgr->Set(stages->at(nowStage).GetEnemyNum(), stages->at(nowStage).GetEnemysInfo());
	ChargeItemMgr->Set(stages->at(nowStage).GetCItemNum(), stages->at(nowStage).GetCItemPos());
	CollectItemMgr->Set(stages->at(nowStage).GetCollectNum(), stages->at(nowStage).GetCollectInfo());
}

void StageMgr::Init()
{
	stageTagEase = 1.0F;
}

void StageMgr::Update()
{
	if (stageTagEase < 1.0F)
	{
		stageTagEase += TAG_EASE_SPEED;
		if (1.0F < stageTagEase)stageTagEase = 1.0F;
	}
}

void StageMgr::Draw(CamAccessor* Cam)
{
	stages->at(nowStage).Draw(Cam);

	if (nowStage == 0)
	{
		/*
		for (int i = 0; i < 3; i++)
		{
			if (GetMovieStateToGraph(TUTORIAL_MOVIE[i]) != 1)
			{
				SeekMovieToGraph(TUTORIAL_MOVIE[i], 0);
				PlayMovieToGraph(TUTORIAL_MOVIE[i]);
			}

			DrawBox(Cam->CamAffectX(tutorialMoviePos[i].x - 1280 * MOVIE_SIZE / 2 - 10),
					Cam->CamAffectY(tutorialMoviePos[i].y - 720 * MOVIE_SIZE / 2 - 10),
					Cam->CamAffectX(tutorialMoviePos[i].x + 1280 * MOVIE_SIZE / 2 + 10),
					Cam->CamAffectY(tutorialMoviePos[i].y + 720 * MOVIE_SIZE / 2 + 10 + 32),
					GetColor(211, 133, 255), TRUE);
			DrawFormatString(Cam->CamAffectX(tutorialMoviePos[i].x - 48),
							 Cam->CamAffectY(tutorialMoviePos[i].y + 720 * MOVIE_SIZE / 2 + 10 + 8),
							 GetColor(0, 0, 0), TUTORIAL_STRING[i]);
			DrawRotaGraph(Cam->CamAffectX(tutorialMoviePos[i].x), Cam->CamAffectY(tutorialMoviePos[i].y),
						  MOVIE_SIZE, 0, TUTORIAL_MOVIE[i], FALSE);
		}
		*/
		//チュートリアル描画
		Vector2<float>pos[5] = {
			{ 15 * 32,18 * 32 },
			{ 48 * 32,18 * 32 },
			{ 75 * 32,17 * 32 },
			{112 * 32,16 * 32 },
			{137 * 32,3 * 32}
		};
		for (int i = 0; i < 5; i++)
		{
			pos[i].x = Cam->CamAffectX(pos[i].x);
			pos[i].y = Cam->CamAffectY(pos[i].y);
			//DrawBox(pos[i].x - sizeHalf, pos[i].y - sizeHalf, pos[i].x + sizeHalf, pos[i].y + sizeHalf, GetColor(255, 0, 0), true);
		}
		//ジャンプ
		TUTORIAL_A.Draw(pos[0], { 128,128 });

		//ジャンプ→チェイン
		TUTORIAL_X.Draw({ pos[1].x - 128,pos[1].y }, { 128, 128 });
		TUTORIAL_ARROW.Draw(pos[1], { 128, 128 });
		TUTORIAL_X.Draw({ pos[1].x + 128,pos[1].y }, { 128, 128 });

		//二段階チェイン
		TUTORIAL_X.Draw(pos[2], { 128, 128 });
		TUTORIAL_X.Draw({ pos[2].x + 32 * 10,pos[2].y - 32 * 6 }, { 128, 128 });

		//チェイン連打
		TUTORIAL_X_FAST.Draw(pos[3], { 128, 128 });

		//ドロップ
		TUTORIAL_RT.Draw(pos[4], { 120,120 });
	}
}

void StageMgr::Finalize()
{
	stages->at(nowStage).Finalize();
}

bool StageMgr::SearchStage(int StageNum) const
{
	return stages->at(StageNum).CollectStage();
}

bool StageMgr::CanGoToExtra() const
{
	bool a = true;
	for (int i = 0; i < stages->size() - 1; i++)
	{
		if (clearFlag[i] == false)
		{
			a = false;
			break;
		}
	}
	return a;
}

void StageMgr::RaiseGoalFlag()
{
	if (CollectStage() == false)
	{
		clearFlag[nowStage] = true;
	}
	else
	{
		clearFlag[nowStage] = true;
	}
}

bool StageMgr::NowExtraStage() const
{
	if (nowStage == stages->size() - 1)
	{
		return true;
	}
	else return false;
}

void StageMgr::SetBestScore(int Score)
{
	if (stages->at(nowStage).GetBestScore() < Score)
	{
		stages->at(nowStage).SetBestScore(Score);
	}
}

void StageMgr::TagDraw()
{
	if (stageTagEase != 1.0F)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170 * (1 - Ease(In, Quart, stageTagEase)));

		DrawBox(Constants::WIN_WIDTH, 0, tagPos.x, Constants::WIN_HEIGHT, GetColor(0, 0, 0), true);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (1 - Ease(In, Quart, stageTagEase)));
		DrawGraph(tagPos.x, TAG_DRAW_POSY, STAGE_GRAPH, true);

		if ((nowStage + 1) < 10)
		{
			DrawGraph(tagPos.x + TAG_GRAPH_WIDTH, TAG_DRAW_POSY, *GrobalGraphs::GetNumGraph2(nowStage + 1), true);
		}
		else
		{
			int num = (nowStage + 1) - 10;
			DrawGraph(tagPos.x + TAG_GRAPH_WIDTH, TAG_DRAW_POSY, *GrobalGraphs::GetNumGraph2(1), true);
			DrawGraph(tagPos.x + TAG_GRAPH_WIDTH + NUM_GRAPH_WIDTH, TAG_DRAW_POSY, *GrobalGraphs::GetNumGraph2(num), true);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}