#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

#include "Character.h"
#include "PlayerCharacter.h"
#include "MonsterCharacter.h"
#include "Skill.h"
#include "DataTable.h"


#define PlayerTable dataTable->GetPlayerCharacterTable()
#define MonsterTable dataTable->GetMonsterCharacterTable()
#define SkillTable dataTable->GetSkillTable()
