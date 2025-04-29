#pragma once
#include "Framework.h"

class Monster : public Character {
public:
	Monster(Character character) {
		this->SetName(character.GetName());
		this->SetHealthPoint(character.GetHealthPoint());
		this->SetMaxHealthPoint(character.GetMaxHealthPoint());
		this->SetManaPoint(character.GetManaPoint());
		this->SetMaxManaPoint(character.GetMaxManaPoint());
		this->SetAttackPoint(character.GetAttackPoint());
		this->SetSpeedPoint(character.GetSpeedPoint());
	}
	Monster() {}
	~Monster() {}

	void TurnStart();
};