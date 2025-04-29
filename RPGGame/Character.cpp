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
	cout << "�̸� : " << name << endl;
	cout << "ü�� : " << healthPoint << "/" << maxHealthPoint << endl;
	cout << "���� : " << manaPoint << "/" << maxManaPoint << endl;
	cout << "���ݷ� : " << attackPoint << endl;
	cout << "�ӵ� : " << speedPoint << endl;
	cout << "��� ����Ʈ" << endl;

//	for (unordered_map<string,Skill>::iterator it = skills.begin(); it != skills.end(); ++it) {
	for (auto it = skills.begin(); it != skills.end(); ++it) {

		cout << it->second.GetName() << endl;

	}
}

