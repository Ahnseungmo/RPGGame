#include "Framework.h" // Framework.h를 통해 모든 필요한 헤더 포함
#include <fstream>     // ifstream 필요
#include <sstream>     // stringstream 필요
#include <stdexcept>   // runtime_error, out_of_range, invalid_argument 필요
#include <iostream>    // cerr 필요

// --- DataTable 멤버 함수 구현 ---

// Getters는 헤더에 정의된 대로 const 참조 반환
const std::unordered_map<std::string, PlayerCharacter>& DataTable::GetPlayerCharacterTable() const {
    return playerCharacterTable;
}

const std::unordered_map<std::string, MonsterCharacter>& DataTable::GetMonsterCharacterTable() const {
    return monsterCharacterTable;
}

const std::unordered_map<std::string, Skill>& DataTable::GetSkillTable() const {
    return skillTable;
}

// Find 메서드는 찾지 못하면 예외 발생
const PlayerCharacter& DataTable::FindPlayerCharacter(const std::string& name) const {
    try {
        return playerCharacterTable.at(name); // .at()은 std::out_of_range 발생
    } catch (const std::out_of_range& oor) {
        throw std::runtime_error("DataTable에서 플레이어 캐릭터를 찾을 수 없습니다: " + name);
    }
}

const MonsterCharacter& DataTable::FindMonsterCharacter(const std::string& name) const {
    try {
        return monsterCharacterTable.at(name);
    } catch (const std::out_of_range& oor) {
        throw std::runtime_error("DataTable에서 몬스터 캐릭터를 찾을 수 없습니다: " + name);
    }
}

const Skill& DataTable::FindSkill(const std::string& name) const {
    try {
        return skillTable.at(name);
    } catch (const std::out_of_range& oor) {
        throw std::runtime_error("DataTable에서 스킬을 찾을 수 없습니다: " + name);
    }
}

// TryFind 메서드는 찾지 못하면 nullptr 반환
const PlayerCharacter* DataTable::TryFindPlayerCharacter(const std::string& name) const {
    auto it = playerCharacterTable.find(name);
    return (it != playerCharacterTable.end()) ? &(it->second) : nullptr;
}

const MonsterCharacter* DataTable::TryFindMonsterCharacter(const std::string& name) const {
    auto it = monsterCharacterTable.find(name);
    return (it != monsterCharacterTable.end()) ? &(it->second) : nullptr;
}

const Skill* DataTable::TryFindSkill(const std::string& name) const {
    auto it = skillTable.find(name);
    return (it != skillTable.end()) ? &(it->second) : nullptr;
}


// CSV 한 줄 파싱 헬퍼 함수
std::vector<std::string> DataTable::parseCsvLine(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ',')) {
        // 기본적인 앞뒤 공백 제거
        cell.erase(0, cell.find_first_not_of(" \t\n\r\f\v"));
        cell.erase(cell.find_last_not_of(" \t\n\r\f\v") + 1);
        result.push_back(cell);
    }
    return result;
}

// CSV에서 플레이어 데이터 로드
void DataTable::LoadPlayerData(const std::string& playerCharacterPath) {
    std::ifstream fs(playerCharacterPath);
    if (!fs.is_open()) {
        throw std::runtime_error("플레이어 데이터 파일 열기 실패: " + playerCharacterPath);
    }

    std::string line;
    // 헤더 줄 건너뛰기
    if (!std::getline(fs, line)) {
        std::cerr << "경고: 플레이어 데이터 파일이 비어있거나 헤더 읽기 실패: " << playerCharacterPath << std::endl;
        return;
    }

    int lineNumber = 1; // 헤더 다음부터 줄 번호 계산 시작
    while (std::getline(fs, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') continue; // 빈 줄과 주석 건너뛰기

        std::vector<std::string> cells = parseCsvLine(line);
        // 기본 스탯에 필요한 최소 컬럼 수 확인
        if (cells.size() < static_cast<int>(PlayerCsvColumn::SPEED) + 1) {
             std::cerr << "경고: " << playerCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 컬럼 부족 (발견: " << cells.size() << ", 최소 필요: " << static_cast<int>(PlayerCsvColumn::SPEED) + 1 << ")." << std::endl;
             continue;
        }

        try {
            std::string characterName = cells[static_cast<int>(PlayerCsvColumn::NAME)];
            if (characterName.empty()) {
                 std::cerr << "경고: " << playerCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 캐릭터 이름이 비어있습니다." << std::endl;
                 continue;
            }

            // 이름이 존재하지 않으면 emplace를 사용하여 PlayerCharacter를 제자리 생성
            auto result = playerCharacterTable.emplace(characterName, PlayerCharacter()); // 먼저 기본 생성
            bool inserted = result.second; // 삽입 발생 여부 (true면 삽입됨, false면 키가 이미 존재)
            auto it = result.first;       // 요소(새로 삽입되었거나 기존 요소)의 반복자

            if (inserted) {
                PlayerCharacter& playerData = it->second; // 새로 삽입된 객체 참조 가져오기

                // 스탯 파싱
                int health = std::stoi(cells[static_cast<int>(PlayerCsvColumn::HEALTH)]);
                int mana = std::stoi(cells[static_cast<int>(PlayerCsvColumn::MANA)]);
                int attack = std::stoi(cells[static_cast<int>(PlayerCsvColumn::ATTACK)]);
                int speed = std::stoi(cells[static_cast<int>(PlayerCsvColumn::SPEED)]);

                // setter를 사용하여 스탯 설정 (음수 값 방지 등 유효성 검사 보장)
                playerData.SetName(characterName); // 이름 명시적 설정
                playerData.SetMaxHealthPoint(health);
                playerData.SetHealthPoint(health); // 현재 HP 초기화
                playerData.SetMaxManaPoint(mana);
                playerData.SetManaPoint(mana);     // 현재 MP 초기화
                playerData.SetAttackPoint(attack);
                playerData.SetSpeedPoint(speed);

                // 스킬 추가 (나머지 컬럼)
                bool hasZeroCostSkill = false;
                for (size_t i = static_cast<size_t>(PlayerCsvColumn::SPEED) + 1; i < cells.size(); ++i) {
                    if (!cells[i].empty()) {
                        playerData.AddSkill(cells[i]);
                        // 이 스킬이 0 코스트인지 확인 (skillTable 로드 필요)
                        const Skill* skillPtr = TryFindSkill(cells[i]);
                        if(skillPtr && skillPtr->GetUseMana() == 0) {
                            hasZeroCostSkill = true;
                        } else if (!skillPtr) {
                             std::cerr << "경고: 플레이어 '" << characterName << "' (" << lineNumber << "번째 줄)의 스킬 '" << cells[i] << "'을(를) 스킬 테이블에서 찾을 수 없습니다!" << std::endl;
                        }
                    }
                }

                 // 최소 하나의 0 코스트 스킬이 추가되었는지 확인
                if (!hasZeroCostSkill) {
                     std::cerr << "오류: 플레이어 캐릭터 '" << characterName << "' (" << lineNumber << "번째 줄)은(는) 최소 하나의 마나 소모 0인 스킬을 가져야 합니다!" << std::endl;
                     // 선택 사항: 이 항목이 치명적인 오류인 경우 캐릭터 제거
                     // playerCharacterTable.erase(it);
                     // 또는 예외 발생
                     // throw std::runtime_error("플레이어 캐릭터 '" + characterName + "'에게 0 코스트 스킬이 없습니다.");
                }

            } else {
                 // 이름이 맵에 이미 존재함
                 std::cerr << "경고: 중복된 플레이어 캐릭터 이름 '" << characterName << "'이(가) " << playerCharacterPath << " 파일의 " << lineNumber << "번째 줄에서 발견되었습니다. 이 줄은 무시됩니다." << std::endl;
            }

        } catch (const std::invalid_argument& ia) {
            std::cerr << "경고: " << playerCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 잘못된 숫자 형식: " << ia.what() << " 내용: " << line << std::endl;
        } catch (const std::out_of_range& oor) {
            std::cerr << "경고: " << playerCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 숫자 범위 초과: " << oor.what() << " 내용: " << line << std::endl;
        } catch (const std::exception& e) {
             std::cerr << "경고: " << playerCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 오류: " << e.what() << " 내용: " << line << std::endl;
        }
    }
     std::cout << "플레이어 데이터 로드 성공: " << playerCharacterPath << std::endl;
}

// CSV에서 몬스터 데이터 로드 (LoadPlayerData와 구조 유사)
void DataTable::LoadMonsterData(const std::string& monsterCharacterPath) {
    std::ifstream fs(monsterCharacterPath);
    if (!fs.is_open()) {
        throw std::runtime_error("몬스터 데이터 파일 열기 실패: " + monsterCharacterPath);
    }
    std::string line;
    if (!std::getline(fs, line)) { // 헤더 건너뛰기
        std::cerr << "경고: 몬스터 데이터 파일이 비어있거나 헤더 읽기 실패: " << monsterCharacterPath << std::endl;
        return;
    }

    int lineNumber = 1;
    while (std::getline(fs, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') continue;

        std::vector<std::string> cells = parseCsvLine(line);
        if (cells.size() < static_cast<int>(MonsterCsvColumn::SPEED) + 1) {
             std::cerr << "경고: " << monsterCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 컬럼 부족." << std::endl;
             continue;
        }

        try {
            std::string characterName = cells[static_cast<int>(MonsterCsvColumn::NAME)];
             if (characterName.empty()) {
                 std::cerr << "경고: " << monsterCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 캐릭터 이름이 비어있습니다." << std::endl;
                 continue;
            }

            auto result = monsterCharacterTable.emplace(characterName, MonsterCharacter());
            bool inserted = result.second;
            auto it = result.first;

            if (inserted) {
                MonsterCharacter& monsterData = it->second;
                int health = std::stoi(cells[static_cast<int>(MonsterCsvColumn::HEALTH)]);
                int mana = std::stoi(cells[static_cast<int>(MonsterCsvColumn::MANA)]);
                int attack = std::stoi(cells[static_cast<int>(MonsterCsvColumn::ATTACK)]);
                int speed = std::stoi(cells[static_cast<int>(MonsterCsvColumn::SPEED)]);

                monsterData.SetName(characterName);
                monsterData.SetMaxHealthPoint(health);
                monsterData.SetHealthPoint(health);
                monsterData.SetMaxManaPoint(mana);
                monsterData.SetManaPoint(mana);
                monsterData.SetAttackPoint(attack);
                monsterData.SetSpeedPoint(speed);

                bool hasZeroCostSkill = false;
                for (size_t i = static_cast<size_t>(MonsterCsvColumn::SPEED) + 1; i < cells.size(); ++i) {
                     if (!cells[i].empty()) {
                        monsterData.AddSkill(cells[i]);
                        const Skill* skillPtr = TryFindSkill(cells[i]);
                        if(skillPtr && skillPtr->GetUseMana() == 0) {
                            hasZeroCostSkill = true;
                        } else if (!skillPtr) {
                             std::cerr << "경고: 몬스터 '" << characterName << "' (" << lineNumber << "번째 줄)의 스킬 '" << cells[i] << "'을(를) 스킬 테이블에서 찾을 수 없습니다!" << std::endl;
                        }
                     }
                }
                 if (!hasZeroCostSkill) {
                     std::cerr << "오류: 몬스터 캐릭터 '" << characterName << "' (" << lineNumber << "번째 줄)은(는) 최소 하나의 마나 소모 0인 스킬을 가져야 합니다!" << std::endl;
                     // monsterCharacterTable.erase(it); // 선택 사항: 잘못된 항목 제거
                 }
            } else {
                 std::cerr << "경고: 중복된 몬스터 캐릭터 이름 '" << characterName << "'이(가) " << monsterCharacterPath << " 파일의 " << lineNumber << "번째 줄에서 발견되었습니다. 이 줄은 무시됩니다." << std::endl;
            }

        } catch (const std::invalid_argument& ia) {
            std::cerr << "경고: " << monsterCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 잘못된 숫자 형식: " << ia.what() << " 내용: " << line << std::endl;
        } catch (const std::out_of_range& oor) {
            std::cerr << "경고: " << monsterCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 숫자 범위 초과: " << oor.what() << " 내용: " << line << std::endl;
        } catch (const std::exception& e) {
             std::cerr << "경고: " << monsterCharacterPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 오류: " << e.what() << " 내용: " << line << std::endl;
        }
    }
    std::cout << "몬스터 데이터 로드 성공: " << monsterCharacterPath << std::endl;
}

// CSV에서 스킬 데이터 로드
void DataTable::LoadSkillData(const std::string& skillPath) {
    std::ifstream fs(skillPath);
    if (!fs.is_open()) {
        throw std::runtime_error("스킬 데이터 파일 열기 실패: " + skillPath);
    }
    std::string line;
     if (!std::getline(fs, line)) { // 헤더 건너뛰기
        std::cerr << "경고: 스킬 데이터 파일이 비어있거나 헤더 읽기 실패: " << skillPath << std::endl;
        return;
    }

    int lineNumber = 1;
    while (std::getline(fs, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') continue;

        std::vector<std::string> cells = parseCsvLine(line);
        // 모든 스킬 속성에 필요한 최소 컬럼 수 확인
        if (cells.size() < static_cast<int>(SkillCsvColumn::CRITICAL_CHANCE) + 1) {
             std::cerr << "경고: " << skillPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 컬럼 부족 (발견: " << cells.size() << ", 최소 필요: " << static_cast<int>(SkillCsvColumn::CRITICAL_CHANCE) + 1 << ")." << std::endl;
             continue;
        }

        try {
            std::string skillName = cells[static_cast<int>(SkillCsvColumn::NAME)];
             if (skillName.empty()) {
                 std::cerr << "경고: " << skillPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 스킬 이름이 비어있습니다." << std::endl;
                 continue;
            }

            // 이름이 존재하지 않으면 emplace를 사용하여 Skill을 제자리 생성
            auto result = skillTable.emplace(skillName, Skill()); // 먼저 기본 생성
            bool inserted = result.second;
            auto it = result.first;

            if(inserted) {
                Skill& skillData = it->second; // 새로 삽입된 객체 참조 가져오기

                // 속성 파싱
                float power = std::stof(cells[static_cast<int>(SkillCsvColumn::POWER)]);
                int manaCost = std::stoi(cells[static_cast<int>(SkillCsvColumn::MANA_COST)]);
                // 불리언 플래그는 0/1을 bool로 변환
                bool isArea = (std::stoi(cells[static_cast<int>(SkillCsvColumn::IS_AREA)]) != 0);
                bool isBonus = (std::stoi(cells[static_cast<int>(SkillCsvColumn::BONUS_ATTACK)]) != 0);
                bool isCrit = (std::stoi(cells[static_cast<int>(SkillCsvColumn::CRITICAL_CHANCE)]) != 0);

                // setter를 사용하여 속성 설정
                skillData.SetName(skillName);
                skillData.SetPower(power);
                skillData.SetUseMana(manaCost);
                skillData.SetAreaAttack(isArea);
                skillData.SetBonusAttack(isBonus); // 올바른 setter 이름 사용
                skillData.SetCriticalAttack(isCrit); // 올바른 setter 이름 사용
            } else {
                 // 이름이 이미 존재함
                 std::cerr << "경고: 중복된 스킬 이름 '" << skillName << "'이(가) " << skillPath << " 파일의 " << lineNumber << "번째 줄에서 발견되었습니다. 이 줄은 무시됩니다." << std::endl;
            }

        } catch (const std::invalid_argument& ia) {
            std::cerr << "경고: " << skillPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 잘못된 숫자/형식: " << ia.what() << " 내용: " << line << std::endl;
        } catch (const std::out_of_range& oor) {
            std::cerr << "경고: " << skillPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 숫자 범위 초과: " << oor.what() << " 내용: " << line << std::endl;
        } catch (const std::exception& e) {
             std::cerr << "경고: " << skillPath << " 파일의 " << lineNumber << "번째 줄 건너뜀. 오류: " << e.what() << " 내용: " << line << std::endl;
        }
    }
    std::cout << "스킬 데이터 로드 성공: " << skillPath << std::endl;
}
