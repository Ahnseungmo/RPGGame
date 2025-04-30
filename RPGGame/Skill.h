#pragma once

#include <string>
#include <utility> // std::move

class Skill {
public:
    // �⺻ ������
    Skill() = default;

    // �Ķ���� ������ (�ϰ��� �̸� ���)
    Skill(std::string name, float power, int useMana, bool isAreaAttack, bool isBonusAttack, bool isCriticalAttack)
        : name(std::move(name)), power(power), useMana(useMana),
          isAreaAttack(isAreaAttack), isBonusAttack(isBonusAttack), isCriticalAttack(isCriticalAttack) {}

    // Getter �Լ� (const ǥ��)
    const std::string& GetName() const { return name; }
    float GetPower() const { return power; }
    int GetUseMana() const { return useMana; }
    bool IsAreaAttack() const { return isAreaAttack; }
    bool IsBonusAttack() const { return isBonusAttack; }    // �ϰ��� �̸�
    bool IsCriticalAttack() const { return isCriticalAttack; } // �ϰ��� �̸�

    // Setter �Լ� (������ �ε� �� �ʿ�)
    void SetName(std::string name) { this->name = std::move(name); }
    void SetPower(float power) { this->power = power; }
    void SetUseMana(int useMana) { this->useMana = useMana; }
    void SetAreaAttack(bool areaAttack) { this->isAreaAttack = areaAttack; }
    void SetBonusAttack(bool isBonusAttack) { this->isBonusAttack = isBonusAttack; }       // �ϰ��� �̸�
    void SetCriticalAttack(bool isCriticalAttack) { this->isCriticalAttack = isCriticalAttack; } // �ϰ��� �̸�

private:
    std::string name;
    float power = 0.0f;         // ��ų ���� ��� �Ǵ� ���� ������
    int useMana = 0;            // �Ҹ� ����
    bool isAreaAttack = false;  // ���� ���� ����
    bool isBonusAttack = false;    // �߰� �� ȹ�� ����
    bool isCriticalAttack = false; // ġ��Ÿ �߻� ���� ���� (���� Ȯ���� �ٸ� ������ ����)
};
