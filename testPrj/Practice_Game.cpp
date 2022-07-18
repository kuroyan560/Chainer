#include "Practice_Game.h"
#include"Constants.h"
#include"SoundEmitter.h"
#include"UsersInput.h"
#include"New_ItemMgr.h"
#include"New_ResultMgr.h"

void Practice_Game::Debug()
{
	int color = GetColor(135, 92, 174);

	bool flag = false;

	int num = 0;
	for (int y = cam.CamAffectY(map.GetHeight() - 32 * 2 - Constants::GetWinCenter().y * num); ;)
	{
		if (y < 0)
		{
			y += Constants::WIN_HEIGHT;
		}
		else if (Constants::WIN_HEIGHT < y)
		{
			y -= Constants::WIN_HEIGHT;
		}
		else
		{
			DrawLine(0, y, Constants::WIN_WIDTH, y, color, 3);
			break;
		}
		num++;
	}

	num = 0;
	for (float x = cam.CamAffectX(0 + Constants::GetWinCenter().x * num); ;)
	{
		if (x < 0)
		{
			x += Constants::WIN_WIDTH;
		}
		else if (Constants::WIN_WIDTH < x)
		{
			x -= Constants::WIN_WIDTH;
		}
		else
		{
			DrawLine(x, 0, x, Constants::WIN_HEIGHT, color, 3);
			break;
		}
		num++;
	}
	//DrawBox(0, cam.CamAffectY(Constants::WIN_HEIGHT + 32), Constants::WIN_WIDTH, cam.CamAffectY(Constants::WIN_HEIGHT), color, true);
}

Practice_Game::Practice_Game(SceneChanger* Changer)
	:BaseScene(Changer),player(&cam,&effectMgr)
{
	map.Load("_resource/maps/debugMap.csv");
}

void Practice_Game::Initialize()
{
	cam.Init(map.GetWidth(), map.GetHeight());
	effectMgr.Init();

	New_ItemMgr::Init(&cam,&effectMgr);
	Vector2<float>PlayerInitPos = map.Init(nullptr);
	player.Init(PlayerInitPos);
	New_ResultMgr::Init();
}

void Practice_Game::Finalize()
{
	player.Finalize();
}

void Practice_Game::Update()
{
	cam.Update(player.GetOpePlayer()->WorldPos(), 0.1f, player.GetZoomChangeRate());
	player.Update(map,goal);
	effectMgr.Update();

	if (JustInput(START))
	{
		mSceneChanger->ChangeScene(TitleScene);
		PlaySE(SE_DECISION);
	}
	if (KeyJustInput(KEY_INPUT_I) || player.GetDead())
	{
		Initialize();
	}
}

void Practice_Game::Draw()
{
	map.Draw(cam);
	New_ItemMgr::Draw();
	effectMgr.DrawBack();
	player.Draw(false);
	effectMgr.DrawFront();

	//デバッグ用
	//Debug();

	New_ResultMgr::Draw();
}
