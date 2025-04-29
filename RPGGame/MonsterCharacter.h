#pragma once
#include "Framework.h"

class MonsterCharacter : public Character {
public:
	MonsterCharacter() {
		SetCharType(1);
	}
	template <typename T>
	void TurnStart(vector<T>& side);
private:

};