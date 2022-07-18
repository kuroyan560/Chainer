#pragma once
#include<queue>
#include"Vector2.h"

enum ActName { NONEACT, JUMP, LEFT_ACT, RIGHT_ACT };

struct Act
{
	ActName name;
	float move;
};

//�s���L���[
class ActRecord
{
private:
	static const Vector2<float> NONE;
	std::queue<Act>recordX;
	std::queue<Act>recordY;
	int numX;
	int numY;

public:
	ActRecord(int DelayFlameX,int DelayFlameY);	//�񑀍�L�����̍s�����f�x���t���[��
	~ActRecord() {};

	//�S�����
	void Init();

	//�s�����L�^
	void PushAct(char which,Act act);
	//�擪�̍s���L�^���Q��
	Act GetAct(char which);
	//�s���L�^��r�o
	void PopAct();
};