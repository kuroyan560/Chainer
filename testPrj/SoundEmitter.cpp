#include "SoundEmitter.h"
#include"DxLib.h"
#include<vector>

static std::vector<int> se;
static std::vector<int> bgm;

void LoadSound()
{
	se.push_back(LoadSoundMem("_resource/sounds/select.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/decision.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/ground.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/glass.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/chain.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/get.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/goal.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/dead.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/cancel.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/jump.mp3"));
	se.push_back(LoadSoundMem("_resource/sounds/dropEffect.mp3"));

	for (int i = 0; i < se.size(); i++)
	{
		ChangeVolumeSoundMem(150, se[i]);
	}
	ChangeVolumeSoundMem(130, se[SE_GLASS]);

	bgm.push_back(LoadSoundMem("_resource/sounds/bgm.wav"));
	bgm.push_back(LoadSoundMem("_resource/sounds/extra.wav"));

	ChangeVolumeSoundMem(180, bgm[BGM_GAME]);
	ChangeVolumeSoundMem(100, bgm[BGM_EXTRA]);
}

void PlaySE(SEName SE)
{
	PlaySoundMem(se[SE], DX_PLAYTYPE_BACK);
}

void PlayBGM(BGMName BGM)
{
	if (!CheckSoundMem(bgm[BGM]))
	{
		PlaySoundMem(bgm[BGM], DX_PLAYTYPE_LOOP);
	}
}

void StopBGM(BGMName BGM)
{
	if (CheckSoundMem(bgm[BGM]))
	{
		StopSoundMem(bgm[BGM]);
	}
}
