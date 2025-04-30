#include "Framework.h" // 모든 헤더 포함
#include <limits> // numeric_limits 사용
#include <iostream> // cout, cin, cerr 사용
#include <vector>   // vector 사용
#include <string>   // string 사용

// using namespace std;

// 0 코스트 스킬 찾는 헬퍼 함수 구현
const Skill* PlayerCharacter::FindZeroCostSkill() const {
    DataTable& dataTable = DataTable::getInstance();
    for (const std::string& skillName : GetSkillNames()) {
        const Skill* skillPtr = dataTable.TryFindSkill(skillName);
        // 스킬이 존재하고 마나 소모가 0인지 확인
        if (skillPtr && skillPtr->GetUseMana() == 0) {
            return skillPtr; // 찾았음
        }
    }
    // 데이터 유효성 검사로 모든 캐릭터가 0 코스트 스킬을 가지도록 보장한다면 여기 도달하면 안 됨
    std::cerr << "치명적 오류: 캐릭터 '" << GetName() << "'에게 마나 소모 0인 스킬이 없습니다!" << std::endl;
    return nullptr; // 실패 표시
}

// 스킬 목록 출력 함수
void PlayerCharacter::PrintSkills() const {
    std::cout << "--- 사용 가능한 스킬 ---" << std::endl;
    const auto& skills = GetSkillNames();
    if (skills.empty()) {
        std::cout << "사용 가능한 스킬이 없습니다." << std::endl;
        return;
    }

    DataTable& dataTable = DataTable::getInstance(); // 싱글톤 인스턴스
    for (size_t i = 0; i < skills.size(); ++i) {
        const Skill* skillPtr = dataTable.TryFindSkill(skills[i]);
        std::cout << i + 1 << ". "; // 사용자용 1 기반 인덱스
        if (skillPtr) {
             // 스킬 상세 정보 출력
             std::cout << skillPtr->GetName()
                       << " (MP: " << skillPtr->GetUseMana()
                       << ", 위력: " << skillPtr->GetPower(); // 짧게 "위력" 사용
             if (skillPtr->IsAreaAttack()) std::cout << ", 광역";
             if (skillPtr->IsCriticalAttack()) std::cout << ", 치명타"; // 짧게 "치명타" 사용
             if (skillPtr->IsBonusAttack()) std::cout << ", 추가턴"; // 짧게 "추가턴" 사용
             std::cout << ")" << std::endl;
        } else {
             // 스킬 데이터가 없는 경우 대비 (정상 로딩 시 발생 안 함)
             std::cout << skills[i] << " (스킬 정보 없음!)" << std::endl;
        }
    }
     std::cout << "----------------------" << std::endl;
}

// 플레이어 행동 결정 함수 (사용한 스킬 포인터 반환)
const Skill* PlayerCharacter::DecideAction(Battle& battle) {
    if (!IsAlive()) return nullptr; // 쓰러졌으면 행동 불가

    std::cout << "\n>>> [" << GetName() << "]의 턴 <<<" << std::endl;
    PrintStatus(); // 현재 상태 출력

    const auto& skillNames = GetSkillNames();
    if (skillNames.empty()) {
        std::cout << GetName() << "은(는) 사용할 스킬이 없어 턴을 넘깁니다." << std::endl;
        return nullptr; // 행동 없음
    }

    PrintSkills(); // 사용 가능한 스킬 목록 출력

    int skillChoice = 0;
    const Skill* selectedSkill = nullptr;
    bool skillChosen = false;

    // --- 스킬 선택 루프 ---
    while (!skillChosen) {
        std::cout << "사용할 스킬 번호 (1-" << skillNames.size() << ") 또는 0 (턴 넘기기): ";
        // 입력 유효성 검사
        if (!(std::cin >> skillChoice)) {
            std::cout << "잘못된 입력입니다. 숫자를 입력해주세요." << std::endl;
            std::cin.clear(); // 에러 플래그 초기화
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 잘못된 입력 버리기
            continue;
        }
        // 줄의 나머지 비우기 (예: "1 abc" 입력 시)
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (skillChoice == 0) {
            std::cout << GetName() << "이(가) 턴을 넘깁니다." << std::endl;
            return nullptr; // 사용자가 턴 넘기기 선택
        }

        if (skillChoice < 1 || skillChoice > static_cast<int>(skillNames.size())) {
            std::cout << "잘못된 스킬 번호입니다. 1과 " << skillNames.size() << " 사이의 번호를 선택하세요." << std::endl;
            continue;
        }

        // DataTable에서 선택된 스킬 가져오기
        DataTable& dataTable = DataTable::getInstance();
        selectedSkill = dataTable.TryFindSkill(skillNames[skillChoice - 1]); // 0 기반 인덱스

        if (!selectedSkill) {
            // 데이터 불일치 상황
            std::cerr << "오류: 선택한 스킬 '" << skillNames[skillChoice - 1] << "' 데이터를 찾을 수 없습니다! 다시 선택하세요." << std::endl;
            continue; // 사용자가 다시 선택하도록 함
        }

        // 마나 확인
        if (GetManaPoint() < selectedSkill->GetUseMana()) {
            std::cout << "마나가 부족합니다! (필요 MP: " << selectedSkill->GetUseMana() << ", 현재 MP: " << GetManaPoint() << ")" << std::endl;
            // 선택한 스킬이 이미 0 코스트가 아니고, 0 코스트 스킬이 있다면 대체 사용 제안
            const Skill* zeroCostSkill = (selectedSkill->GetUseMana() > 0) ? FindZeroCostSkill() : nullptr;
            if (zeroCostSkill) {
                char useZeroCostChoice = ' ';
                std::cout << "대신 기본 공격 '" << zeroCostSkill->GetName() << "' (0 MP)을 사용하시겠습니까? (y/n): ";
                std::cin >> useZeroCostChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
                if (useZeroCostChoice == 'y' || useZeroCostChoice == 'Y') {
                    selectedSkill = zeroCostSkill; // 0 코스트 스킬로 변경
                    skillChosen = true; // 이 스킬로 진행
                } else {
                    std::cout << "다른 스킬을 선택하거나 턴을 넘기세요." << std::endl;
                    continue; // 스킬 선택 프롬프트로 돌아감
                }
            } else {
                 // 마나도 없고 0 코스트 스킬도 없는 경우 (또는 선택한 스킬이 이미 0 코스트)
                 std::cout << "이 스킬을 사용할 수 없습니다. 다른 스킬을 선택하거나 턴을 넘기세요." << std::endl;
                 continue; // 스킬 선택 프롬프트로 돌아감
            }
        } else {
            // 마나 충분
            skillChosen = true;
        }
    } // --- 스킬 선택 루프 종료 ---

    // --- 대상 선택 및 스킬 실행 ---
    bool skillUsedSuccessfully = false;
    if (selectedSkill->IsAreaAttack()) {
        // 모든 살아있는 적 대상
        std::vector<Character*> monsterTargets;
        for (Character* monster : battle.GetMonsterSide()) { // Battle에서 몬스터 목록 가져오기
            if (monster && monster->IsAlive()) {
                monsterTargets.push_back(monster);
            }
        }
        if (!monsterTargets.empty()) {
            skillUsedSuccessfully = UseSkill(*selectedSkill, monsterTargets); // 광역 스킬 사용
        } else {
            std::cout << "광역 공격 대상이 없습니다." << std::endl;
            // 기술적으로 대상에게 "사용"되지는 않았지만, 마나는 확인/소모됨.
            // 성공으로 간주할지 결정. 마나가 충분했다면 성공으로 간주.
            skillUsedSuccessfully = (GetManaPoint() + selectedSkill->GetUseMana() <= GetMaxManaPoint()); // 사용 전 마나가 있었는지 확인
        }
    } else {
        // 단일 대상 스킬: 살아있는 적 중 하나 선택
        auto& monsterSide = battle.GetMonsterSide();
        std::vector<Character*> aliveMonsters;
        std::cout << "--- 대상 선택 ---" << std::endl;
        int aliveCount = 0;
        for (size_t i = 0; i < monsterSide.size(); ++i) {
            if (monsterSide[i] && monsterSide[i]->IsAlive()) {
                aliveCount++;
                std::cout << aliveCount << ". ";
                monsterSide[i]->PrintStatus(); // 대상 상태 표시
                aliveMonsters.push_back(monsterSide[i]);
            }
        }
         std::cout << "-----------------" << std::endl;


        if (aliveMonsters.empty()) {
            std::cout << "단일 공격 대상이 없습니다." << std::endl;
            // 광역 공격과 유사하게 처리 결정.
             skillUsedSuccessfully = (GetManaPoint() + selectedSkill->GetUseMana() <= GetMaxManaPoint());
        } else {
            int targetChoice = 0;
            bool targetChosen = false;
            // 대상 선택 루프
            while(!targetChosen) {
                std::cout << "대상 번호 (1-" << aliveCount << "): ";
                if (!(std::cin >> targetChoice)) {
                    std::cout << "잘못된 입력입니다. 숫자를 입력해주세요." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (targetChoice < 1 || targetChoice > aliveCount) {
                    std::cout << "잘못된 대상 번호입니다. 1과 " << aliveCount << " 사이의 번호를 선택하세요." << std::endl;
                    continue;
                }
                targetChosen = true;
            } // 대상 선택 루프 종료

            Character* target = aliveMonsters[targetChoice - 1]; // 선택된 대상 가져오기
            skillUsedSuccessfully = UseSkill(*selectedSkill, *target); // 단일 대상 스킬 사용
        }
    }

    // 성공적으로 사용된 스킬 포인터 반환, 아니면 nullptr 반환
    return skillUsedSuccessfully ? selectedSkill : nullptr;
}
