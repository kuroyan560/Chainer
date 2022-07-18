#pragma once
#include"SceneChanger.h"
#include"Vector2.h"

class BaseScene
{
private:

protected:
	SceneChanger* mSceneChanger;

public:
	BaseScene(SceneChanger* changer);
	virtual ~BaseScene() {};
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};