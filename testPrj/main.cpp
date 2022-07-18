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
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ChangeWindowMode(TRUE);						//�E�B���h�E���[�h�ɐݒ�
	//�E�B���h�E�T�C�Y���蓮�ł͕ύX�ł����A���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// �^�C�g����ύX
	SetGraphMode(Constants::WIN_WIDTH, Constants::WIN_HEIGHT, 32);	//��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�i���j�^�[�̉𑜓x�ɍ��킹��j
	SetWindowSizeExtendRate(1.0);				//��ʃT�C�Y��ݒ�i�𑜓x�Ƃ̔䗦�Őݒ�j
	SetBackgroundColor(39, 34, 46);		// ��ʂ̔w�i�F��ݒ肷��

	SetWindowIconID(333);

	//Dx���C�u�����̏�����
	if (DxLib_Init() == -1) { return -1; }

	//�i�_�u���o�b�t�@�j�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	SceneMgr sceneMgr;

	//�摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	Map::Load();
	New_ItemMgr::Load();
	GrobalGraphs::Load();
	LoadSound();


	//�Q�[�����[�v�Ŏg���ϐ��̐錾
	Fps fps;


	// �Q�[�����[�v
	while (1)
	{

		//��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		//�X�V����
		sceneMgr.Update();



		//�`�揈��
		sceneMgr.Draw();



		//---------  �����܂łɃv���O�������L�q  ---------//
		ScreenFlip();//�i�_�u���o�b�t�@�j����
		// 20�~���b�ҋ@�i�^��60FPS�j
		//WaitTimer(20);
		fps.Update();
		// Windows �V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}
		// �d�r�b�L�[�������ꂽ�烋�[�v���甲����
		if (sceneMgr.GameEndFlag() == true)
		{
			break;
		}
	}
	//Dx���C�u�����I������
	DxLib_End();

	return 0;
}