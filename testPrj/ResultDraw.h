#pragma once
#include"Result.h"
#include<vector>
#include"Vector2.h"

class ResultDraw
{
	//Result result;
	int totalScore;

	static const Vector2<int> GRAPH_SIZE;
	
	std::vector<int> scoreNum;
	std::vector<int> comboScoreNum;
	std::vector<int> timeScoreNum;
	std::vector<int> totalScoreNum;

	bool flag;

	int GetDigit(int num);
	void GetDrawNum(std::vector<int>* Array, const int& Num);

	static const int ITEM_NUM = 4;
	static const int ITEM_POS_X = 1280 / 2 - 300;
	static const int ITEM_BASE_POS_Y = 100;
	static const int ITEM_START_POS_Y = ITEM_BASE_POS_Y-18;
	static const int ITEM_LINE_SPACE = 30;

	float posY[ITEM_NUM];
	bool easeFlag[ITEM_NUM];
	float ease[ITEM_NUM];

	static const int EASE_SPAN = 3;
	int timer;
	int num;

public:
	ResultDraw();
	~ResultDraw();

	void Init();
	void Update();
	void Draw();

	void Start(const Result& Result,const Timer& TimeIndex);

	void GetTotalScore(const Result& Result, const Timer& TimeIndex);
};