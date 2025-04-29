#pragma once
#include "Framework.h"

class Skill {
public:

	void SetName(string name) { this->name = name; }
	string GetName() { return name; }
	void SetAttackPower(int attackPower) { this->attackPower = attackPower; }
	int GetAttackPower() { return attackPower; }
	void SetUseMana(int attackPower) { this->useMana = useMana; }
	int GetUseMana() { return useMana; }
	void SetAreaAttack(bool areaAttack) { this->areaAttack = areaAttack; }
	bool GetAreaAttack() { return areaAttack; }
	void SetBonusAttack(bool bonusAttack) { this->bonusAttack = bonusAttack; }
	bool GetBonusAttack() { return bonusAttack; }
	void SetCriticalAttack(bool criticalAttack) { this->criticalAttack = criticalAttack; }
	bool GetCriticalAttack() { return criticalAttack; }



private:
	string name;
	int attackPower;
	int useMana;
	bool areaAttack;
	bool bonusAttack;
	bool criticalAttack;
};
