#include "New_ResultMgr.h"
#include"New_ItemMgr.h"
#include"MyFunc.h"
#include"GrobalGraphs.h"
#include"Constants.h"
#include"easing.h"

int New_ResultMgr::easeID[MAX_DIGIT] = { 0 };
int New_ResultMgr::oldNum[MAX_DIGIT] = { 0 };

float New_ResultMgr::GetPosX(int Num)
{
	return Constants::WIN_WIDTH - GrobalGraphs::NUM_GRAPH_WIDTH * (Num + 1);
}

void New_ResultMgr::Init()
{
	for (int i = 0; i < MAX_DIGIT; i++)
	{
		if (!easeID[i])
		{
			CreateEaseSeed(&easeID[i], Out, Elastic, 0.08f, START_POSY, END_POSY);
		}
		PlayEase_Trigger(easeID[i]);
	}
}

void New_ResultMgr::Draw()
{
	int* ptr = nullptr;
	int num = New_ItemMgr::getCount;

	int digi = MyFunc::GetDigits(num);

	if (MAX_DIGIT < digi)
	{
		ptr = new int[MAX_DIGIT];
		for (int i = 0; i < MAX_DIGIT; i++)ptr[i] = 9;
		digi = MAX_DIGIT;
	}
	else if(digi != 0)
	{
		ptr = new int[digi];
		//それぞれの桁の数を求める
		for (int i = 0; i < digi; i++)
		{
			ptr[i] = num % 10;
			num /= 10;
		}
	}

	if (ptr != nullptr)
	{
		for (int i = 0; i < digi; i++)
		{
			if (oldNum[i] != ptr[i])
			{
				PlayEase_Trigger(easeID[i]);
			}
			DrawGraph(GetPosX(i), GetEaseUpdate(easeID[i]), *GrobalGraphs::GetNumGraph(ptr[i], NUM_GREEN), true);
		}
	}
	else
	{
		DrawGraph(GetPosX(0), 0, *GrobalGraphs::GetNumGraph(0, NUM_GREEN), true);
	}

	for (int i = 0; i < digi; i++) //前フレームの情報を記録
	{
		oldNum[i] = ptr[i];
	}

	if (ptr != nullptr)delete[] ptr;
}
