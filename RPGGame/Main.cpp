#include "Framework.h"

int main()
{
	srand(time(NULL));

	Character character;
	unordered_map<string,Character> monsterTable,playerTable;

	DataTable dataTable("PlayerTable.csv", "MonsterTable.csv","Skill.csv");
	/*
	
	monsterTable = ReadCharacterData("MonsterTable.csv");
	playerTable = ReadCharacterData("PlayerTable.csv");
	*/
	monsterTable = dataTable.GetMonster();
	playerTable = dataTable.GetPlayerCharacter();



	string sel = "전사";
	PlayerCharacter* playerCharacter;
	playerCharacter = new PlayerCharacter(playerTable[sel]);
	playerCharacter->Print();

	monsterTable["오크"].Print();



}

