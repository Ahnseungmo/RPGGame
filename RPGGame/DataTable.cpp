#include "Framework.h"
#include "DataTable.h"

unordered_map<string, Character> DataTable::ReadCharacterData(string path)
{
	unordered_map<string, Character> CharData;
	Character data;
	fstream fs;
	fs.open(path, ios::in);
	string buff;
	getline(fs, buff);
	while (getline(fs, buff)) {
		stringstream ss(buff);
		ss.clear();
		for (int i = 0; getline(ss, buff, ','); i++) {
			switch (i) {
			case CHARACTERNAME:
				data.SetName(buff);
				break;
			case HEALTHPOINT:
				data.SetHealthPoint(stoi(buff));
				data.SetMaxHealthPoint(stoi(buff));
				break;
			case MANAPOINT:
				data.SetManaPoint(stoi(buff));
				data.SetMaxManaPoint(stoi(buff));
				break;
			case ATTACKPOINT:
				data.SetAttackPoint(stoi(buff));
				break;
			case SPEEDPOINT:
				data.SetSpeedPoint(stoi(buff));
				break;
			default:				
				data.AddSkill(buff,GetSkill()[buff]);
				break;
			}
		}
		CharData.insert({ data.GetName(), data });
	}
	return CharData;
}

unordered_map<string, Skill> DataTable::ReadSkillData(string path)
{
	unordered_map<string, Skill> SkillData;
	Skill data;
	fstream fs;
	fs.open(path, ios::in);
	string buff;
	getline(fs, buff);
	while (getline(fs, buff)) {
		stringstream ss(buff);
		ss.clear();
		for (int i = 0; getline(ss, buff, ','); i++) {
			switch (i) {
			case SKILLNAME:
				data.SetName(buff);
				break;
			case ATTACKPOWER:
				data.SetAttackPower(stoi(buff));
				break;
			case USEMANA:
				data.SetUseMana(stoi(buff));
				break;
			case AREAATTACK:
				data.SetAreaAttack(stoi(buff));
				break;
			case BONUSATTACK:
				data.SetBonusAttack(stoi(buff));
				break;
			case CRITICALATTACK:
				data.SetCriticalAttack(stoi(buff));
				break;
			}
		}
		SkillData.insert({ data.GetName(), data });
	}
	return SkillData;

}
