#pragma once
struct Timer
{
	int f;
	int s;
	int m;

	void Reset();
};

struct Result
{
	int score;
	Timer resultTime;
	int maxCombo;

	//TimeIndex = 平均タイム（TimeIndex - Time) * 1000というスコア計算
	void Reset();
};