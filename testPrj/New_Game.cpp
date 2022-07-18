#include "New_Game.h"
#include"Constants.h"
#include"SoundEmitter.h"
#include"UsersInput.h"
#include"New_ItemMgr.h"
#include"New_ResultMgr.h"
#include"New_Select.h"
#include"MyFunc.h"

const float New_Game::CAM_FOLLOW_PLAYER_RATE = 0.1f;
int New_Game::MINI_TUTORIAL = 0;

New_Game::New_Game(SceneChanger* Changer)
	:BaseScene(Changer), player(&cam, &effectMgr)
{
	MINI_TUTORIAL = LoadGraph("_resource/graphics/mini_tutorial2.png");

	std::string str("_resource/maps/");
	char c = '0';
	int num = 0;

	if (New_Map::MAP_NUM == 0)
	{
		while (MyFunc::CheckFileExist(str + c + ".csv"))
		{
			mapPass.push_back(str + c + ".csv");
			New_Map::MAP_NUM++;
			num++;
			if (num % 10 == 0)
			{
				str += ('0' + num / 10);
				num = 0;
			}
			c = '0' + num;
		}
	}

	str = "_resource/maps/easy";
	c = '0';
	num = 0;
	if (New_Map::EASY_MAP_NUM == 0)
	{
		while (MyFunc::CheckFileExist(str + c + ".csv"))
		{
			easyMapPass.push_back(str + c + ".csv");
			New_Map::EASY_MAP_NUM++;
			num++;
			if (num % 10 == 0)
			{
				str += ('0' + num / 10);
				num = 0;
			}
			c = '0' + num;
		}
	}

	STR_GRAPH = LoadGraph("_resource/graphics/str.png");
}

void New_Game::Initialize()
{
	if (New_Select::EASY_MODE)
	{
		map.Load(easyMapPass[New_Select::easySelectNum].c_str());
	}
	else
	{
		map.Load(mapPass[New_Select::selectNum].c_str());
	}
	cam.Init(map.GetWidth(),map.GetHeight());
	effectMgr.Init();

	New_ItemMgr::Init(&cam,&effectMgr);
	Vector2<float>PlayerInitPos = map.Init(&goal);
	player.Init(PlayerInitPos);
	New_ResultMgr::Init();
	
	drawFlag = true;

	PlayBGM(BGM_GAME);

	resultMgr.Init();
}

void New_Game::Finalize()
{
	player.Finalize();
	StopBGM(BGM_GAME);
}

void New_Game::Update()
{
	New_ItemMgr::Update();

	bool oldGoalFlag = goal.flag;
	if (!goal.flag && player.GetAlive())
	{
		cam.Update(player.GetOpePlayer()->WorldPos(), CAM_FOLLOW_PLAYER_RATE, player.GetZoomChangeRate());
		resultMgr.Update();
	}
	player.Update(map,goal);
	effectMgr.Update();

	if (player.GetAlive())
	{
		goal.HitCheck(player.GetOpePlayer()->WorldPos(), player.GetCpuPlayer()->WorldPos());
	}

	if (goal.flag)
	{
		drawFlag = !cam.GoalUpdate(goal.GetCenter(), (!oldGoalFlag && goal.flag));
	}

	if (!oldGoalFlag && goal.flag)	//ÉSÅ[ÉãÇµÇΩèuä‘
	{
		
	}
	if (!drawFlag)
	{
		resultMgr.FinishResultLoad();
		resultMgr.SetModeResult();
	}

	if (JustInput(START))
	{
		mSceneChanger->ChangeScene(New_SelectScene);
		PlaySE(SE_DECISION);
	}
	if (KeyJustInput(KEY_INPUT_I) || player.GetDead() || JustInput(BACK))
	{
		Initialize();
	}

	if (!drawFlag && JustInput(A))
	{
		mSceneChanger->ChangeScene(New_SelectScene);
		PlaySE(SE_DECISION);
		New_Select::Goal();
	}
}

void New_Game::Draw()
{
	if (drawFlag)
	{
		map.Draw(cam);
		New_ItemMgr::Draw();
		effectMgr.DrawBack();

		goal.Draw(&cam);

		player.Draw(goal.flag);
		effectMgr.DrawFront();

		New_ResultMgr::Draw();

		DrawGraph(5, 5, MINI_TUTORIAL, true);
		DrawGraph(5, 5 + 128, STR_GRAPH, true);
	}
	else
	{
		//DrawString(0, 0, "ÉSÅ[ÉãÅIÅ@AÇ≈ëIëâÊñ Ç…ñﬂÇÈ", GetColor(255, 255, 255));
	}

	resultMgr.Draw(&cam);

	//cam.Debug();
}
