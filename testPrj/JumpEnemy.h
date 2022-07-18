#pragma once
#include "Enemy.h"

class JumpEnemy : public Enemy
{
	static const int JUMP_HEIGHT = 320;

	double t;
	int waitCount;
public:
	JumpEnemy() {}
	~JumpEnemy() {}
	void ParaInit()override;
	void Update()override;
};

