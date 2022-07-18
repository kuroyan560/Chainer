#include "Title.h"
#include"UsersInput.h"
#include"DxLib.h"
#include"SoundEmitter.h"
#include"GrobalGraphs.h"
#include"MyFunc.h"

int Title::TITLE_GRAPH = 0;
const float Title::FLASH_SPEED = DX_PI / 60;

Title::Title(SceneChanger* changer) :BaseScene(changer)//, sideCam(&player), player(&sideCam, &effectMgr), floor(&sideCam)
{
	TITLE_GRAPH = LoadGraph("_resource/graphics/title.png");
	//flag = false;
	select = 0;

	mainGame= LoadGraph("_resource/graphics/mainGame.png");
	//omake = LoadGraph("_resource/graphics/omake.png");
	//omakeStr = LoadGraph("_resource/graphics/omakeStr.png");
	movie = LoadGraph("_resource/graphics/movie.mp4");
}

Title::~Title()
{
}

void Title::Initialize()
{
	//player.Init();
	//sideCam.Init();
	//floor.Init();
	//effectMgr.Init();
	angle = 0.0F;

	if (select == 0)
	{
		mainGameWidth = MAIN_GAME_WIDTH;
		//omakeWidth = 0;
	}
	else if (select == 1)
	{
		mainGameWidth = 0;
		//omakeWidth = OMAKE_WIDTH;
	}

	movieTimer = 0;
	movieAlpha = 0;
}

void Title::Finalize()
{
	PauseMovieToGraph(movie);
	SeekMovieToGraph(movie, 0);
}

void Title::Update()
{
	//if (flag)
	//{
	//	if (JustInput(A))
	//	{
	//		if (select == 0)
	//		{
	//			mSceneChanger->ChangeScene(New_SelectScene);
	//			PlaySE(SE_DECISION);
	//		}
	//		/*else if(select == 1)
	//		{
	//			mSceneChanger->ChangeScene(Practice_GameScene);
	//			PlaySE(SE_DECISION);
	//		}*/
	//		else if (select == 1)
	//		{
	//			mSceneChanger->ChangeScene(SelectScene);
	//			PlaySE(SE_DECISION);
	//		}
	//	}
	//	else if (JustInput(DOWN))
	//	{
	//		if (select < 1)
	//		{
	//			select++;
	//		}
	//		PlaySE(SE_SELECT);
	//	}
	//	else if (JustInput(UP))
	//	{
	//		if (0 < select)
	//		{
	//			select--;
	//		}
	//		PlaySE(SE_SELECT);
	//	}
	//	else if (JustInput(START))
	//	{
	//		flag = false;
	//		PlaySE(SE_DECISION);
	//	}

	//	//omakeWidth = MyFunc::lerp(omakeWidth, (select == 0) ? 0 : OMAKE_WIDTH, 0.2);
	//	//if (fabs(omakeWidth - ((select == 0) ? 0 : OMAKE_WIDTH)) < 0.1)omakeWidth = (select == 0) ? 0.0f : OMAKE_WIDTH;
	//	mainGameWidth = MyFunc::lerp(mainGameWidth, (select == 0) ? MAIN_GAME_WIDTH : 0, 0.2);
	//	if (fabs(mainGameWidth - ((select == 0) ? MAIN_GAME_WIDTH : 0)) < 0.1)mainGameWidth = (select == 0) ? MAIN_GAME_WIDTH : 0.0f;
	//}
	//else
	//{
		if (JustInput(A))
		{
			PlaySE(SE_DECISION);
			mSceneChanger->ChangeScene(New_SelectScene);
			PlaySE(SE_DECISION);
			//flag = true;
		}
		else if (JustInput(START))
		{
			PlaySE(SE_DECISION);
			mSceneChanger->EndGame();
		}
		angle += FLASH_SPEED;
		if (angle == DX_PI)angle = 0.0F;
	//}
	//player.TitleUpdate();
	//sideCam.Update();
	//floor.Update();
	//effectMgr.Update();

		if (GetMovieStateToGraph(movie) == 0)	//停止中
		{
			movieAlpha = 0.0f;
			movieTimer++;
			if (movieTimer == MOVIE_SPAN)
			{
				SeekMovieToGraph(movie, 0);
				PlayMovieToGraph(movie);
				movieTimer = 0;
			}
		}
		if (GetMovieStateToGraph(movie) == 1)	//再生中
		{
			movieAlpha = MyFunc::lerp(movieAlpha, 255.0f, 0.1f);
			if (254.0f < movieAlpha)movieAlpha = 255.0f;
		}
}

void Title::Draw()
{
	//if(!flag)
	//{
		//DrawFormatString(0, 0, GetColor(255, 255, 255), "?^?C?g???V?[??\nA?{?^????X?e?[?W?I?????");

		DrawGraph(Constants::WIN_WIDTH / 2 - 748 / 2, Constants::WIN_HEIGHT / 2 - 346 / 2 - 128, TITLE_GRAPH, true);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * sin(angle));
		DrawGraph(Constants::WIN_WIDTH / 2 - NEXT_GRAPH_SIZE / 2, Constants::WIN_HEIGHT / 2 + NEXT_GRAPH_SIZE, *GrobalGraphs::GetAbuttonGraph(), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//}
	//else
	//{
	//	DrawBox(54 - 4, 120 - 4, 54 - 4 + mainGameWidth, 120 + 133 + 4, GetColor(74, 40, 87), true);
	//	//DrawBox(54 - 4, 420 - 4, 54 - 4 + omakeWidth, 420 + 123 + 4, GetColor(74, 40, 87), true);
	//	if (select == 0)
	//	{
	//		DrawGraph(54, 120, mainGame, true);
	//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//		//DrawGraph(54, 420, omake, true);
	//	}
	//	else if (select == 1)
	//	{
	//		//DrawGraph(54, 420, omake, true);
	//	//	DrawGraph(490, 435, omakeStr, true);
	//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//		DrawGraph(54, 120, mainGame, true);
	//	}
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//	//DrawFormatString(0, 0, GetColor(255, 255, 255), (select == 0) ? "\n→旧バージョン" : "\n　旧バージョン");
	//	//DrawFormatString(0, 0, GetColor(255, 255, 255), (select == 1) ? "→新モード" : "　新モード");
	//}

	//effectMgr.DrawBack();
	//player.Draw();
	//floor.Draw();
	//effectMgr.DrawFront();

		if (GetMovieStateToGraph(movie) == 1)	//再生中
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, movieAlpha);
			DrawExtendGraph(0, 0, Constants::WIN_WIDTH, Constants::WIN_HEIGHT, movie, false);
		}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
