#pragma once

// --- ǥ�� ���̺귯�� ��� ---
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm> // sort, find, max, min, all_of, any_of, remove_if ��
#include <memory>    // unique_ptr, shared_ptr (���ҽ� ���� ���� ��� ���)
#include <stdexcept> // runtime_error, out_of_range, invalid_argument ��
#include <limits>    // numeric_limits (�Է� ��Ʈ�� ó����)
#include <cstdlib>   // rand, srand
#include <ctime>     // time (rand �õ��)
#include <utility>   // std::move

// --- ���� ���� ---
// ������ �ð� ���� �� ��ȯ ���� ���� ���� ��� ����,
// ������ �Ʒ����� ��� ����� �����ϹǷ� ���⼭�� �ʼ��� �ƴ�.
class Skill;
class Character;
class PlayerCharacter;
class MonsterCharacter;
class DataTable;
class Battle;

// --- ������Ʈ ��� ���� ---
// �������� ���� �ùٸ� ���� ���� ����
#include "Skill.h"          // Skill ����
#include "DataTable.h"      // DataTable ���� (Skill ���� ���� �ʿ�)
#include "Character.h"      // Character ���� (Skill, Battle ���� ���� �ʿ�)
#include "PlayerCharacter.h"// PlayerCharacter ���� (Character, Battle, Skill �ʿ�)
#include "MonsterCharacter.h"// MonsterCharacter ���� (Character, Battle, Skill �ʿ�)
#include "Battle.h"         // Battle ���� (Character, PlayerCharacter, MonsterCharacter �ʿ�)

// --- ���� ���ӽ����̽� ��� ---
// ��� ���Ͽ��� 'using namespace std;' ����� ���ӽ����̽� ���� ���� ���� ����.
// .cpp ���Ͽ����� ��ȣ�� ���� ��� ����.

// --- ���� ���� �Ǵ� �Լ� ---
// ���� ���� �ּ�ȭ. DataTable �̱����� ���� ������ �ʿ伺 ��ü.
// extern DataTable* dataTable; // ���ŵ�, DataTable::getInstance() ���
