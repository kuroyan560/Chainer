#pragma once
#include"TransMgr.h"

typedef enum
{
	TitleScene, New_GameScene, New_SelectScene, NoneScene
}SceneName;

class SceneChanger
{
public:
	virtual ~SceneChanger() {};
	virtual void ChangeScene(SceneName NextScene) = 0;
	virtual void EndGame() = 0;

	virtual TransMgr* GetTransMgr() = 0;
};