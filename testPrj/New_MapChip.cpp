#include "New_MapChip.h"
#include"DxLib.h"
#include"MyFunc.h"

using namespace Info_MapChip;

New_MapChip::New_MapChip()
{
	if ((LoadDivGraph("_resource/graphics/new_mapchip.png",
		TYPE_NUM - NOT_CHIP_GRAPH_NUM, TYPE_NUM - NOT_CHIP_GRAPH_NUM, 1, CHIP_SIZE, CHIP_SIZE, graph)) == -1)
	{
		MessageBox(NULL, "マップチップのロードに失敗！！", "", MB_OK);
		DebugBreak();
	}
}

void New_MapChip::Draw(int ChipData, float X1, float Y1, float X2, float Y2)
{
	if (ChipData < 0 || TYPE_NUM - NOT_CHIP_GRAPH_NUM <= ChipData)
	{
		MessageBox(NULL, "マップチップ描画で不正なデータを渡されました", "", MB_OK);
		DebugBreak();
	}
	else
	{
		DrawExtendGraph(X1, Y1, X2, Y2, graph[ChipData], true);
	}
}
