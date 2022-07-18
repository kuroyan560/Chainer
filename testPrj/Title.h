#pragma once
#include "BaseScene.h"
#include"FloorAndWall.h"
#include"BaseCamera.h"
#include"EffectMgr.h"

class Title :
    public BaseScene
{
private:
    //PlayerMgr player;
   // Camera_SideScroll sideCam;
   // Floor floor;
   // EffectMgr effectMgr;

    static int TITLE_GRAPH;

    static const int NEXT_GRAPH_SIZE = 128;
    static const float FLASH_SPEED;
    float angle;
    
    int select;
    //bool flag;

    const int MAIN_GAME_WIDTH = 780;
    //const int OMAKE_WIDTH = 1090;
    float mainGameWidth;
    //float omakeWidth;

    int mainGame;
    //int omake;
    //int omakeStr;

    //ˆê’èŠÔ‘€ì‚ª‚È‚©‚Á‚½‚ç“®‰æ‚ğ—¬‚·
    const int MOVIE_SPAN = 2500;
    int movie;
    int movieTimer;
    float movieAlpha;

public:
    Title(SceneChanger* changer);
    ~Title();
    void Initialize()override;
    void Finalize()override;
    void Update()override;
    void Draw()override;
};

