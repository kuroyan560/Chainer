#include "DxLib.h"
#include"Constants.h"
#include"SceneMgr.h"
#include <crtdbg.h>
#include"Map.h"
#include"Fps.h"
#include"GrobalGraphs.h"
#include"SoundEmitter.h"
#include"New_ItemMgr.h"

const char TITLE[] = "Chainer";

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ChangeWindowMode(TRUE);						//ウィンドウモードに設定
	//ウィンドウサイズを手動では変更できず、かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// タイトルを変更
	SetGraphMode(Constants::WIN_WIDTH, Constants::WIN_HEIGHT, 32);	//画面サイズの最大サイズ、カラービット数を設定（モニターの解像度に合わせる）
	SetWindowSizeExtendRate(1.0);				//画面サイズを設定（解像度との比率で設定）
	SetBackgroundColor(39, 34, 46);		// 画面の背景色を設定する

	SetWindowIconID(333);

	//Dxライブラリの初期化
	if (DxLib_Init() == -1) { return -1; }

	//（ダブルバッファ）描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	SceneMgr sceneMgr;

	//画像などのリソースデータの変数宣言と読み込み
	Map::Load();
	New_ItemMgr::Load();
	GrobalGraphs::Load();
	LoadSound();


	//ゲームループで使う変数の宣言
	Fps fps;


	// ゲームループ
	while (1)
	{

		//画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		//更新処理
		sceneMgr.Update();



		//描画処理
		sceneMgr.Draw();



		//---------  ここまでにプログラムを記述  ---------//
		ScreenFlip();//（ダブルバッファ）裏面
		// 20ミリ秒待機（疑似60FPS）
		//WaitTimer(20);
		fps.Update();
		// Windows システムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}
		// ＥＳＣキーが押されたらループから抜ける
		if (sceneMgr.GameEndFlag() == true)
		{
			break;
		}
	}
	//Dxライブラリ終了処理
	DxLib_End();

	return 0;
}