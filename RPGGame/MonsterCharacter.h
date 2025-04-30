#pragma once
#include "Character.h" // Character Ŭ���� ���� �ʿ�

// ���� ����
class Battle;
class Skill; // ��ȯ Ÿ�� ���� �ʿ�

class MonsterCharacter : public Character {
public:
    // �⺻ ������: Ÿ�� ����
    MonsterCharacter() : Character() {
        charType = CharType::MONSTER;
    }

    // �Ķ���� ������ (�⺻ Ŭ���� ������ ȣ��)
    // ���ʿ��� hp/mp �Ķ���� ����
    MonsterCharacter(std::string name, int maxHp, int maxMp, int atk, int spd)
        : Character(std::move(name), maxHp, maxMp, atk, spd, CharType::MONSTER) {}

    // ���� ������ (DataTable ���ø����� �ν��Ͻ� ���� �� �ʿ�)
     MonsterCharacter(const Character& base) : Character(base) { // �⺻ Ŭ���� ���� ����
        charType = CharType::MONSTER; // �ùٸ� Ÿ�� ����

        // --- �߿�: ���� �� HP/MP�� Max ������ �缳�� ---
        this->healthPoint = this->maxHealthPoint;
        this->manaPoint = this->maxManaPoint;
        this->isAlive = (this->healthPoint > 0); // HP ������� ���� ���� �缳��
        // -------------------------------------------------

        // MonsterCharacter Ư�� ����� �ִٸ� ���⼭ ����
    }

    // DecideAction �������̵� (���� AI ����, ���� ��ų �Ǵ� nullptr ��ȯ)
    const Skill* DecideAction(Battle& battle) override;

private:
    // ���� Ư�� ������ �߰� ���� (��: ��� ������, Ư�� ����)

    // 0 �ڽ�Ʈ ��ų ã�� ���� �Լ� (���ο�, const ǥ��)
    const Skill* FindZeroCostSkill() const;
};
