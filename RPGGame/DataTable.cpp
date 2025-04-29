#include "DataTable.h"
DataTable* DataTable::instance = nullptr;
DataTable* dataTable = DataTable::get();

void DataTable::LoadPlayerData(string playerCharacterPath)
{
	fstream fs;
	string buff;
	PlayerCharacter playerData;

	fs.open(playerCharacterPath, ios::in);
	getline(fs, buff);
	while (getline(fs, buff)){
		stringstream ss(buff);
		for (int i = 0;getline(ss, buff, ',');i++) {
			switch (i)
			{
			case CHARACTERNAME:
				playerData.SetName(buff);
				break;
			case HEALTHPOINT:
				playerData.SetHealthPoint(stoi(buff));
				playerData.SetMaxHealthPoint(stoi(buff));
				break;
			case MANAPOINT:
				playerData.SetManaPoint(stoi(buff));
				playerData.SetMaxManaPoint(stoi(buff));
				break;
			case ATTACKPOINT:
				playerData.SetAttackPoint(stoi(buff));
				break;
			case SPEEDPOINT:
				playerData.SetSpeedPoint(stoi(buff));
				break;
			default:
				playerData.AddSkills(buff);
				break;
			}
		}
		AddPlayerCharacterTable(playerData.GetName(),playerData);
	}

}

void DataTable::LoadMonsterData(string monsterCharacterPath)
{
	fstream fs;
	string buff;
	MonsterCharacter monsterData;

	fs.open(monsterCharacterPath, ios::in);
	getline(fs, buff);
	while (getline(fs, buff)) {
		stringstream ss(buff);
		for (int i = 0;getline(ss, buff, ',');i++) {
			switch (i)
			{
			case CHARACTERNAME:
				monsterData.SetName(buff);
				break;
			case HEALTHPOINT:
				monsterData.SetHealthPoint(stoi(buff));
				monsterData.SetMaxHealthPoint(stoi(buff));
				break;
			case MANAPOINT:
				monsterData.SetManaPoint(stoi(buff));
				monsterData.SetMaxManaPoint(stoi(buff));
				break;
			case ATTACKPOINT:
				monsterData.SetAttackPoint(stoi(buff));
				break;
			case SPEEDPOINT:
				monsterData.SetSpeedPoint(stoi(buff));
				break;
			default:
				monsterData.AddSkills(buff);
				break;
			}
		}
		AddMonsterCharacterTable(monsterData.GetName(), monsterData);
	}
}

void DataTable::LoadSkillData(string skillPath)
{
	fstream fs;
	string buff;
	Skill skillData;

	fs.open(skillPath, ios::in);
	getline(fs, buff);
	while (getline(fs, buff)) {
		stringstream ss(buff);
		for (int i = 0;getline(ss, buff, ',');i++) {
			switch (i)
			{
			case SKILLNAME:
				skillData.SetName(buff);
				break;
			case ATTACKPOWER:
				skillData.SetPower(stof(buff));
				break;
			case USEMANA:
				skillData.SetUseMana(stoi(buff));
				break;
			case AREAATTACK:
				skillData.SetAreaAttack(stoi(buff));
				break;
			case BONUSATTACK:
				skillData.SetBonusAttack(stoi(buff));
				break;
			case CRITICALATTACK:
				skillData.SetCriticalAttack(stoi(buff));
				break;
			}
		}
		AddSkillTable(skillData.GetName(), skillData);
	}
}
