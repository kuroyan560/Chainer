#pragma once
#include"BaseScene.h"
#include<string>
#include<vector>

struct StageInfo
{
	Vector2<float>pos = { 0.0f,0.0f };
	bool clear = false;
	bool complete = false;
};

class New_Select : public BaseScene
{
	int modeGraph[2];
	int modeChangeGraph;

	const int STAGE_NAME_DRAW_POSY = 720 / 2 - 128 / 2;
	const int STAGE_NAME_SPACE = 320;
	const float LERP_SPEED = 0.1f;

	static std::vector<StageInfo>stageInfo;
	static std::vector<StageInfo>easyStageInfo;

	int complete;
	float completePosY;
	const int COMPLETE_DRAW_POSY = 200;
	const int COMPLETE_DEFAULT_POSY = COMPLETE_DRAW_POSY - 50;

public:
	static bool EASY_MODE;
	static void Goal();
	static int selectNum;
	static int easySelectNum;

	New_Select(SceneChanger* Changer);
	~New_Select() {};

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;
};