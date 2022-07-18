#include "New_Select.h"
#include"New_Map.h"
#include"UsersInput.h"
#include"SoundEmitter.h"
#include"MyFunc.h"
#include"Constants.h"
#include"GrobalGraphs.h"
#include"New_ItemMgr.h"
int New_Select::selectNum = 0;
int New_Select::easySelectNum = 0;
std::vector<StageInfo> New_Select::stageInfo;
std::vector<StageInfo> New_Select::easyStageInfo;
bool New_Select::EASY_MODE = true;

void New_Select::Goal()
{
	if (EASY_MODE)
	{
		easyStageInfo[easySelectNum].clear = true;
		if (New_ItemMgr::getCount == New_ItemMgr::MaxItemNum())
		{
			easyStageInfo[easySelectNum].complete = true;
		}
	}
	else
	{
		stageInfo[selectNum].clear = true;
		if (New_ItemMgr::getCount == New_ItemMgr::MaxItemNum())
		{
			stageInfo[selectNum].complete = true;
		}
	}
}

New_Select::New_Select(SceneChanger* Changer):BaseScene(Changer)
{
	selectNum = 0;
	easySelectNum = 0;

	for (int i = 0; i < New_Map::MAP_NUM; i++)
	{
		stageInfo.push_back(StageInfo());
	}
	for (int i = 0; i < New_Map::EASY_MAP_NUM; i++)
	{
		easyStageInfo.push_back(StageInfo());
	}

	complete = LoadGraph("_resource/graphics/new_complete.png");

	LoadDivGraph("_resource/graphics/mode.png", 2, 1, 2, 473, 73, modeGraph);
	modeChangeGraph = LoadGraph("_resource/graphics/modeChange.png");
}

void New_Select::Initialize()
{
	for (int i = 0; i < New_Map::MAP_NUM; i++)
	{
		int num = -(selectNum - i);
		stageInfo[i].pos.x = Constants::WIN_WIDTH / 2 + (STAGE_NAME_SPACE + 128 * 2) * num;
	}
	for (int i = 0; i < New_Map::EASY_MAP_NUM; i++)
	{
		int num = -(easySelectNum - i);
		easyStageInfo[i].pos.x = Constants::WIN_WIDTH / 2 + (STAGE_NAME_SPACE + 128 * 2) * num;
	}

	completePosY = COMPLETE_DEFAULT_POSY;
}

void New_Select::Finalize()
{
}

void New_Select::Update()
{
	if (JustInput(LEFT) && 0 < (EASY_MODE ? easySelectNum : selectNum))
	{
		(EASY_MODE ? easySelectNum : selectNum)--;
		PlaySE(SE_SELECT);
	}
	else if (JustInput(RIGHT) && (EASY_MODE ? easySelectNum : selectNum) < (EASY_MODE ? New_Map::EASY_MAP_NUM : New_Map::MAP_NUM) - 1)
	{
		(EASY_MODE ? easySelectNum : selectNum)++;
		PlaySE(SE_SELECT);
	}

	if (JustInput(A) || JustInput(X))
	{
		PlaySE(SE_DECISION);
		mSceneChanger->ChangeScene(New_GameScene);
	}
	else if (JustInput(START))
	{
		mSceneChanger->ChangeScene(TitleScene);
		PlaySE(SE_DECISION);
	}
	else if (JustInput(RT))
	{
		EASY_MODE = !EASY_MODE;
		PlaySE(SE_DECISION);
	}

	for (int i = 0; i < (EASY_MODE ? New_Map::EASY_MAP_NUM : New_Map::MAP_NUM); i++)
	{
		int num = -((EASY_MODE ? easySelectNum : selectNum) - i);
		if (EASY_MODE)
		{
			easyStageInfo[i].pos.x = MyFunc::lerp(easyStageInfo[i].pos.x, Constants::WIN_WIDTH / 2 + (STAGE_NAME_SPACE + 128 * 2) * num, LERP_SPEED);
		}
		else
		{
			stageInfo[i].pos.x = MyFunc::lerp(stageInfo[i].pos.x, Constants::WIN_WIDTH / 2 + (STAGE_NAME_SPACE + 128 * 2) * num, LERP_SPEED);
		}
	}
}

void New_Select::Draw()
{
	if (!EASY_MODE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 15);
		DrawBox(0, 0, Constants::WIN_WIDTH, Constants::WIN_HEIGHT, GetColor(255, 0, 255), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	int* ptr = nullptr;
	ptr = new int[2];
	int num;

	if (EASY_MODE)
	{
		for (int i = 0; i < New_Map::EASY_MAP_NUM; i++)
		{
			num = i + 1;

			//‚»‚ê‚¼‚ê‚ÌŒ…‚Ì”‚ð‹‚ß‚é
			for (int j = 1; 0 <= j; j--)
			{
				ptr[j] = num % 10;
				num /= 10;
			}

			for (int j = 0; j < 2; j++)
			{
				if (!easyStageInfo[i].clear)
				{

					if (i == easySelectNum)
					{
						DrawRotaGraph(easyStageInfo[i].pos.x - 128 + 128 * 2 * j, Constants::WIN_HEIGHT / 2, 2.0, 0.0, *GrobalGraphs::GetNumGraph(ptr[j], NUM_GREEN), true);
					}
					else
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
						DrawGraph(easyStageInfo[i].pos.x - 128 + 128 * j, STAGE_NAME_DRAW_POSY, *GrobalGraphs::GetNumGraph(ptr[j], NUM_GREEN), true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					}
				}
				else
				{
					if (i == easySelectNum)
					{
						DrawRotaGraph(easyStageInfo[i].pos.x - 128 + 128 * 2 * j, Constants::WIN_HEIGHT / 2, 2.0, 0.0,
							*GrobalGraphs::GetNumGraph(ptr[j], easyStageInfo[i].complete ? NUM_GRA : NUM_PURPLE), true);
					}
					else
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
						DrawGraph(easyStageInfo[i].pos.x - 128 + 128 * j, STAGE_NAME_DRAW_POSY,
							*GrobalGraphs::GetNumGraph(ptr[j], easyStageInfo[i].complete ? NUM_GRA : NUM_PURPLE), true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					}
				}
			}
		}

		if (easyStageInfo[easySelectNum].complete)
		{
			completePosY = MyFunc::lerp(completePosY, COMPLETE_DRAW_POSY, 0.2);
			if (fabs(completePosY - COMPLETE_DRAW_POSY) < 0.1)completePosY = COMPLETE_DRAW_POSY;
			DrawRotaGraph(Constants::GetWinCenter().x, Constants::GetWinCenter().y + completePosY, 1.0, 0.0f, complete, true);
		}
		else
		{
			completePosY = COMPLETE_DEFAULT_POSY;
		}
	}
	else
	{
		for (int i = 0; i < New_Map::MAP_NUM; i++)
		{
			num = i + 1;

			//‚»‚ê‚¼‚ê‚ÌŒ…‚Ì”‚ð‹‚ß‚é
			for (int j = 1; 0 <= j; j--)
			{
				ptr[j] = num % 10;
				num /= 10;
			}

			for (int j = 0; j < 2; j++)
			{
				if (!stageInfo[i].clear)
				{

					if (i == selectNum)
					{
						DrawRotaGraph(stageInfo[i].pos.x - 128 + 128 * 2 * j, Constants::WIN_HEIGHT / 2, 2.0, 0.0, *GrobalGraphs::GetNumGraph(ptr[j], NUM_GREEN), true);
					}
					else
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
						DrawGraph(stageInfo[i].pos.x - 128 + 128 * j, STAGE_NAME_DRAW_POSY, *GrobalGraphs::GetNumGraph(ptr[j], NUM_GREEN), true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					}
				}
				else
				{
					if (i == selectNum)
					{
						DrawRotaGraph(stageInfo[i].pos.x - 128 + 128 * 2 * j, Constants::WIN_HEIGHT / 2, 2.0, 0.0,
							*GrobalGraphs::GetNumGraph(ptr[j], stageInfo[i].complete ? NUM_GRA : NUM_PURPLE), true);
					}
					else
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
						DrawGraph(stageInfo[i].pos.x - 128 + 128 * j, STAGE_NAME_DRAW_POSY,
							*GrobalGraphs::GetNumGraph(ptr[j], stageInfo[i].complete ? NUM_GRA : NUM_PURPLE), true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					}
				}
			}
		}

		if (stageInfo[selectNum].complete)
		{
			completePosY = MyFunc::lerp(completePosY, COMPLETE_DRAW_POSY, 0.2);
			if (fabs(completePosY - COMPLETE_DRAW_POSY) < 0.1)completePosY = COMPLETE_DRAW_POSY;
			DrawRotaGraph(Constants::GetWinCenter().x, Constants::GetWinCenter().y + completePosY, 1.0, 0.0f, complete, true);
		}
		else
		{
			completePosY = COMPLETE_DEFAULT_POSY;
		}
	}

	if (EASY_MODE)
	{
		DrawGraph(Constants::GetWinCenter().x - 473 / 2, 120, modeGraph[0], true);
	}
	else
	{
		DrawGraph(Constants::GetWinCenter().x - 473 / 2, 120, modeGraph[1], true);
	}
	DrawGraph(3, Constants::WIN_HEIGHT - 58 - 3, modeChangeGraph, true);

	delete[] ptr;
}