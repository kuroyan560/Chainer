#include "Record.h"
#include"DxLib.h"
#include"easing.h"
#include"Constants.h"
#include"GrobalGraphs.h"
#include"UsersInput.h"

int Record::COMBO_GRAPH = 0;
int Record::COMPLETE_GRAPH = 0;

static void DrawExtendGraph2(int x, int y, int size, int handle, bool trans)
{
	DrawExtendGraph(x, y, x + size, y  + size, handle, trans);
}

static void DrawGraph2(int CenterX, int CenterY,int SizeX,int SizeY, const int& Handle,bool Trans)
{
	DrawGraph(CenterX - SizeX / 2, CenterY - SizeY / 2, Handle, Trans);
}

void GetScore(int* Array, int Score)
{
	if (Array != nullptr)
	{
		int s = Score;			//スコアのコピー

	//それぞれの桁の数字を求める
		for (int i = Record::SCORE_DIGIT_MAX - 1; 0 <= i; i--)
		{
			Array[i] = s % 10;				//6-i桁目の数字を記録
			s /= 10;					//桁を減らす
		}
	}
}

void GetCombo(int* Array, int Combo)
{
	if (Array != nullptr)
	{
		int c = Combo;

		//それぞれの桁の数字を求める
		for (int i = Record::COMBO_DIGIT_MAX - 1; 0 <= i; i--)
		{
			Array[i] = c % 10;				//6-i桁目の数字を記録
			c /= 10;					//桁を減らす
		}
	}
}

void GetTime(int* Array, Timer ClearTime)
{
	int digi = 0;
	int num = ClearTime.m;
	int t;

	for (int i = 2 - 1; 0 <= i; i--)
	{
		Array[digi++] = num % 10;
		num /= 10;
	}
	t = Array[digi - 1];
	Array[digi - 1] = Array[digi - 2];
	Array[digi - 2] = t;

	Array[digi++] = 10;

	num = ClearTime.s;

	for (int i = 2 - 1; 0 <= i; i--)
	{
		Array[digi++] = num % 10;
		num /= 10;
	}
	t = Array[digi - 1];
	Array[digi - 1] = Array[digi - 2];
	Array[digi - 2] = t;

	Array[digi++] = 10;

	num = ClearTime.f;

	for (int i = 2 - 1; 0 <= i; i--)
	{
		Array[digi++] = num % 10;
		num /= 10;
	}
	t = Array[digi - 1];
	Array[digi - 1] = Array[digi - 2];
	Array[digi - 2] = t;
}

void Record::TimeDraw()
{
	for (int i = 0; i < 8; i++)
	{
		/*float x1 = cam->CamAffectX(TIME_DRAW_POS_X + i * TIME_DRAW_SIZE);
		float y1 = cam->CamAffectY(0);
		float x2 = cam->CamAffectX(TIME_DRAW_POS_X + (i + 1) * TIME_DRAW_SIZE);
		float y2 = cam-> CamAffectY(TIME_DRAW_SIZE);*/
		float x1 = TIME_DRAW_POS_X + i * TIME_DRAW_SIZE;
		float y1 = 0;
		float x2 = TIME_DRAW_POS_X + (i + 1) * TIME_DRAW_SIZE;
		float y2 = TIME_DRAW_SIZE;

		DrawExtendGraph(x1, y1, x2, y2, *GrobalGraphs::GetNumGraph(time[i], NUM_GREEN), true);
		//DrawGraph(TIME_DRAW_POS_X + NUM_GRAPH_SIZE/2 * i, 0, NUM_GRAPH[time[i]], true);
	}
}

void Record::ComboDraw()
{
	DrawGraph(cam->CamAffectX(COMBO_DRAW_POSX + 8), cam->CamAffectY(COMBO_DRAW_POSY - 96), COMBO_GRAPH, true);
	for (int i = 0; i < COMBO_DIGIT_MAX; i++)
	{
		float x1 = cam->CamAffectX(COMBO_DRAW_POSX + COMBO_DRAW_SIZE * i);
		float y1 = cam->CamAffectY(comboY[i]);
		float x2 = cam->CamAffectX(COMBO_DRAW_POSX + COMBO_DRAW_SIZE * (i + 1));
		float y2 = cam->CamAffectY(comboY[i] + COMBO_DRAW_SIZE);

		DrawExtendGraph(x1, y1, x2, y2, *GrobalGraphs::GetNumGraph(comboDraw[i],NUM_GREEN), true);
	}
}

int Record::SCORE_GRAPH = 0;
bool Record::LOAD_GRAPH = false;

const float Record::COMBO_EASE_SPEED = 0.12F;

void Record::UIShake()
{
	cam->Shake(SHAKE_POWER, SHAKE_TIME, SHAKE_SPAN);
}

int Record::GetDigitNum(int num, int digit)
{
	return num / (int)powf(10.0f, digit) % 10;
}

int Record::GetDigit(int num)
{
	int digit = 0;
	while (num != 0) {
		num /= 10;
		digit++;
	}
	return digit;
}

Record::Record(CamAccessor* Cam)
{
	if (Cam != nullptr)
	{
		cam = Cam;
	}

	if (!LOAD_GRAPH)
	{
		SCORE_GRAPH = LoadGraph("_resource/graphics/score.png");
		COMBO_GRAPH = LoadGraph("_resource/graphics/combo.png");
		COMPLETE_GRAPH = LoadGraph("_resource/graphics/complete.png");
		LOAD_GRAPH = true;
	}
}

void Record::Init()
{
	timeCountFlag = true;

	result.Reset();
	timer.Reset();
	GetTime(time, result.resultTime);
	scoreDrawTimer = 0;

	comboTimer = 0;
	combo = 0;

	for (int i = 0; i < COMBO_DIGIT_MAX; i++)
	{
		comboEase[i] = 1.0F;
		comboY[i] = COMBO_DRAW_DEFAULT_POSY;
	}
	addScoreEase = 1.0F;

	timer.Start();

	search = false;
	finish = false;
}

void Record::Init(int ItemMax, int GetItemNum)
{
	Init();
	search = true;
	posX = 0.0F;
	posY = 0.0F;
	collectEase = 1.0F;
	completeEase = 1.0F;
	compY = -1000;
	collectNum = GetItemNum;
	collectMax = ItemMax;
}

void Record::Update()
{
	if (!search)
	{
		//経過時間更新
		if (timeCountFlag)
		{
			timer.Update();
			result.resultTime = timer.GetResultTime();
			GetTime(time, result.resultTime);
		}

		//スコアアップの更新
		if (scoreDrawTimer)
		{
			scoreDrawTimer--;
		}

		if (addScoreEase < 1.0F)
		{
			addScoreEase += 0.05F;
		}

		//コンボ更新
		if (comboTimer)
		{
			comboTimer--;
		}
		else
		{
			if (combo)
			{
				combo = 0;	//しばらくコンボがなかったらコンボリセット
			}
		}

		//コンボ描画更新
		for (int i = 0; i < COMBO_DIGIT_MAX; i++)
		{
			if (comboEase[i] < 1.0F)
			{
				comboEase[i] += COMBO_EASE_SPEED;
				if (1.0F < comboEase[i])
				{
					comboEase[i] = 1.0F;
				}
			}

			comboY[i] = (COMBO_DRAW_POSY - COMBO_DRAW_DEFAULT_POSY) * Ease(Out, Elastic, comboEase[i]) + COMBO_DRAW_DEFAULT_POSY;
		}
	}
	else
	{
		if (collectEase < 1.0F)
		{
			collectEase += 0.02F;

			if (1.0F < collectEase)
			{
				collectEase = 1.0F;

				if (collectNum == collectMax)
				{
					completeEase = 0.0F;
				}
				else
				{
					finish = true;
				}
			}
			posX = (END_POSX - START_POSX) * Ease(InOut, Quint, collectEase) + START_POSX;
			posY = (END_POSY - START_POSY) * Ease(InOut, Quint, collectEase) + START_POSY;
		}

		if (completeEase < 1.0F)
		{
			completeEase += 0.05F;

			if (1.0F < completeEase)
			{
				completeEase = 1.0F;
				finish = true;
			}
			compY = (COMP_END_POSY - COMP_START_POSY) * Ease(Out, Back, completeEase) + COMP_START_POSY;
		}
	}
}

void Record::Draw()
{
	if (!search)
	{
		//経過時間の描画
		TimeDraw();

		//スコアの描画
		//DrawFormatString(Constants::WIN_WIDTH - 12 * 6 - 12 * 6, 0, GetColor(255, 0, 0), "\nscore: %d", result.score);
		DrawExtendGraph(SCORE_DRAW_SIZE * 5, 0, SCORE_DRAW_SIZE * 6, SCORE_DRAW_SIZE, *GrobalGraphs::GetNumGraph(10, NUM_GREEN), TRUE);
		DrawExtendGraph(0, 0, SCORE_DRAW_SIZE * 5, SCORE_DRAW_SIZE, SCORE_GRAPH, TRUE);
		for (int i = 0; i < SCORE_DIGIT_MAX; i++)
		{
			float x1 = SCORE_DRAW_POS_X + (SCORE_DIGIT_MAX - i - 1) * SCORE_DRAW_SIZE;
			float y1 = 0;
			float x2 = SCORE_DRAW_POS_X + (SCORE_DIGIT_MAX - i) * SCORE_DRAW_SIZE;
			float y2 = SCORE_DRAW_SIZE;
			DrawExtendGraph(x1, y1, x2, y2, *GrobalGraphs::GetNumGraph(GetDigitNum(result.score, i), NUM_GREEN), TRUE);
		}

		//スコア変動の描画
		if (scoreDrawTimer)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
			//DrawFormatString(scoreDrawPos.x, scoreDrawPos.y, GetColor(255, 0, 0), "+%d", addScore);

			scoreDrawPos.x = SCORE_DRAW_SIZE * 13 - (GetDigit(addScore) + 1) * SCORE_DRAW_SIZE;
			scoreDrawPos.y = ((SCORE_DRAW_SIZE + 2) - (SCORE_DRAW_SIZE - 16)) * Ease(Out, Elastic, addScoreEase) + (SCORE_DRAW_SIZE - 16);

			for (int i = 0; i < GetDigit(addScore); i++)
			{
				float x1 = scoreDrawPos.x + (GetDigit(addScore) - i - 1) * ADD_SCORE_DRAW_SIZE;
				float y1 = scoreDrawPos.y;
				float x2 = scoreDrawPos.x + (GetDigit(addScore) - i) * ADD_SCORE_DRAW_SIZE;
				float y2 = scoreDrawPos.y + ADD_SCORE_DRAW_SIZE;
				DrawExtendGraph(x1, y1, x2, y2, *GrobalGraphs::GetNumGraph(GetDigitNum(addScore, i), NUM_GREEN), TRUE);
			}
			DrawExtendGraph(scoreDrawPos.x - ADD_SCORE_DRAW_SIZE, scoreDrawPos.y,
							scoreDrawPos.x, scoreDrawPos.y + ADD_SCORE_DRAW_SIZE, *GrobalGraphs::GetNumGraph(11, NUM_GREEN), TRUE);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		//コンボ数の描画
		if (comboTimer)
		{
			float rate = (float)comboTimer / (float)COMBO_SPAN;
			float pal = 255 * Ease(In, Circ, rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
			ComboDraw();
			//DrawFormatString(PlayerLocalPos.x - 12 * 3, PlayerLocalPos.y - 32 - 32, GetColor(0, 0, 255), "%d combo", combo);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	else
	{
		DrawExtendGraph2(posX, posY, 64, *GrobalGraphs::GetCollectItemGraph(), true);
		DrawExtendGraph2(posX + 68, posY + 8, 48, *GrobalGraphs::GetKakeruGraph(), true);
		DrawExtendGraph2(posX + 128, posY + 0, 64, *GrobalGraphs::GetNumGraph(collectNum, NUM_GREEN), true);
		DrawLine(posX + 144 + 64 + 16, posY + 6, posX + 144 + 64, posY + 64 - 6, GetColor(255, 255, 255), 3);
		DrawExtendGraph2(posX + 144 + 64 + 16 + 16, posY, 64, *GrobalGraphs::GetNumGraph(collectMax, NUM_GREEN), true);

		if (collectEase == 1.0F)
		{
			DrawGraph2(Constants::WIN_WIDTH / 2, compY, 320, 64, COMPLETE_GRAPH, true);
		}
	}
}

void Record::EnemyKillScoreGet()
{
	combo++;
	if (result.maxCombo < combo)
	{
		result.maxCombo = combo;
	}

	int oldCombo[COMBO_DIGIT_MAX] = { comboDraw[0],comboDraw[1],comboDraw[2] };
	GetCombo(comboDraw, combo);

	for (int i = 0; i < COMBO_DIGIT_MAX; i++)
	{
		if (oldCombo[i] != comboDraw[i])
		{
			comboEase[i] = 0.0F;
		}
	}

	comboTimer = COMBO_SPAN;

	scoreDrawTimer = SCORE_UP_DRAW_SPAN;

	addScoreEase = 0.0F;
	addScore = combo * KILL_ENEMY_SCORE;
	result.score += addScore;

	if (999999 < result.score)
	{
		result.score = 999999;
	}

	UIShake();
}

void Record::SingleCollectItemGet()
{
	if (!search)
	{
		scoreDrawTimer = SCORE_UP_DRAW_SPAN;
		addScoreEase = 0.0F;
		addScore = 10000;
		result.score += addScore;

		if (999999 < result.score)
		{
			result.score = 999999;
		}
	}
}

void Record::CollectItem()
{
	if (collectNum < collectMax)
	{
		collectNum++;
	}
}

bool Record::AllComplete()
{
	if (search && collectMax == collectNum)
	{
		return true;
	}
	return false;
}

void Record::StartMove()
{
	collectEase = 0.0F;
}

bool Record::Finish()
{
	if (JustInput(A) && finish)
	{
		return true;
	}
	return false;
}
