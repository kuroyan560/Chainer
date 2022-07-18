#pragma once
#include"Vector2.h"

class HashiTrans
{
	static int GRAPH[3];
	static const int GRAPH_WIDTH = 1280;
	static const int GRAPH_HEIGHT = 90;

	static const int IN_START_POS = -GRAPH_WIDTH;
	static const int CENTER_POSX = 0;
	static const int OUT_END_POS = 1280;

	static const float SPEED;

	struct Square
	{
		Vector2<float>pos;
		float easeRate;
		bool invertLR;
		bool invertUD;
		int* graph;

		int start;
		int end;

		bool alive;

		Vector2<float> CenterPos() { return { pos.x + GRAPH_WIDTH / 2,pos.y + GRAPH_HEIGHT / 2 }; }

		void Init()
		{
			start = 0;
			end = 0;
			graph = nullptr;
			easeRate = 1.0F;
			alive = false;
		}
		void Update();
		void Draw();
		void Set(int Num, int& Graph);
		void StartIn();
		void StartOut();
	};

	static const int SQUARE_NUM = 720 / 90;

	Square square[SQUARE_NUM];

	bool in;
	bool out;

	bool emitFlag[SQUARE_NUM];
	int timer;
	static const int EMIT_SPAN = 2;
	static const int MAX_TIME = EMIT_SPAN * SQUARE_NUM;

	bool full;

	void ResetEmitFlag();

public:
	HashiTrans();
	~HashiTrans() {};
	void Update();
	void Draw();
	void Start();

	bool FullScreen();
	bool NowTrans();
};