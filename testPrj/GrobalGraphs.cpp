#include "GrobalGraphs.h"
#include"DxLib.h"

static int NUM_GRAPH[13 * 3];
static int NUM_GRAPH2[11];
static int COLLECT_ITEM_GRAPH = 0;
static int KAKERU_GRAPH = 0;
static int A_GRAPH = 0;

void GrobalGraphs::Load()
{
    LoadDivGraph("_resource/graphics/num2.png", 13 * 3, 13, 3, 128, 128, NUM_GRAPH);
    LoadDivGraph("_resource/graphics/num.png", 12, 12, 1, 80, 156, NUM_GRAPH2);
    COLLECT_ITEM_GRAPH = LoadGraph("_resource/graphics/collectItem_image.png");
    KAKERU_GRAPH = LoadGraph("_resource/graphics/kakeru.png");
    A_GRAPH = LoadGraph("_resource/graphics/nextButton.png");
}

int* GrobalGraphs::GetNumGraph(int Num, NumColor Color)
{
    return &NUM_GRAPH[Num + Color * 13];
}

int* GrobalGraphs::GetNumGraph2(int Num)
{
    return &NUM_GRAPH2[Num];
}

int* GrobalGraphs::GetCollectItemGraph()
{
    return &COLLECT_ITEM_GRAPH;
}

int* GrobalGraphs::GetKakeruGraph()
{
    return &KAKERU_GRAPH;
}

int* GrobalGraphs::GetAbuttonGraph()
{
    return &A_GRAPH;
}
