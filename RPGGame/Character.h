#pragma once
#include "Framework.h"


class Character
{
public:
	Character() {}
	Character(string name, int maxHealthPoint, int maxManaPoint, int attackPoint, int speedPoint);
	~Character(){}

	void SetName(string name) { this->name = name; }
	string GetName() { return name; }
	void SetHealthPoint(int healthPoint) { this->healthPoint = healthPoint; }
	int GetHealthPoint() { return healthPoint; }
	void SetMaxHealthPoint(int maxHealthPoint) { this->maxHealthPoint = maxHealthPoint; }
	int GetMaxHealthPoint() { return maxHealthPoint; }
	void SetManaPoint(int manaPoint) { this->manaPoint = manaPoint; }
	int GetManaPoint() { return manaPoint; }
	void SetMaxManaPoint(int maxManaPoint) { this->maxManaPoint = maxManaPoint; }
	int GetMaxManaPoint() { return maxManaPoint; }
	void SetAttackPoint(int attackPoint) { this->attackPoint = attackPoint; }
	int GetAttackPoint() { return attackPoint; }
	void SetSpeedPoint(int speedPoint) { this->speedPoint = speedPoint; }
	int GetSpeedPoint() { return speedPoint; }
	void SetSkills(unordered_map<string, Skill> skills) { this->skills = skills; }
	unordered_map<string, Skill> GetSkills() { return skills; }


	void AddSkill(string skillname, Skill skill) { skills.insert({ skillname, skill }); }


	void TurnStart() {}
	void Print();

private:
	string name;
	int healthPoint;
	int maxHealthPoint;
	int manaPoint;
	int maxManaPoint;
	int speedPoint;
	int attackPoint;

	unordered_map<string,Skill> skills;
};

