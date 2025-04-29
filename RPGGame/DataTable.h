#pragma once
#include "Framework.h"

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
class DataTable {
public:
//	static DataTable dataTable;
	static DataTable* get() {
		if (instance == nullptr) instance = new DataTable();
		return instance;
	}

	DataTable() {}
	void LoadPlayerData(string playerCharacterPath);
	void LoadMonsterData(string monsterCharacterPath);
	void LoadSkillData(string skillPath);

	unordered_map<string, PlayerCharacter> GetPlayerCharacterTable() { return playerCharacterTable; };
	void AddPlayerCharacterTable(string name, PlayerCharacter playerCharacter) { this->playerCharacterTable.insert({ name,playerCharacter }); }

	unordered_map<string, MonsterCharacter> GetMonsterCharacterTable() { return monsterCharacterTable; };
	void AddMonsterCharacterTable(string name, MonsterCharacter monsterCharacter) { this->monsterCharacterTable.insert({ name,monsterCharacter }); }

	unordered_map<string, Skill> GetSkillTable() { return skillTable; };
	void AddSkillTable(string name, Skill skillTable) { this->skillTable.insert({ name,skillTable }); }

private:
	static DataTable* instance;
	unordered_map<string, PlayerCharacter> playerCharacterTable;
	unordered_map<string, MonsterCharacter> monsterCharacterTable;
	unordered_map<string, Skill> skillTable;



};


