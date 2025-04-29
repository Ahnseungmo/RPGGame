#pragma once
#include "Framework.h"

class Skill {
public:



    string GetName() const { return name; }
    void SetName(const string& newName) { name = newName; }

    // �Ŀ�(power) getter�� setter
    float GetPower() const { return power; }
    void SetPower(float newPower) { power = newPower; }

    // ��� ����(useMana) getter�� setter
    int GetUseMana() const { return useMana; }
    void SetUseMana(int newUseMana) { useMana = newUseMana; }

    // ����� ����(areaAttack) getter�� setter
    bool IsAreaAttack() const { return isAreaAttack; }
    void SetAreaAttack(bool newAreaAttack) { isAreaAttack = newAreaAttack; }

    // ���ʽ� ����(bonusAttack) getter�� setter
    bool IsBonusAttack() const { return isBonusAttack; }
    void SetBonusAttack(bool newBonusAttack) { isBonusAttack = newBonusAttack; }

    // ġ��Ÿ ����(criticalAttack) getter�� setter
    bool IsCriticalAttack() const { return isCriticalAttack; }
    void SetCriticalAttack(bool newCriticalAttack) { isCriticalAttack = newCriticalAttack; }

private:
	string name;
	float power;
	int useMana;
	bool isAreaAttack;
	bool isBonusAttack;
    bool isCriticalAttack;
};