#pragma once
#include "Framework.h"
#include "Skill.h"
enum CharacterDataType {
	CHARACTERNAME = 0,
	HEALTHPOINT,
	MANAPOINT,
	ATTACKPOINT,
	SPEEDPOINT,
	DATATYPE_SIZE
};

enum SkillDataType {
	SKILLNAME = 0,
	ATTACKPOWER,
	USEMANA,
	AREAATTACK,
	BONUSATTACK,
	CRITICALATTACK
};

//unordered_map<string, Character> ReadCharacterData(string path);

class DataTable {
public:

	DataTable(string PlayerCharacterPath,string MonsterPath,string SkillPath) {
		playerCharacter = ReadCharacterData(PlayerCharacterPath);
		monster = ReadCharacterData(MonsterPath);
		skill = ReadSkillData(SkillPath);
	}

	unordered_map<string, Character> ReadCharacterData(string path);
	unordered_map<string, Skill> ReadSkillData(string path);
	
	unordered_map<string, Character> GetPlayerCharacter() { return playerCharacter; }
	unordered_map<string, Character> GetMonster() { return monster; }
	unordered_map<string, Skill> GetSkill() { return skill; };

private:
	unordered_map<string, Character> playerCharacter;
	unordered_map<string, Character> monster;
	unordered_map<string, Skill> skill;
};