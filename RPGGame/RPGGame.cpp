#include "Framework.h"


int main()
{

	DataTable* dataTable = DataTable::get();
	dataTable->LoadPlayerData("PlayerTable.csv");
	dataTable->LoadMonsterData("MonsterTable.csv");
	dataTable->LoadSkillData("SkillTable.csv");
	cout << PlayerTable["전사"].GetAttackPoint() << endl;
	cout << MonsterTable["오크"].GetAttackPoint() << endl;
	cout << SkillTable["가로베기"].GetName() << endl;

	Character pc = PlayerTable["전사"];

	cout << pc.GetName();


	return 0;
}
