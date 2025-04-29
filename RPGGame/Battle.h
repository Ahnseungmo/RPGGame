#pragma once

#include "Framework.h"

class Battle {
public:
	Battle(vector<PlayerCharacter> ps, vector<MonsterCharacter> ms): playerSide(ps), monsterSide(ms){
		this->playerSide = ps;
		this->monsterSide = ms;
	}
	void Fight();


private:
	vector<PlayerCharacter> playerSide;
	vector<MonsterCharacter> monsterSide;

};