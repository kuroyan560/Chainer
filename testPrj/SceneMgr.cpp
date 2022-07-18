#include "SceneMgr.h"
#include"Title.h"
#include"DxLib.h"
#include"StagesInfo.h"
#include"UsersInput.h"
#include"Graphics.h"
#include"New_Game.h"
#include"New_Select.h"

SceneMgr::SceneMgr() :stageMgr(GetStages())
{
	nextScene = NoneScene;
	nowScene = TitleScene;

	scene.push_back(new Title(this));
	//scene.push_back(new Select(this, &stageMgr));
	//scene.push_back(new Game(this, &stageMgr));
	//scene.push_back(new Practice_Game(this));
	scene.push_back(new New_Game(this));
	scene.push_back(new New_Select(this));

	scene[nowScene]->Initialize();

	InputSet(&transMgr);
}

SceneMgr::~SceneMgr()
{
	for (int i = 0; i < scene.size(); i++)
	{
		delete scene[i];
	}
}

void SceneMgr::Update()
{
	LoopImage::FlameUpdate();

	KeyUpdate();
	PadUpdate();

	scene[nowScene]->Update();
	transMgr.Update();

	if (nextScene != NoneScene && transMgr.FullScreen())	//????V?[?????Z?b?g??????????
	{
		scene[nowScene]->Finalize();	//?????V?[????I??????
		scene[nextScene]->Initialize(); //????V?[?????????????

		nowScene = nextScene;
		nextScene = NoneScene;
	}
}

void SceneMgr::Draw()
{
	scene[nowScene]->Draw();
	transMgr.Draw();
}

void SceneMgr::ChangeScene(SceneName NextScene)
{
	nextScene = NextScene;
	transMgr.StartSceneChangeTrans();
}