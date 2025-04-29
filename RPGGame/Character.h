#pragma once

#include "Framework.h"


class Character {
public:

	Character() {};

	Character(string name, int healthPoint, int maxHealthPoint, int manaPoint,
		int maxManaPoint, int attackPoint, int speedPoint)
		: name(name), healthPoint(healthPoint), maxHealthPoint(maxHealthPoint),
		manaPoint(manaPoint), maxManaPoint(maxManaPoint), attackPoint(attackPoint),
		speedPoint(speedPoint) {}

	string GetName() { return name; }
	void SetName(string name) { this->name = name; }
	int GetHealthPoint() { return healthPoint; }
	void SetHealthPoint(int healthPoint) { this->healthPoint = healthPoint; }
	int GetMaxHealthPoint() { return maxHealthPoint; }
	void SetMaxHealthPoint(int maxhealthPoint) { this->maxHealthPoint = maxHealthPoint; }
	int GetManaPoint() { return manaPoint; }
	void SetManaPoint(int manaPoint) { this->manaPoint = manaPoint; }
	int GetMaxManaPoint() { return maxManaPoint; }
	void SetMaxManaPoint(int maxManaPoint) { this->maxManaPoint = maxManaPoint; }
	int GetAttackPoint() { return attackPoint; }
	void SetAttackPoint(int attackPoint) { this->attackPoint = attackPoint; }
	int GetSpeedPointt() { return speedPoint; }
	void SetSpeedPoint(int speedPoint) { this->speedPoint = speedPoint; }
	vector<string> GetSkills() { return skills; }
	void AddSkills(string name) {skills.push_back(name);}
	int GetCharType() { return charType; }
	void SetCharType(int charType) { this->charType = charType; }

	void Hit();
	void attack();

	template <typename T>
	void TurnStart(vector<T>& side);

private:
	string name;
	int healthPoint;
	int maxHealthPoint;
	int manaPoint;
	int maxManaPoint;
	int attackPoint;
	int speedPoint;
	int charType;
	vector<string> skills;
};