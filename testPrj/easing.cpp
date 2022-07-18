#include<math.h>
#include"easing.h"

static const float PI = 3.14159265359;

using namespace std;

namespace EasingFunction
{
    //イージング関数
    double InQuad(double t)
    {
        return t * t;
    }

    double InCubic(double t)
    {
        return t * t * t;
    }

    double InQuart(double t)
    {
        return t * t * t * t;
    }

    double InQuint(double t)
    {
        return t * t * t * t * t;
    }

    double InSine(double t)
    {
        return 1 - cos(t * PI / 2);
    }

    double InExp(double t)
    {
        return pow(2, -(1 - t) * 10);
    }

    double InCirc(double t)
    {
        return 1 - sqrt((0 > 1 - t * t) ? 0 : 1 - t * t);
    }

    double InBack(double t)
    {
        return t * t * (2.70158 * t - 1.70158);
    }
    double InSoftBack(double t)
    {
        return t * t * (2 * t - 1);
    }

    double InElastic(double t)
    {
        return 56 * t * t * t * t * t - 105 * t * t * t * t + 60 * t * t * t - 10 * t * t;
    }

    double InBounce(double t)
    {
        double pow2;
        int bounce = 4;
        while (t < ((pow2 = pow(2, --bounce)) - 1) / 11) {}
        return 1 / pow(4, 3 - bounce) - 7.5625 * pow((pow2 * 3 - 2) / 22 - t, 2);
    }

    double InLinear(double t)
    {
        return t;
    }

    void EaseSeed::Play_Trigger()
    {
        if (typeName == InOut)
        {
            play = true;
            playBack = false;
        }
        else
        {
            play = true;
            nowType = typeName;
        }
        easeRate = 0.0F;
    }

    void EaseSeed::PlayBack_Trigger()
    {
        if (typeName == InOut)
        {
            playBack = true;
            play = false;
            easeRate == 1.0F;
        }
        else
        {
            float t = start;
            start = end;
            end = t;
            easeRate = 0.0F;
            play = true;
        }
    }

    float EaseSeed::GetEase_Update()
    {
        if (typeName == InOut)
        {
            if (play && easeRate < 1.0F)
            {
                 easeRate += changeRate;
                 if (1.0F < easeRate)
                 {
                     easeRate = 1.0F;
                     play = false;
                 }
            }
            else if (playBack && 0.0F < easeRate)
            {
                 easeRate -= changeRate;
                 if (easeRate < 0.0F)
                 {
                     easeRate = 0.0F;
                     playBack = false;
                 }
            }

            return (end - start) * Ease(typeName, easeName, easeRate) + start;
        }
        else
        {
            if (play && easeRate < 1.0F)
            {
                easeRate += changeRate;
                if (1.0F < easeRate)
                {
                    easeRate = 1.0F;
                    play = false;
                }
            }
            
            return (end - start) * Ease(nowType, easeName, easeRate) + start;
        }
    }

    void EaseSeed2::Play_Trigger()
    {
        if (typeName == InOut)
        {
            play = true;
            playBack = false;
        }
        else
        {
            play = true;
            nowType = typeName;
        }
        easeRate = 0.0F;
    }
    void EaseSeed2::PlayBack_Trigger()
    {
        if (typeName == InOut)
        {
            playBack = true;
            play = false;
            easeRate == 1.0F;
        }
        else
        {
            easeRate = 0.0F;
            play = true;
        }
    }
    void EaseSeed2::Update()
    {
        if (typeName == InOut)
        {
            if (play && easeRate < 1.0F)
            {
                easeRate += changeRate;
                if (1.0F < easeRate)
                {
                    easeRate = 1.0F;
                    play = false;
                }
            }
            else if (playBack && 0.0F < easeRate)
            {
                easeRate -= changeRate;
                if (easeRate < 0.0F)
                {
                    easeRate = 0.0F;
                    playBack = false;
                }
            }
        }
        else
        {
            if (play && easeRate < 1.0F)
            {
                easeRate += changeRate;
                if (1.0F < easeRate)
                {
                    easeRate = 1.0F;
                    play = false;
                }
            }
        }
    }

    std::vector<EaseSeed>EaseSeedMgr::seed;
    std::vector<EaseSeed2>EaseSeedMgr::seed2;

    int EaseSeedMgr::TransIDtoIndex(int ID)
    {
        if (0 < ID)return (ID - 1);
        else if (ID < 0)return (-ID - 1);

        return 0;
    }

    void EaseSeedMgr::PushEaseSeed(int* Id,type Type, ease Ease, float ChangeRate, float Start, float End)
    {
        seed.push_back(EaseSeed(Type, Ease, ChangeRate, Start, End));
        *Id = (seed.size() - 1) + 1;
    }
    void EaseSeedMgr::PushEaseSeed(int* Id, type Type, ease Ease, float ChangeRate)
    {
        seed2.push_back(EaseSeed2(Type, Ease, ChangeRate));
        *Id = ((seed2.size() - 1) + 1) * -1;
    }
    int EaseSeedMgr::Play_Trigger(int id)
    {
        int ID = TransIDtoIndex(id);
        if (0 < id && ID < seed.size())
        {
            seed[ID].Play_Trigger();
            return 0;
        }
        else if (id < 0 && ID < seed2.size())
        {
            seed2[ID].Play_Trigger();
            return 0;
        }
        return -1;
    }
    int EaseSeedMgr::PlayBackTrigger(int id)
    {
        int ID = TransIDtoIndex(id);
        if (0 < id && ID < seed.size())
        {
            seed[ID].PlayBack_Trigger();
            return 0;
        }
        else if (id < 0 && ID < seed2.size())
        {
            seed2[ID].Play_Trigger();
            return 0;
        }
        return -1;
    }
    float EaseSeedMgr::GetEaseUpdate(int id)
    {
        int ID = TransIDtoIndex(id);
        if (0 < id && ID < seed.size())
        {
            return seed[ID].GetEase_Update();
        }
        return 0.0f;
    }
    float EaseSeedMgr::GetEaseAffect(int id, float Start, float End)
    {
        int ID = TransIDtoIndex(id);
        if (id < 0 && ID < seed2.size())
        {
            return (End - Start)* seed2[ID].GetEaseRate() + Start;
        }
        return 0.0f;
    }
    void EaseSeedMgr::EaseUpdate(int id)
    {
        int ID = TransIDtoIndex(id);
        if (id < 0 && ID < seed2.size())
        {
            seed2[ID].Update();
        }
    }
    bool EaseSeedMgr::CompleteEase(int id)
    {
        int ID = TransIDtoIndex(id);
        if (0 < id && ID < seed.size())
        {
            return seed[ID].GetComplete();
        }
        else if (id < 0 && ID < seed2.size())
        {
            return seed2[ID].GetComplete();
        }
        return false;
    }
    int EaseSeedMgr::ReSetStart(int id, float start)
    {
        int ID = TransIDtoIndex(id);
        if (0 < id && ID < seed.size())
        {
            seed[ID].ReSetStart(start);
            return 0;
        }
        return -1;
    }
    int EaseSeedMgr::ReSetEnd(int id, float end)
    {
        int ID = TransIDtoIndex(id);
        if (0 < id && ID < seed.size())
        {
            seed[ID].ReSetEnd(end);
            return 0;
        }
        return -1;
    }
    void EaseSeedMgr::EaseInit(int id)
    {
        int ID = TransIDtoIndex(id);
        if (0 < id && ID < seed.size())
        {
            seed[ID].Init();
        }
        else if (id < 0 && ID < seed2.size())
        {
            seed2[ID].Init();
        }
    }
}

using namespace EasingFunction;
static double (*FuncPtr[12])(double t)
= {
    InQuad,InCubic,InQuart,InQuint,InSine,InExp,InCirc,InBack,InSoftBack,InElastic,InBounce,InLinear
};

double Ease(type Type, ease Ease, double t)
{
    if (Type == In)
    {
        return FuncPtr[Ease](t);
    }
    else if (Type == Out)
    {
        return 1 - FuncPtr[Ease](1 - t);
    }
    else if (Type == InOut)
    {
        if (t < 0.5)
            return FuncPtr[Ease](t * 2) / 2;

        return 1 - FuncPtr[Ease](2 - 2 * t) / 2;
    }
}

int CreateEaseSeed(int* Id, type Type, ease Ease, float ChangeRate, float Start, float End)
{
    if (Id != nullptr)
    {
        EaseSeedMgr::PushEaseSeed(Id, Type, Ease, ChangeRate, Start, End);
        return 0;
    }
    return -1;
}

int CreateEaseSeed(int* Id, type Type, ease Ease, float ChangeRate)
{
    if (Id != nullptr)
    {
        EaseSeedMgr::PushEaseSeed(Id, Type, Ease, ChangeRate);
        return 0;
    }
    return -1;
}

int PlayEase_Trigger(const int& EaseId)
{
    return EaseSeedMgr::Play_Trigger(EaseId);
}

int PlayBackEase_Trigger(const int& EaseId)
{
    return EaseSeedMgr::PlayBackTrigger(EaseId);
}

float GetEaseUpdate(const int& EaseId)
{
    return EaseSeedMgr::GetEaseUpdate(EaseId);
}

float GetEaseAffect(const int& EaseID, const float& Start, const float& End)
{
    return EaseSeedMgr::GetEaseAffect(EaseID, Start, End);
}

void EaseUpdate(const int& EaseId)
{
    EaseSeedMgr::EaseUpdate(EaseId);
}

bool CompleteEase(const int& EaseId)
{
    return EaseSeedMgr::CompleteEase(EaseId);
}

int ReSetEaseStart(const int& EaseId, const float& Start)
{
    return EaseSeedMgr::ReSetStart(EaseId, Start);
}

int ReSetEaseEnd(const int& EaseId, const float& End)
{
    return EaseSeedMgr::ReSetEnd(EaseId, End);
}

void EaseInit(const int& EaseId)
{
    EaseSeedMgr::EaseInit(EaseId);
}