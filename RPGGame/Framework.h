#pragma once

// --- 표준 라이브러리 헤더 ---
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm> // sort, find, max, min, all_of, any_of, remove_if 등
#include <memory>    // unique_ptr, shared_ptr (리소스 관리 위해 사용 고려)
#include <stdexcept> // runtime_error, out_of_range, invalid_argument 등
#include <limits>    // numeric_limits (입력 스트림 처리용)
#include <cstdlib>   // rand, srand
#include <ctime>     // time (rand 시드용)
#include <utility>   // std::move

// --- 전방 선언 ---
// 컴파일 시간 단축 및 순환 참조 방지 위해 사용 가능,
// 하지만 아래에서 모든 헤더를 포함하므로 여기서는 필수는 아님.
class Skill;
class Character;
class PlayerCharacter;
class MonsterCharacter;
class DataTable;
class Battle;

// --- 프로젝트 헤더 포함 ---
// 의존성에 따라 올바른 포함 순서 보장
#include "Skill.h"          // Skill 정의
#include "DataTable.h"      // DataTable 정의 (Skill 전방 선언 필요)
#include "Character.h"      // Character 정의 (Skill, Battle 전방 선언 필요)
#include "PlayerCharacter.h"// PlayerCharacter 정의 (Character, Battle, Skill 필요)
#include "MonsterCharacter.h"// MonsterCharacter 정의 (Character, Battle, Skill 필요)
#include "Battle.h"         // Battle 정의 (Character, PlayerCharacter, MonsterCharacter 필요)

// --- 전역 네임스페이스 사용 ---
// 헤더 파일에서 'using namespace std;' 사용은 네임스페이스 오염 방지 위해 피함.
// .cpp 파일에서는 선호에 따라 사용 가능.

// --- 전역 변수 또는 함수 ---
// 전역 변수 최소화. DataTable 싱글톤이 전역 포인터 필요성 대체.
// extern DataTable* dataTable; // 제거됨, DataTable::getInstance() 사용
