#pragma once

enum class MonsterState {
    IDEL=1,             //�⺻����
    ATTACK=2,           //���ݻ���
    MOVE=3,             //�̵�����
    RETURN_MOVE=4,      //�÷��̾ �þ߿��� ����� �ڱ� �ڸ��� ���ư��� ����
};

class CMonsterInputComponent{
private:
    MonsterState state_;
public:
    void State();
};

