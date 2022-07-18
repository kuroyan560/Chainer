#pragma once
#include "Enemy.h"
class StopEnemy : public Enemy
{
	static LoopImage DEFAULT_GRAPH;
	static bool LOAD_GRAPH;

public:
	StopEnemy();
	~StopEnemy() {}
	virtual void Draw()override;
	virtual void Update()override;
};

