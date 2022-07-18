#include "JumpEnemy.h"

void JumpEnemy::ParaInit()
{
	t = 0;
	waitCount = 0;
}

void JumpEnemy::Update()
{
	if (waitCount <= 0)
	{
		t += 0.05f;
	}

	if (t >= 2)
	{
		waitCount++;
	}

	if (waitCount >= 120)
	{
		t = 0;
		waitCount = 0;
	}

	pos.y = initPos.y - JUMP_HEIGHT * Ease(Out, Quad, t <= 1 ? t : 2 - t);
}