#include "StopEnemy.h"

LoopImage StopEnemy::DEFAULT_GRAPH;
bool StopEnemy::LOAD_GRAPH = false;

StopEnemy::StopEnemy()
{
	if (!LOAD_GRAPH)
	{
		DEFAULT_GRAPH.Init("_resource/graphics/stopEnemy_default.png", true, 4, 13, { 4,1 }, size);
		LOAD_GRAPH = true;
	}
}

void StopEnemy::Draw()
{
	if (aliveFlag)
	{
		DEFAULT_GRAPH.Draw(LocalPos(), size);
	}
}

void StopEnemy::Update()
{
}