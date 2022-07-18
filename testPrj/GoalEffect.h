#pragma once
#include"Vector2.h"
#include"Result.h"
class GoalEffect
{
	static const int SCORE_DIGIT_MAX = 6;
	static const int COMBO_DIGIT_MAX = 3;
	static const int TIME_DIGIT_MAX = 8;

	static const int ITEM_NUM = 3;
	static const int LINE_SPACE = 20;	//行間

	static const Vector2<int>RESULT_GRAPH_SIZE;	//ステージクリア画像のサイズ
	static int RESULT_GRAPH[3];

	static const Vector2<int>NUM_GRAPH_SIZE;

	static const Vector2<int>POS_FIXED;

	static float START_POS_X;
	static float CENTER_POS_X;
	static float END_POS_X;

	static const int EASE_START_SPAN = 5;
	static const float EASE_SPEED;

	//左上座標
	Vector2<float>pos[ITEM_NUM];
	float easeRate[ITEM_NUM];
	bool easeFlag[ITEM_NUM];

	Result result;
	int score[SCORE_DIGIT_MAX];
	int combo[COMBO_DIGIT_MAX];
	int time[8];

	int num;
	int timer;

	bool in;
	bool out;
	bool flag;

	//次へ
	static const int NEXT_GRAPH_SIZE = 128;
	static const float FLASH_SPEED;
	bool nextFlag;
	float angle;

	enum RESULT_NAME { SCORE_RESULT, TIME_RESULT, COMBO_RESULT };


	void DrawScore(Vector2<float>Pos);
	void DrawCombo(Vector2<float>Pos);
	void DrawTime(Vector2<float>Pos);

	bool search;
	float collectEase;
	int getNum;
	int maxNum;

public:
	//下に向かって
	GoalEffect();
	~GoalEffect() {};

	void Init();
	void Update();
	void Draw();

	void GoalIn(Result Result);
	void GoalIn(int GetItemNum, int ItemMax);
	bool Finish();	//演出終了
};