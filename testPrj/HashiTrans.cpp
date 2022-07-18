#include "HashiTrans.h"
#include"DxLib.h"
#include"easing.h"

const float HashiTrans::SPEED = 0.1F;
int HashiTrans::GRAPH[3] = { 0 };

void HashiTrans::Square::Update()
{
	if (easeRate < 1.0F)
	{
		easeRate += SPEED;
		if (1.0F < easeRate)
		{
			easeRate = 1.0F;
			if (end == OUT_END_POS)
			{
				alive = false;
			}
		}
	}

	if (start == IN_START_POS)
	{
		pos.x = (CENTER_POSX - IN_START_POS) * Ease(Out, Quint, easeRate) + IN_START_POS;
	}
	else if (end == OUT_END_POS)
	{
		pos.x = (OUT_END_POS - CENTER_POSX) * Ease(In, Quint, easeRate) + CENTER_POSX;
	}
}

void HashiTrans::Square::Draw()
{
	if (alive)
	{
		if (invertLR && invertUD)
		{
			DrawRotaGraph(CenterPos().x, CenterPos().y, 1.0F, 0.0F, *graph, true, true, true);
		}
		else if (invertLR)
		{
			DrawRotaGraph(CenterPos().x, CenterPos().y, 1.0F, 0.0F, *graph, true, true);
		}
		else if (invertUD)
		{
			DrawRotaGraph(CenterPos().x, CenterPos().y, 1.0F, 0.0F, *graph, true, false, true);
		}
		else
		{
			DrawGraph(pos.x, pos.y, *graph, true);
		}
	}
}

void HashiTrans::Square::Set(int Num, int& Graph)
{
	pos.x = -GRAPH_WIDTH;
	pos.y = Num * GRAPH_HEIGHT;
	invertLR = GetRand(1);
	graph = &Graph;
	invertUD = GetRand(1);
}

void HashiTrans::Square::StartIn()
{
	start = IN_START_POS;
	end = CENTER_POSX;
	easeRate = 0.0F;
	alive = true;
}

void HashiTrans::Square::StartOut()
{
	easeRate = 0.0F;
	start = CENTER_POSX;
	end = OUT_END_POS;
}

HashiTrans::HashiTrans()
{
	if (!GRAPH[0])
	{
		LoadDivGraph("_resource/graphics/transLine.png", 3, 1, 3, GRAPH_WIDTH, GRAPH_HEIGHT, GRAPH);
	}

	full = false;
	out = false;
	in = false;
	for (int i = 0; i < SQUARE_NUM; i++)
	{
		square[i].Init();
	}
	timer = 0;
}

void HashiTrans::Update()
{
	full = false;

	for (int i = 0; i < SQUARE_NUM; i++)
	{
		square[i].Update();
	}

	if (in)
	{
		if (timer != 0 && timer % EMIT_SPAN == 0)
		{
			for (int i = 0; i < SQUARE_NUM; i++)
			{
				if (!emitFlag[i])
				{
					square[i].StartIn();
					emitFlag[i] = true;
					break;
				}
			}
		}

		bool flag = true;

		for (int i = 0; i < SQUARE_NUM; i++)
		{
			if (square[i].easeRate != 1.0F)
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{
			timer = MAX_TIME;
			in = false;
			out = true;
			ResetEmitFlag();
			full = true;
		}
	}
 	if (out)
	{
		if (timer!= 0 && timer % EMIT_SPAN == 0)
		{
			for (int i = 0; i < SQUARE_NUM; i++)
			{
				if (!emitFlag[i])
				{
					square[i].StartOut();
					emitFlag[i] = true;
					break;
				}
			}
		}

		bool flag = true;

		for (int i = 0; i < SQUARE_NUM; i++)
		{
			if (square[i].easeRate != 1.0F)
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{
			out = false;
		}
	}

	if (timer)
	{
		timer--;
	}
}

void HashiTrans::Draw()
{
	for (int i = 0; i < SQUARE_NUM; i++)
	{
		square[i].Draw();
	}
}

void HashiTrans::Start()
{
	if (!in && !out)
	{
		bool flag[SQUARE_NUM] = { false };

		int num = GetRand(SQUARE_NUM-1);
		square[num].Set(num, GRAPH[1]);
		flag[num] = true;

		num = GetRand(SQUARE_NUM-1);

		square[num].Set(num, GRAPH[2]);
		flag[num] = true;

		for (int i = 0; i < SQUARE_NUM; i++)
		{
			if (!flag[i])
			{
				square[i].Set(i, GRAPH[0]);
				flag[i] = true;
			}
			emitFlag[i] = false;
		}

		timer = MAX_TIME;
		in = true;
	}
}

bool HashiTrans::FullScreen()
{
	if (full)
	{
		return true;
	}
	return false;
}

bool HashiTrans::NowTrans()
{
	if (!in && !out)
	{
		return false;
	}
	return true;
}

void HashiTrans::ResetEmitFlag()
{
	for (int i = 0; i < SQUARE_NUM; i++)
	{
		emitFlag[i] = false;
	}
}
