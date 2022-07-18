#include "OgawaEffect.h"
#include"DxLib.h"
#include"easing.h"
#include"Constants.h"

int KickSmoke::COLOR[2] = { 0 };

KickSmoke::KickSmoke()
{
	if (!COLOR[0])
	{
		COLOR[GREEN] = GetColor(43, 220, 67);
		COLOR[PURPLE] = GetColor(217, 64, 167);
	}
};

void KickSmoke::Init()
{
	size = 0;

	for (int i = 0; i < JUMP_MAX_ARRAY; i++)
	{
		jump[i].Init();
	}
}

void KickSmoke::Update()
{
	for (int i = 0; i < JUMP_MAX_ARRAY; i++)
	{
		if (jump[i].flag == true)
		{
			if (jump[i].speed >= 3)
				jump[i].speed -= 3;
			jump[i].x += jump[i].speed * cos(jump[i].angle);
			jump[i].y += jump[i].speed * sin(jump[i].angle);
			size -= 0.05;
			if (size < 0)
			{
				jump[i].Init();
			}
		}
	}
}

void KickSmoke::Draw()
{
	for (int i = 0; i < JUMP_MAX_ARRAY; i++)
	{
		if (jump[i].flag == true)
		{
			DrawCircle(jump[i].x, jump[i].y, size, color, TRUE);
		}
	}
}

void KickSmoke::In(Vector2<float> EmitPos,EffectColor ColorName)
{
	for (int i = 0; i < JUMP_MAX_ARRAY; i++)
	{
		if (jump[i].flag == false)
		{
			jump[i].speed = GetRand(100) / 10 + 10;
			jump[i].angle = (GetRand(180) + 180) / 180.0f * DX_PI_F;
			jump[i].x = EmitPos.x;
			jump[i].y = EmitPos.y;
			size = 6;
			color = COLOR[ColorName];
			jump[i].flag = true;
		}
	}
}

int ChainSquare::GH[2] = { 0 };
const float ChainSquare::EXP_EASE_SPEED = 0.03F;

ChainSquare::ChainSquare()
{
	if (!GH[0])
	{
		LoadDivGraph("_resource/graphics/sikaku.png", 2, 2, 1, 64, 64, GH);
	}
}

void ChainSquare::Init()
{
	x = 0;
	y = 0;
	angle = 0;
	angleRate = 0;
	extRate = 0;
	flag = false;
}

void ChainSquare::Update()
{
	if (flag)
	{
		if (extEaseRate < 1.0F)
		{
			extEaseRate += EXP_EASE_SPEED;
			if (1.0F < extEaseRate)
			{
				extEaseRate = 1.0F;
			}
		}
		else
		{
			flag = false;
		}

		extRate = (float)MAX_EXPAND * Ease(Out, Quart, extEaseRate);
		pal = 190 * (1 - Ease(Out, Quart, extEaseRate));
		angle += angleRate;
	}
}

void ChainSquare::Draw()
{
	if (flag == true)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, pal);
		DrawRotaGraph(x, y, extRate, angle, GH[color], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void ChainSquare::In(Vector2<float> LocalPos, EffectColor Color)
{
	x = LocalPos.x;
	y = LocalPos.y;
	angle = GetRand(180) * DX_PI_F;
	angleRate = GetRand(40) / 100 + 0.1;
	extEaseRate = 0;
	flag = true;
	color = Color;
}

void StartEffect::Init()
{
	for (int i = 0; i < EFFECT04_MAX_ARRAY; i++)
	{
		effect04->Init();
	}
	acceleration = 0.98;
	size = 0;
	lineFlag = false;
	flagCount = 0;
	lineX = 0;
}

void StartEffect::Update()
{
	flagCount++;
	if (lineFlag)
	{
		if (ease < 1.0F)
		{
			ease += 0.04F;
			if (1.0F < ease)ease = 1.0F;
		}

		size = (0 - 60) * Ease(InOut, Bounce, ease) + 60;

		if (ease==1.0F)
			lineFlag = false;

		for (int i = 0; i < EFFECT04_MAX_ARRAY; i++)
		{
			if (effect04[i].particleFlag == 1)
			{
				effect04[i].aliveCount++;
				effect04[i].gravity = effect04[i].aliveCount * acceleration;
				effect04[i].x += effect04[i].speed * cos(effect04[i].angle);
				effect04[i].y += effect04[i].speed * sin(effect04[i].angle) + effect04[i].gravity;
				if (effect04[i].alpha <= 0)
					effect04[i].particleFlag = 0;
			}
		}
	}
}

void StartEffect::Draw()
{
	if (lineFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, GetRand(120));
		DrawLine(lineX, 0, lineX, Y, GetColor(182, 60, 255), size);
		if (flagCount % 2 == 0 && 20 <= size)
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, pal-=3);
			DrawLine(lineX, 0, lineX, Y, GetColor(182, 60, 255), size - 20);
		}
	}

	for (int i = 0; i < EFFECT04_MAX_ARRAY; i++)
	{
		if (effect04[i].particleFlag == 1)
		{
			//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, effect04[i].alpha -= 15);
			DrawCircle(effect04[i].x, effect04[i].y, 3, GetColor(182, 60, 255), TRUE);
		}
	}
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void StartEffect::In(Vector2<float> Pos)
{
	pal = 120;
	lineX = Pos.x;
	size = 60;
	lineFlag = true;
	ease = 0.0F;

	for (int i = 0; i < EFFECT04_MAX_ARRAY; i++)
	{
		if (effect04[i].particleFlag == 0)
		{
			effect04[i].aliveCount = 1;
			effect04[i].speed = GetRand(100) / 10 + 10;
			effect04[i].angle = (GetRand(180) + 180) / 180.0f * DX_PI_F;
			effect04[i].alpha = 200;
			effect04[i].x = lineX;
			effect04[i].y = Pos.y;
			effect04[i].particleFlag = 1;
		}
	}

	Y = Pos.y;
}