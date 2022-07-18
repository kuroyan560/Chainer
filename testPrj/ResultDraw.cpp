#include "ResultDraw.h"
#include"DxLib.h"
#include"easing.h"
#include"UsersInput.h"

const Vector2<int>ResultDraw::GRAPH_SIZE = { 600,120 };

int ResultDraw::GetDigit(int num)
{
	int digit = 0;
	while (num != 0) {
		num /= 10;
		digit++;
	}
	return digit;
}

void ResultDraw::GetDrawNum(std::vector<int>* Array, const int& Num)
{
	if (Array != nullptr)
	{
		Array->clear();
		int n = Num;
		int digit = GetDigit(n);

		for (int i = 0; i < digit; i++)
		{
			Array->push_back(0);
		}

		for (int i = digit - 1; 0 <= i; i--)
		{
			Array->at(i) = n % 10;
			n /= 10;
		}
	}
}

ResultDraw::ResultDraw()
{
}

ResultDraw::~ResultDraw()
{
}

void ResultDraw::Init()
{
	//result.Reset();
	flag = false;

	for (int i = 0; i < ITEM_NUM; i++)
	{
		easeFlag[i] = false;
		ease[i] = 1.0F;
	}
}

void ResultDraw::Update()
{
	if (flag)
	{
		if (num < ITEM_NUM)
		{
			if (timer % EASE_SPAN == 0)
			{
				easeFlag[num] = true;
				num++;
			}
			timer++;
		}

		for (int i = 0; i < ITEM_NUM; i++)
		{
			posY[i] = (ITEM_BASE_POS_Y - ITEM_START_POS_Y) * Ease(Out, Elastic, ease[i]) + ITEM_START_POS_Y + i * (GRAPH_SIZE.y + ITEM_LINE_SPACE);

			if (easeFlag[i] && ease[i] < 1.0F)
			{
				ease[i] += 0.05F;
				if (1.0F <= ease[i])
				{
					easeFlag[i] = false;
				}
			}
		}

		if (JustInput(A) && num == ITEM_NUM)
		{
		}
	}
}

void ResultDraw::Draw()
{
	if (flag)
	{
		for (int i = 0; i < comboScoreNum.size(); i++)
		{
			DrawFormatString(12 * i, 0, GetColor(255, 0, 0), "%d", comboScoreNum[i]);
		}
		for (int i = 0; i < timeScoreNum.size(); i++)
		{
			DrawFormatString(12 * i, 12, GetColor(255, 0, 0), "%d", timeScoreNum[i]);
		}
		for (int i = 0; i < totalScoreNum.size(); i++)
		{
			DrawFormatString(12 * i, 24, GetColor(255, 0, 0), "%d", totalScoreNum[i]);
		}

		for (int i = 0; i < ITEM_NUM; i++)
		{
			if (ease[i] != 0.0F)
			DrawBox(ITEM_POS_X, posY[i], ITEM_POS_X + GRAPH_SIZE.x, posY[i] + GRAPH_SIZE.y, GetColor(255, 0, 0), true);
		}
	}
}

void ResultDraw::Start(const Result& Result, const Timer& TimeIndex)
{
	if (!flag)
	{
		//result = Result;
		flag = true;

		for (int i = 0; i < ITEM_NUM; i++)
		{
			easeFlag[i] = false;
			ease[i] = 0.0F;
		}

		timer = 0;
		num = 0;

		GetTotalScore(Result, TimeIndex);
	}
}

void ResultDraw::GetTotalScore(const Result& Result, const Timer& TimeIndex)
{
	GetDrawNum(&scoreNum, Result.score);

	int comboScore = 1000 * Result.maxCombo;
	GetDrawNum(&comboScoreNum, comboScore);

	float index = TimeIndex.f + TimeIndex.s * 10 + TimeIndex.m * 100;
	float time = Result.resultTime.f + Result.resultTime.s * 10 + Result.resultTime.m * 100;

	int timeScore = 10000 * (time / index);
	GetDrawNum(&timeScoreNum, timeScore);

	totalScore = Result.score + comboScore + timeScore;

	if (999999 < totalScore)
	{
		totalScore = 999999;
	}
	GetDrawNum(&totalScoreNum, totalScore);
}
