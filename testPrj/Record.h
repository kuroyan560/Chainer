#pragma once
#include"Vector2.h"
#include"Result.h"
#include"CamAccessor.h"
#include"MuraEffect.h"

void GetScore(int* Array,int Score);
void GetCombo(int* Array,int Combo);
void GetTime(int* Array, Timer ClearTime);

class Record
{
public:
	static const int SCORE_DIGIT_MAX = 6;
	static const int COMBO_DIGIT_MAX = 3;
	static const int TIME_DIGIT_MAX = 8;

private:
	CamAccessor* cam;

	static bool LOAD_GRAPH;
	static const int NUM_GRAPH_SIZE = 128;
	static int SCORE_GRAPH;

	static const int TIME_DRAW_SIZE = NUM_GRAPH_SIZE / 4;
	static const int TIME_DRAW_POS_X = 1280 - TIME_DRAW_SIZE * 8;

	static const int SCORE_DRAW_SIZE = NUM_GRAPH_SIZE / 4;
	static const int SCORE_DRAW_POS_X = SCORE_DRAW_SIZE * 6;

	static const int ADD_SCORE_DRAW_SIZE = NUM_GRAPH_SIZE / 4;

	static const int KILL_ENEMY_SCORE = 100;

	bool timeCountFlag;

	Result result;

	static const int SCORE_UP_DRAW_SPAN = 80;
	int addScore;
	float addScoreEase;
	Vector2<float>scoreDrawPos;
	int scoreDrawTimer;

	static int COMBO_GRAPH;
	static const int COMBO_SPAN = 170;	//コンボが切れるまでのスパン
	int comboTimer;
	int combo;

	static const int COMBO_DRAW_SIZE = NUM_GRAPH_SIZE;
	static const int COMBO_DRAW_POSX = 0;
	static const int COMBO_DRAW_DEFAULT_POSY = - NUM_GRAPH_SIZE;
	static const int COMBO_DRAW_POSY = 720 - NUM_GRAPH_SIZE;
	static const float COMBO_EASE_SPEED;
	float comboEase[COMBO_DIGIT_MAX];
	int comboY[COMBO_DIGIT_MAX];

	int time[TIME_DIGIT_MAX];
	int scoreDraw[SCORE_DIGIT_MAX];
	int comboDraw[COMBO_DIGIT_MAX];

	MTimer timer;

	void TimeDraw();
	void ComboDraw();

	static const int SHAKE_POWER = 30;
	static const int SHAKE_TIME = 16;
	static const int SHAKE_SPAN = 2;
	void UIShake();

	int GetDigitNum(int num, int digit);
	int GetDigit(int num);

	bool search;
	static const int START_POSX = 0;
	static const int START_POSY = 0;
	static const int END_POSX = 1280 / 2 - 128 - 32;
	static const int END_POSY = 720 / 2 - 32;
	float posX;
	float posY;
	float collectEase;
	int collectNum;
	int collectMax;
	bool finish;

	static const int COMP_START_POSY = END_POSY + 64;
	static const int COMP_END_POSY = COMP_START_POSY + 48;
	static int COMPLETE_GRAPH;
	float completeEase;
	float compY;

public:
	Record(CamAccessor* Cam);
	~Record() {};

	void Init();
	void Init(int ItemMax,int GetItemNum);
	void Update();
	void Draw();

	//エネミーを倒した時にスコアゲット
	void EnemyKillScoreGet();

	//収集アイテム回収した時にスコアゲット
	void SingleCollectItemGet();

	void StopGameTimer() { timeCountFlag = false; }
	void ResumeGameTimer() { timeCountFlag = true; }

	Result GetResult()const { return result; }
	int GetResultScore()const { return result.score; }

	void CollectItem();
	int CollectNum()const { return collectNum; }
	int CollectMax()const { return collectMax; }

	bool AllComplete();
	void StartMove();
	bool Finish();
};