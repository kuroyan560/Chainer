#include "EffectMgr.h"

void EffectMgr::Init()
{
	for (int i = 0; i < KICK_SMOKE_NUM; i++)
	{
		kickSmoke[i].Init();
	}

	for (int i = 0; i < CHAIN_SQUARE_NUM; i++)
	{
		chainSquare[i].Init();
	}

	startEffect.Init();

	for (int i = 0; i < EXPLOSION_NUM; i++)
	{
		explosionEmitter[i].Init();
	}
}

void EffectMgr::Update()
{
	for (int i = 0; i < KICK_SMOKE_NUM; i++)
	{
		kickSmoke[i].Update();
	}
	for (int i = 0; i < CHAIN_SQUARE_NUM; i++)
	{
		chainSquare[i].Update();
	}
	startEffect.Update();

	for (int i = 0; i < EXPLOSION_NUM; i++)
	{
		explosionEmitter[i].Update();
	}
}

void EffectMgr::DrawFront()
{
	for (int i = 0; i < KICK_SMOKE_NUM; i++)
	{
		kickSmoke[i].Draw();
	}

	for (int i = 0; i < CHAIN_SQUARE_NUM; i++)
	{
		chainSquare[i].Draw();
	}

	for (int i = 0; i < EXPLOSION_NUM; i++)
	{
		explosionEmitter[i].Draw();
	}
}

void EffectMgr::DrawBack()
{
	startEffect.Draw();
}

void EffectMgr::EmitKickSmoke(Vector2<float> EmitPos,EffectColor ColorName)
{
	for (int i = 0; i < KICK_SMOKE_NUM; i++)
	{
		if (kickSmoke[i].Flag() == false)
		{
			kickSmoke[i].In(EmitPos,ColorName);
			break;
		}
	}
}

void EffectMgr::EmitChainSquare(Vector2<float> EmitPos, EffectColor ColorName)
{
	for (int i = 0; i < CHAIN_SQUARE_NUM; i++)
	{
		if (chainSquare[i].Flag() == false)
		{
			chainSquare[i].In(EmitPos, ColorName);
			break;
		}
	}
}

void EffectMgr::EmitStartEffect(Vector2<float> Pos)
{
	startEffect.In(Pos);
}

void EffectMgr::EmitExplosion(Vector2<float> Pos)
{
	for (int i = 0; i < EXPLOSION_NUM; i++)
	{
		if (explosionEmitter[i].GetAliveFlag() == false)
		{
			explosionEmitter[i].In(Pos);
			break;
		}
	}
}
