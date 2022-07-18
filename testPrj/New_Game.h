#pragma once
#include"BaseScene.h"
#include"New_Camera.h"
#include"New_PlayerMgr.h"
#include"New_Map.h"
#include"New_ItemMgr.h"
#include<vector>
#include<string>
#include"New_Goal.h"
#include"New_ResultDraw.h"

class New_Game :public BaseScene
{
	//マップCSVのパス
	std::vector<std::string>mapPass;
	std::vector<std::string>easyMapPass;

	static const float CAM_FOLLOW_PLAYER_RATE;

	//カメラ
	New_Camera cam;

	//プレイヤー
	New_PlayerMgr player;

	//エフェクトマネージャー
	EffectMgr effectMgr;

	New_Map map;

	//ゴールオブジェ
	New_Goal goal;

	bool drawFlag;

	int STR_GRAPH;

	//時間と取ったアイテムの記録
	New_ResultDraw resultMgr;

	static int MINI_TUTORIAL;

public:
	New_Game(SceneChanger* Changer);
	~New_Game() {};

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;
};

