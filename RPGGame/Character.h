#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm> // std::max, std::min
#include <utility>   // std::move

// ���� ����
class Skill;
class Battle; // DecideAction �Ķ���� ���� �ʿ�

// ĳ���� Ÿ�� enum (enum class ��� ����)
enum class CharType {
    PLAYER,
    MONSTER,
    UNKNOWN // �⺻�� �Ǵ� ���� ����
};

class Character {
public:
    // �⺻ ������: �⺻������ �ʱ�ȭ
    Character() : charType(CharType::UNKNOWN), isAlive(false) {}

    // �Ķ���� ������ (HP/MP�� Max ������ �ʱ�ȭ)
    Character(std::string name, int maxHealthPoint, int maxManaPoint,
              int attackPoint, int speedPoint, CharType type)
        : name(std::move(name)),
          maxHealthPoint(std::max(0, maxHealthPoint)), // ���� �ƴ� �ִ� HP ����
          maxManaPoint(std::max(0, maxManaPoint)),     // ���� �ƴ� �ִ� MP ����
          attackPoint(std::max(0, attackPoint)),       // ���� �ƴ� ���ݷ� ����
          speedPoint(speedPoint),
          charType(type),
          healthPoint(this->maxHealthPoint), // ���� HP�� �ִ� HP�� �ʱ�ȭ
          manaPoint(this->maxManaPoint),     // ���� MP�� �ִ� MP�� �ʱ�ȭ
          isAlive(this->maxHealthPoint > 0)  // �ʱ� ���� ���´� �ִ� HP ���
    {}

    // ���� �Ҹ��� (���� �Լ��� �ִ� �⺻ Ŭ������ �ʼ�)
    virtual ~Character() = default;

    // --- Getters (const ǥ��, string/vector�� ���� ���� ���� const& ���) ---
    const std::string& GetName() const { return name; }
    int GetHealthPoint() const { return healthPoint; }
    int GetMaxHealthPoint() const { return maxHealthPoint; }
    int GetManaPoint() const { return manaPoint; }
    int GetMaxManaPoint() const { return maxManaPoint; }
    int GetAttackPoint() const { return attackPoint; }
    int GetSpeedPoint() const { return speedPoint; }
    const std::vector<std::string>& GetSkillNames() const { return skillNames; } // const ���� ��ȯ
    CharType GetCharType() const { return charType; }
    bool IsAlive() const { return isAlive; }

    // --- Setters (������ �ε� �� ���� �����) ---
    void SetName(std::string name) { this->name = std::move(name); }
    void SetHealthPoint(int hp); // ������ ��� Ȯ�� ����
    void SetMaxHealthPoint(int maxHp) { this->maxHealthPoint = std::max(0, maxHp); } // ���� ����
    void SetManaPoint(int mp); // ������ ���� Ȯ�� ����
    void SetMaxManaPoint(int maxMp) { this->maxManaPoint = std::max(0, maxMp); } // ���� ����
    void SetAttackPoint(int ap) { this->attackPoint = std::max(0, ap); } // ���� ����
    void SetSpeedPoint(int sp) { this->speedPoint = sp; }
    void AddSkill(const std::string& skillName); // ��ų �̸� �߰� (�ߺ� Ȯ�� ����)

    // --- ���� ���� �Լ� ---
    // �������� �޴� �Լ� (ĳ���� ��� �� false ��ȯ)
    virtual bool TakeDamage(int damage);

    // ��ų ��� �Լ� (���� �� true ��ȯ)
    virtual bool UseSkill(const Skill& skill, Character& target); // ���� ���
    virtual bool UseSkill(const Skill& skill, std::vector<Character*>& targets); // ���� ���

    // �� ���� �� �ൿ ���� (����� ��ų ������ ��ȯ, ���ų� ���� �� nullptr)
    // �Ļ� Ŭ�������� �ݵ�� �����ϵ��� ���� ���� �Լ��� ����
    virtual const Skill* DecideAction(Battle& battle) = 0;

    // ĳ���� ���� ��� (������, const ǥ��)
    virtual void PrintStatus() const;

protected: // �Ļ� Ŭ���� ���� ���� protected�� ����
    std::string name;
    int healthPoint = 0;
    int maxHealthPoint = 0;
    int manaPoint = 0;
    int maxManaPoint = 0;
    int attackPoint = 0;
    int speedPoint = 0;
    CharType charType = CharType::UNKNOWN;
    bool isAlive = false; // �����ڿ��� HP �������� �ʱ�ȭ��
    std::vector<std::string> skillNames; // ��ų �̸� ����

    // ���� ���� ���� �Լ� (protected)
    void Die(); // ��� ó�� ����
};
