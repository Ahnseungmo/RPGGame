#pragma once
#include "Framework.h"

class PlayerCharacter : public Character{
public:
	PlayerCharacter() {
		SetCharType(0);
	}
	template <typename T>
	void TurnStart(vector<T>& side);
private:

};

