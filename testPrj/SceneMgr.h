#pragma once
#include"SceneChanger.h"
#include"BaseScene.h"
#include<vector>
#include"UsersInput.h"
#include "StageMgr.h"
#include"TransMgr.h"

class SceneMgr :
    public SceneChanger
{
private:
    StageMgr stageMgr;
    SceneName nowScene;
    SceneName nextScene;

    std::vector<BaseScene*> scene;
    bool gameEnd;

    TransMgr transMgr;

public:
    SceneMgr();
    ~SceneMgr();
    void Update();
    void Draw();

    void ChangeScene(SceneName NextScene);

    void EndGame()override { gameEnd = true; }
    bool GameEndFlag() { return gameEnd; }

    TransMgr* GetTransMgr() { return &transMgr; }
};

