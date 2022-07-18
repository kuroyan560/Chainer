#include "Particle.h"
#include<math.h>
#include"DxLib.h"
#include"easing.h"

const int Particle::BLEND[2] = { DX_BLENDMODE_ALPHA,DX_BLENDMODE_ADD };

void Particle::Update()
{
	if (_isAliveFlag) {

		_pal -= _palChange;
		if (_pal <= 0) {
			_isAliveFlag = false;
			_mTranser->EmitterDead();
		}

		_pos += _vel;
	}
}


BoxParticle::BoxParticle(ParticleInfoTranser* Transer, int initSize,int maxEmitSpeed,int palChangeRate,BLEND_MODE blend, int r, int g, int b)
{
	_mTranser = Transer;
	_size = initSize;
	_maxSpeed = maxEmitSpeed;
	_palChange = palChangeRate;
	_blendMode = blend;
	_red = r;
	_green = g;
	_blue = b;
}

void BoxParticle::Initialize()
{
	_isAliveFlag = false;
}

void BoxParticle::Draw()
{
	if (_isAliveFlag)
	{
		SetDrawBlendMode(BLEND[_blendMode], _pal);
		DrawBox(_pos.x-_size/2, _pos.y-_size/2, _pos.x + _size/2, _pos.y + _size/2, GetColor(_red, _green, _blue), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void BoxParticle::In(Vector2<float> initPos)
{
	_pos = { initPos.x - _size / 2,initPos.y - _size / 2 };
	_pal = 255;
	float angle = GetRand(360) * DX_PI / 180;
	Vector2<float>_vec = { cos(angle),sin(angle) };
	_vel = _vec * (GetRand(_maxSpeed - 1) + 1);
	_isAliveFlag = true;
}

FallBoxParticle::FallBoxParticle(ParticleInfoTranser* Transer, int initSize, int maxEmitSpeed, int palChangeRate, BLEND_MODE blend, int r, int g, int b, float gravity)
	:BoxParticle(Transer,initSize, maxEmitSpeed, palChangeRate, blend, r, g, b )
{
	_gravity = gravity;
}

void FallBoxParticle::Update()
{
	if (_isAliveFlag) {

		_pal -= _palChange;
		if (_pal <= 0) {
			_isAliveFlag = false;
			_mTranser->EmitterDead();
		}

		_fallVel += _gravity;
		_vel.y += _fallVel;

		_pos += _vel;
	}
}

void FallBoxParticle::In(Vector2<float> initPos)
{
	_pos = { initPos.x - _size / 2,initPos.y - _size / 2 };
	_pal = 255;
	float angle = GetRand(360) * DX_PI / 180;
	Vector2<float>_vec = { cos(angle),sin(angle) };
	_vel = _vec * (GetRand(_maxSpeed - 1) + 1);
	_fallVel = 0.0F;
	_isAliveFlag = true;
}


//爆発パーティクル（画像）=========================

int ExplosionParticle::GRAPH[GRAPH_TYPE] = { 0 };
const float ExplosionParticle::SPIN_SPEED = DX_PI / 180 * 2;

ExplosionParticle::ExplosionParticle(ParticleInfoTranser* Transer, int InitSize, int MaxEmitSpeed, float ChangeRate, BLEND_MODE Blend)
{
	if (GRAPH[0] == 0) {
		LoadDivGraph("_resource/graphics/particle.png", GRAPH_TYPE, GRAPH_TYPE, 1, GRAPH_SIZE, GRAPH_SIZE, GRAPH);
	}
	_mTranser = Transer;
	_size = InitSize;
	_maxSpeed = MaxEmitSpeed;
	_blendMode = Blend;
	_maxSize = InitSize;
	_rateChangeSpeed = ChangeRate;
}

void ExplosionParticle::Initialize()
{
	_isAliveFlag = false;
}

void ExplosionParticle::Update() {

	if (_isAliveFlag) {

		if (_changeRate == 1.0F) {
			_isAliveFlag = false;
			_mTranser->EmitterDead();
		}

		_pal = (1 - Ease(Out, Circ, _changeRate)) * 255;
		_vel.x = (1 - Ease(Out, Circ, _changeRate)) * _maxVel.x;
		_vel.y = (1 - Ease(Out, Circ, _changeRate)) * _maxVel.y;
		_size = (1 - Ease(Out, Circ, _changeRate)) * _maxSize;

		_pos += _vel;

		if (_changeRate < 1.0F) {
			_changeRate += _rateChangeSpeed;
			if (1.0F < _changeRate) {
				_changeRate = 1.0F;
			}
		}

		_angle += SPIN_SPEED;
	}
}

void ExplosionParticle::Draw()
{
	if (_isAliveFlag) {
		SetDrawBlendMode(BLEND[_blendMode], _pal);
		DrawRotaGraph(_pos.x, _pos.y, _size / GRAPH_SIZE, _angle, *_graph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void ExplosionParticle::In(Vector2<float> initPos) {
	_pos = initPos;
	_size = _maxSize;
	_pal = 255;
	_angle = GetRand(360) * DX_PI / 180;
	Vector2<float>_vec = { cos(_angle),sin(_angle) };
	_maxVel = { _vec.x * _maxSpeed,_vec.y * _maxSpeed };
	_vel = { 0.0F,0.0F };
	_changeRate = 0.0F;
	_graph = &GRAPH[GetRand(GRAPH_TYPE)];
	_isAliveFlag = true;
}