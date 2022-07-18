#pragma once
#include"BaseScene.h"
#include"New_Camera.h"
#include"New_PlayerMgr.h"
#include"New_Goal.h"
class Practice_Game:public BaseScene
{
	//カメラ
	New_Camera cam;

	//プレイヤー
	New_PlayerMgr player;

	//エフェクトマネージャー
	EffectMgr effectMgr;

	New_Map map;

	New_Goal goal;

	void Debug();

public:
	Practice_Game(SceneChanger* Changer);
	~Practice_Game() {};

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;
};