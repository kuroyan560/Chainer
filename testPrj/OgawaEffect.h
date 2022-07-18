#pragma once
#include"Vector2.h"
#include"DxLib.h"
#include"Constants.h"

enum EffectColor { GREEN, PURPLE };

class KickSmoke
{
    static int COLOR[2];

    struct JumpEffect
    {
        float x = 0;
        float y = 0;
        bool flag = false;
        float speed = 0;
        float angle = 0;

        void Init()
        {
            x = 0;
            y = 0;
            flag = false;
            speed = 0;
            angle = 0;
        };
    };

    static const int JUMP_MAX_ARRAY = 15;
    float size;
    int color;
    JumpEffect jump[JUMP_MAX_ARRAY];

public:
    KickSmoke();
    ~KickSmoke() {};

    void Init();
    void Update();
    void Draw();
    void In(Vector2<float> EmitPos,EffectColor ColorName);

    bool Flag()const { return (size <= 0) ? false : true; }
};

class ChainSquare
{
    static int GH[2];

    static const int MAX_EXPAND = 10;
    static const float EXP_EASE_SPEED;

    int x = 0;
    int y = 0;
    float angle = 0;
    float angleRate = 0;
    float extEaseRate = 0;
    float extRate = 0;
    bool flag = false;

    int pal;
    int color;

public:
    ChainSquare();
    ~ChainSquare() {};
    void Init();
    void Update();
    void Draw();
    void In(Vector2<float>LocalPos, EffectColor Color);

    bool Flag() { return flag; }
};

class StartEffect
{
    struct effect04
    {
        float x = 0;
        float y = 0;
        bool particleFlag = 0;
        int alpha = 0;
        float speed = 0;
        float angle = 0;
        int aliveCount = 0;
        float gravity = 0;

        void Init()
        {
            x = 0;
            y = 0;
            particleFlag = false;
            alpha = 0;
            speed = 0;
            angle = 0;
            aliveCount = 0;
            gravity = 0;
        }
    };
    
    static const int EFFECT04_MAX_ARRAY = 20;

    effect04 effect04[EFFECT04_MAX_ARRAY];
    float acceleration;
    float size;
    bool lineFlag;
    int flagCount;
    float lineX;
    int pal;

    float ease;

    float Y;

public:
    void Init();
    void Update();
    void Draw();
    void In(Vector2<float> Pos);
};