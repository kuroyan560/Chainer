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

	//�O�c�\�����Ȃ��A�P�c�Q�[�����̕\���A�Q�c�X�e�[�W�N���A��̕\�L
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

