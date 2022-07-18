#pragma once
#include<vector>
#include"MuraEffect.h"
#include"CamAccessor.h"

class New_ResultDraw
{
	MTimer timer;
	const int SIZE = 128;

	static const int TIMER_DIGIT = 8;
	int timerScore[TIMER_DIGIT];

	//０…表示しない、１…ゲーム中の表示、２…ステージクリア後の表記
	int flag;

	const int CLEAR_WIDTH = 384;
	const int CLEAR_HEIGHT = 128;
	int clear;

	std::vector<int>itemNum;
	std::vector<int>getItemNum;

public:
	New_ResultDraw();
	~New_ResultDraw() {};

	void Init();
	void Update();
	void Draw(CamAccessor* Cam);

	void FinishResultLoad();
	void SetModeResult();
};

