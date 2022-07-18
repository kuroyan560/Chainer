#pragma once
#include"OgawaEffect.h"
#include"Emitter.h"

class EffectMgr
{
	static const int KICK_SMOKE_NUM = 5;
	KickSmoke kickSmoke[KICK_SMOKE_NUM];

	static const int CHAIN_SQUARE_NUM = 5;
	ChainSquare chainSquare[CHAIN_SQUARE_NUM];

	StartEffect startEffect;

	static const int EXPLOSION_NUM = 2;
	ExplosionEmitter explosionEmitter[EXPLOSION_NUM];

public:
	EffectMgr() {};
	~EffectMgr() {};

	void Init();
	void Update();
	void DrawFront();
	void DrawBack();

	void EmitKickSmoke(Vector2<float>EmitPos, EffectColor ColorName);
	void EmitChainSquare(Vector2<float>EmitPos, EffectColor ColorName);
	void EmitStartEffect(Vector2<float>Pos);
	void EmitExplosion(Vector2<float>Pos);
};