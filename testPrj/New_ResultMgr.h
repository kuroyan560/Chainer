#pragma once
static class New_ResultMgr
{
	static const int MAX_DIGIT = 2;
	static float GetPosX(int Num);

	static const int START_POSY = -200;
	static const int END_POSY = 0;
	static int easeID[MAX_DIGIT];
	static int oldNum[MAX_DIGIT];
public:
	static void Init();
	static void Draw();
};