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

	//TimeIndex = ���σ^�C���iTimeIndex - Time) * 1000�Ƃ����X�R�A�v�Z
	void Reset();
};