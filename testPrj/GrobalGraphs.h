#pragma once

enum NumColor { NUM_PURPLE, NUM_GREEN, NUM_GRA };

static class GrobalGraphs
{
public:
	static const int NUM_GRAPH_WIDTH = 128;
	static const int NUM_GRAPH_HEIGHT = 128;
	static const int NUM_GRAPH_2_WIDTH = 80;
	static const int NUM_GRAPH_2_HEIGHT = 156;

	static void Load();
	static int* GetNumGraph(int Num, NumColor Color);
	static int* GetNumGraph2(int Num);
	static int* GetCollectItemGraph();
	static int* GetKakeruGraph();
	static int* GetAbuttonGraph();
};