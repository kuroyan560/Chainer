#pragma once
#include"BaseScene.h"
#include"New_Camera.h"
#include"New_PlayerMgr.h"
#include"New_Goal.h"
class Practice_Game:public BaseScene
{
	//�J����
	New_Camera cam;

	//�v���C���[
	New_PlayerMgr player;

	//�G�t�F�N�g�}�l�[�W���[
	EffectMgr effectMgr;

	New_Map map;

	New_Goal goal;

	void Debug();

public:
	Practice_Game(SceneChanger* Changer);
	~Practice_Game() {};

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;
};