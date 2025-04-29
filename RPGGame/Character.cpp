#include "Framework.h"

Character::Character(string name, int maxHealthPoint, int maxManaPoint, int attackPoint, int speedPoint)
{
	this->name = name;
	this->maxHealthPoint = maxHealthPoint;
	this->healthPoint = maxHealthPoint;
	this->maxManaPoint = maxManaPoint;
	this->manaPoint = maxManaPoint;
	this->attackPoint = attackPoint;
	this->speedPoint = speedPoint;
}

void Character::Print()
{
	cout << "이름 : " << name << endl;
	cout << "체력 : " << healthPoint << "/" << maxHealthPoint << endl;
	cout << "마나 : " << manaPoint << "/" << maxManaPoint << endl;
	cout << "공격력 : " << attackPoint << endl;
	cout << "속도 : " << speedPoint << endl;
	cout << "기술 리스트" << endl;

//	for (unordered_map<string,Skill>::iterator it = skills.begin(); it != skills.end(); ++it) {
	for (auto it = skills.begin(); it != skills.end(); ++it) {

		cout << it->second.GetName() << endl;

	}
}

