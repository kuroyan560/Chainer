#include "New_ResultDraw.h"
#include"New_ItemMgr.h"
#include"GrobalGraphs.h"
#include"MyFunc.h"

New_ResultDraw::New_ResultDraw()
{
	clear = LoadGraph("_resource/graphics/clear.png");
}

void New_ResultDraw::Init()
{
	timer.Reset();
	timer.Start();
	flag = 1;
}

void New_ResultDraw::Update()
{
	timer.Update();
}

void New_ResultDraw::Draw(CamAccessor* Cam)
{
	if (flag == 0)
	{

	}
	else if (flag == 1)
	{

	}
	else if (flag == 2)
	{
		MyFunc::DrawExtendGraph3({ Constants::GetWinCenter().x,Constants::GetWinCenter().y - 128 * 3 / 2 },
			Cam->CamZoomAffect((float)CLEAR_WIDTH),
			Cam->CamZoomAffect((float)CLEAR_HEIGHT), clear, true);

		float posX = Cam->CamAffectX(Constants::GetWinCenter().x - GrobalGraphs::NUM_GRAPH_WIDTH * 4);
		float size = Cam->CamZoomAffect((float)SIZE);

		for (int i = 0; i < TIMER_DIGIT; i++)
		{
			MyFunc::DrawExtendGraph4(posX + size * i, Cam->CamAffectY(Constants::GetWinCenter().y - size/2), size, 
				*GrobalGraphs::GetNumGraph(timerScore[i], NUM_GREEN), true);
			//DrawGraph(posX + size *i, Constants::GetWinCenter().y- size, *GrobalGraphs::GetNumGraph(timerScore[i], NUM_GREEN), true);
		}

		posX = Cam->CamAffectX(Constants::GetWinCenter().x - GrobalGraphs::NUM_GRAPH_WIDTH * (((getItemNum.size() + itemNum.size()) / 2) + 1.5));

		int x = posX;

		for (int i = 0; i < getItemNum.size(); i++)
		{
			MyFunc::DrawExtendGraph4(x += size, Cam->CamAffectY(Constants::GetWinCenter().y) + size/2, 
				size, *GrobalGraphs::GetNumGraph(getItemNum[i], NUM_GREEN), true);
			//DrawGraph(x += size, Constants::GetWinCenter().y, *GrobalGraphs::GetNumGraph(getItemNum[i], NUM_GREEN), true);
		}
		
		x += size;
		DrawLine(x + size - size /4, Cam->CamAffectY(Constants::GetWinCenter().y + size/2), 
			x + size /4, Cam->CamAffectY(Constants::GetWinCenter().y + size*3/2), GetColor(255, 255, 255), 3);

		for (int i = 0; i < itemNum.size(); i++)
		{
			MyFunc::DrawExtendGraph4(x += size, Cam->CamAffectY(Constants::GetWinCenter().y + size/2), size,
				*GrobalGraphs::GetNumGraph(itemNum[i], NUM_GREEN), true);
			//DrawGraph(x += size, Constants::GetWinCenter().y, *GrobalGraphs::GetNumGraph(itemNum[i], NUM_GREEN), true);
		}
	}
}

void New_ResultDraw::FinishResultLoad()
{
	Timer time = timer.GetResultTime();
	int digi = 0;
	int num = time.m;
	int t;

	for (int i = 2 - 1; 0 <= i; i--)
	{
		timerScore[digi++] = num % 10;
		num /= 10;
	}
	t = timerScore[digi - 1];
	timerScore[digi - 1] = timerScore[digi - 2];
	timerScore[digi - 2] = t;

	timerScore[digi++] = 10;

	num = time.s;

	for (int i = 2 - 1; 0 <= i; i--)
	{
		timerScore[digi++] = num % 10;
		num /= 10;
	}
	t = timerScore[digi - 1];
	timerScore[digi - 1] = timerScore[digi - 2];
	timerScore[digi - 2] = t;

	timerScore[digi++] = 10;

	num = time.f;

	for (int i = 2 - 1; 0 <= i; i--)
	{
		timerScore[digi++] = num % 10;
		num /= 10;
	}
	t = timerScore[digi - 1];
	timerScore[digi - 1] = timerScore[digi - 2];
	timerScore[digi - 2] = t;

	MyFunc::SetNumArray(&itemNum, New_ItemMgr::MaxItemNum());
	MyFunc::SetNumArray(&getItemNum, New_ItemMgr::getCount);
	flag = 0;
}

void New_ResultDraw::SetModeResult()
{
	flag = 2;
}
