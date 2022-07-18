#include "GoalEffect.h"
#include"Constants.h"
#include"easing.h"
#include"UsersInput.h"
#include "Record.h"
#include"GrobalGraphs.h"
#include"SoundEmitter.h"

const Vector2<int> GoalEffect::RESULT_GRAPH_SIZE = { 600,200 };
const Vector2<int> GoalEffect::NUM_GRAPH_SIZE = { 80,156 };

int GoalEffect::RESULT_GRAPH[3] = { 0 };

const Vector2<int> GoalEffect::POS_FIXED = { 355,20 };

float GoalEffect::START_POS_X = 0.0F;
float GoalEffect::CENTER_POS_X = 0.0F;
float GoalEffect::END_POS_X = 0.0F;
const float GoalEffect::EASE_SPEED = 0.05F;

static const float PI = 3.14159265359F;
const float GoalEffect::FLASH_SPEED = PI / 60;

void GoalEffect::DrawScore(Vector2<float> Pos)
{
	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		DrawGraph(Pos.x + POS_FIXED.x + NUM_GRAPH_SIZE.x * i, Pos.y + POS_FIXED.y, *GrobalGraphs::GetNumGraph2(score[i]), true);
	}
}

void GoalEffect::DrawCombo(Vector2<float> Pos)
{
	for (int i = 0; i < COMBO_DIGIT_MAX; i++)
	{
		DrawGraph(Pos.x + POS_FIXED.x + NUM_GRAPH_SIZE.x * i, Pos.y + POS_FIXED.y, *GrobalGraphs::GetNumGraph2(combo[i]), true);
	}
}

void GoalEffect::DrawTime(Vector2<float> Pos)
{
	for (int i = 0; i < TIME_DIGIT_MAX; i++)
	{
		DrawGraph(Pos.x + POS_FIXED.x + NUM_GRAPH_SIZE.x * i, Pos.y + POS_FIXED.y, *GrobalGraphs::GetNumGraph2(time[i]), true);
	}
}

GoalEffect::GoalEffect()
{
	CENTER_POS_X = (Constants::WIN_WIDTH - RESULT_GRAPH_SIZE.x) / 2 - 210;
	START_POS_X = CENTER_POS_X + Constants::WIN_WIDTH;
	END_POS_X = CENTER_POS_X - Constants::WIN_WIDTH;

	if (!RESULT_GRAPH[0])
	{
		LoadDivGraph("_resource/graphics/stageClear.png", ITEM_NUM, 1, ITEM_NUM, RESULT_GRAPH_SIZE.x, RESULT_GRAPH_SIZE.y, RESULT_GRAPH);
	}
}

void GoalEffect::Init()
{
	for (int i = 0; i<SCORE_DIGIT_MAX; i++)
	{
		score[i] = 0;
	}

	float BaseY = Constants::WIN_HEIGHT / 2 - RESULT_GRAPH_SIZE.y / 2 - (RESULT_GRAPH_SIZE.y + LINE_SPACE) * (ITEM_NUM / 2) ;

	for (int i = 0; i < ITEM_NUM; i++)
	{
		pos[i].x = START_POS_X;
		pos[i].y = BaseY + (RESULT_GRAPH_SIZE.y + LINE_SPACE) * i;
		easeRate[i] = 1.0F;
	}

	timer = 0;

	in = false;
	out = false;
	flag = false;
	nextFlag = false;
	search = false;
}

void GoalEffect::Update()
{
	if (!search)
	{
		for (int i = 0; i < ITEM_NUM; i++)
		{
			if (easeRate[i] < 1.0F && easeFlag[i])
			{
				easeRate[i] += EASE_SPEED;
				if (1.0F <= easeRate[i])
				{
					easeRate[i] = 1.0F;
					easeFlag[i] = false;
				}
			}
		}

		if (timer)
		{
			if (timer % EASE_START_SPAN == 0 && num < ITEM_NUM)
			{
				easeFlag[num] = true;
				num++;
			}
			timer++;
		}

		if (in)
		{
			bool comp = true;

			for (int i = 0; i < ITEM_NUM; i++)
			{
				pos[i].x = (CENTER_POS_X - START_POS_X) * Ease(Out, Back, easeRate[i]) + START_POS_X;
				if (easeRate[i] != 1.0F)
				{
					comp = false;
				}
			}

			if (comp)
			{
				in = false;
				nextFlag = true;
				angle = 0.0F;
			}
		}
		else
		{
			if (JustInput(A) && nextFlag && !out)
			{
				num = 0;
				for (int i = 0; i < ITEM_NUM; i++)
				{
					easeRate[i] = 0.0F;
					easeFlag[i] = false;
				}
				easeFlag[0] = true;
				timer = 10;
				out = true;
				PlaySE(SE_DECISION);
			}
		}

		if (out)
		{
			for (int i = 0; i < ITEM_NUM; i++)
			{
				pos[i].x = (END_POS_X - CENTER_POS_X) * Ease(In, Back, easeRate[i]) + CENTER_POS_X;
			}
		}

		if (nextFlag)
		{
			if (!out)
			{
				angle += FLASH_SPEED;
				if (angle == PI)angle = 0.0F;
			}
			else
			{
				if (0 < sin(angle) * 255)
				{
					angle += FLASH_SPEED;
				}
			}
		}
	}
	else
	{
		if (JustInput(A))
		{
			out = true;
		}
	}
}

void GoalEffect::Draw()
{
	if (!search)
	{
		if (flag)
		{
			for (int i = 0; i < ITEM_NUM; i++)
			{
				DrawGraph(pos[i].x, pos[i].y, RESULT_GRAPH[i], true);
				if (i == SCORE_RESULT)
				{
					DrawScore(pos[i]);
				}
				if (i == TIME_RESULT)
				{
					DrawTime(pos[i]);
				}
				if (i == COMBO_RESULT)
				{
					DrawCombo(pos[i]);
				}
			}

			if (nextFlag)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * sin(angle));
				DrawGraph(Constants::WIN_WIDTH - NEXT_GRAPH_SIZE, Constants::WIN_HEIGHT - NEXT_GRAPH_SIZE, *GrobalGraphs::GetAbuttonGraph(), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	else
	{
		DrawFormatString(640 - 12, 360 - 12, GetColor(255, 0, 0), "%d / %d", getNum, maxNum);
	}
}

void GoalEffect::GoalIn(Result Result)
{
	result = Result;
	GetScore(score,result.score);
	GetCombo(combo,result.maxCombo);
	GetTime(time,result.resultTime);
	num = 0;
	for (int i = 0; i < ITEM_NUM; i++)
	{
		easeRate[i] = 0.0F;
		easeFlag[i] = false;
	}
	easeFlag[0] = true;
	timer = 10;
	in = true;
	flag = true;

	search = false;
}

void GoalEffect::GoalIn(int GetItemNum, int ItemMax)
{
	in = true;
	collectEase = 0.0F;
	getNum = GetItemNum;
	maxNum = ItemMax;
	search = true;
}

bool GoalEffect::Finish()
{
	if (!search && out && easeRate[ITEM_NUM - 1] == 1.0F)
	{
		return true;
	}
	else if(search && out)
	{
		return true;
	}
	else return false;
}
