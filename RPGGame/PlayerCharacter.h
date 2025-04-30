#pragma once
#include "Character.h" // Character Ŭ���� ���� �ʿ�

// ���� ����
class Battle;
class Skill; // ��ȯ Ÿ�� ���� �߰�

class PlayerCharacter : public Character {
public:
    // �⺻ ������: Ÿ�� ����
    PlayerCharacter() : Character() {
        charType = CharType::PLAYER;
    }

    // �Ķ���� ������ (�⺻ Ŭ���� ������ ȣ��)
    // ���ʿ��� hp/mp �Ķ���� ����, �ʱⰪ���� maxHp/maxMp ���
    PlayerCharacter(std::string name, int maxHp, int maxMp, int atk, int spd)
        : Character(std::move(name), maxHp, maxMp, atk, spd, CharType::PLAYER) {}

    // ���� ������ (DataTable ���ø����� �ν��Ͻ� ���� �� �ʿ�)
    PlayerCharacter(const Character& base) : Character(base) { // �⺻ Ŭ���� ���� ����
        charType = CharType::PLAYER; // �ùٸ� Ÿ�� ����

        // --- �߿�: ���� �� HP/MP�� Max ������ �缳�� ---
        // �̴� DataTable�� ���ø� ��ü�� �����Ǿ����� (�����Ǹ� �� ��)
        // ���� �ν��Ͻ��� �׻� �ִ� ü��/������ �����ϵ��� �����մϴ�.
        this->healthPoint = this->maxHealthPoint;
        this->manaPoint = this->maxManaPoint;
        this->isAlive = (this->healthPoint > 0); // HP ������� ���� ���� �缳��
        // -------------------------------------------------

        // PlayerCharacter Ư�� ����� �ִٸ� ���⼭ ����
    }

    // DecideAction �������̵� (�÷��̾� �ൿ ����, ���� ��ų �Ǵ� nullptr ��ȯ)
    const Skill* DecideAction(Battle& battle) override;

    // ��� ������ ��ų ��� ��� (const ǥ��)
    void PrintSkills() const;

private:
    // �÷��̾� Ư�� ������ �߰� ���� (��: ����ġ, ����)

    // 0 �ڽ�Ʈ ��ų ã�� ���� �Լ� (���ο�, const ǥ��)
    const Skill* FindZeroCostSkill() const;
};
