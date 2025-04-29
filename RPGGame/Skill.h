#pragma once
#include "Framework.h"

class Skill {
public:



    string GetName() const { return name; }
    void SetName(const string& newName) { name = newName; }

    // 파워(power) getter와 setter
    float GetPower() const { return power; }
    void SetPower(float newPower) { power = newPower; }

    // 사용 마나(useMana) getter와 setter
    int GetUseMana() const { return useMana; }
    void SetUseMana(int newUseMana) { useMana = newUseMana; }

    // 에어리어 공격(areaAttack) getter와 setter
    bool IsAreaAttack() const { return isAreaAttack; }
    void SetAreaAttack(bool newAreaAttack) { isAreaAttack = newAreaAttack; }

    // 보너스 공격(bonusAttack) getter와 setter
    bool IsBonusAttack() const { return isBonusAttack; }
    void SetBonusAttack(bool newBonusAttack) { isBonusAttack = newBonusAttack; }

    // 치명타 공격(criticalAttack) getter와 setter
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