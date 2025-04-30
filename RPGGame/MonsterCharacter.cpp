#include "Framework.h" // 모든 헤더 포함
#include <iostream>    // cout, cerr 사용
#include <vector>      // vector 사용
#include <cstdlib>     // rand() 사용 - srand()는 main에서 한 번 호출
#include <string>      // string 사용

// using namespace std;

// 0 코스트 스킬 찾는 헬퍼 함수 구현
const Skill* MonsterCharacter::FindZeroCostSkill() const {
    DataTable& dataTable = DataTable::getInstance();
    for (const std::string& skillName : GetSkillNames()) {
        const Skill* skillPtr = dataTable.TryFindSkill(skillName);
        if (skillPtr && skillPtr->GetUseMana() == 0) {
            return skillPtr;
        }
    }
    // 도달 시 데이터 오류 의미
    std::cerr << "치명적 오류: 몬스터 '" << GetName() << "'에게 마나 소모 0인 스킬이 없습니다!" << std::endl;
    return nullptr;
}

// 몬스터 행동 결정 함수 (사용한 스킬 포인터 반환)
const Skill* MonsterCharacter::DecideAction(Battle& battle) {
    if (!IsAlive()) return nullptr; // 쓰러졌으면 행동 불가

    std::cout << "\n>>> [" << GetName() << "]의 턴 <<<" << std::endl;
    // 몬스터는 보통 자신의 상태를 출력하지 않음, 행동만 출력할 수 있음

    DataTable& dataTable = DataTable::getInstance();
    const auto& skillNames = GetSkillNames();
    if (skillNames.empty()) {
        std::cout << GetName() << "은(는) 사용할 스킬이 없어 턴을 넘깁니다." << std::endl;
        return nullptr;
    }

    // --- AI 스킬 선택 ---
    std::vector<const Skill*> usableSkills; // 마나 충분한 스킬 목록
    const Skill* zeroCostSkill = nullptr;   // 보장된 기본 공격

    // 사용 가능한 스킬과 0 코스트 스킬 찾기
    for (const auto& name : skillNames) {
        const Skill* skillPtr = dataTable.TryFindSkill(name);
        if (skillPtr) {
            if (GetManaPoint() >= skillPtr->GetUseMana()) {
                usableSkills.push_back(skillPtr); // 마나 충분하면 추가
            }
            if (skillPtr->GetUseMana() == 0) {
                zeroCostSkill = skillPtr; // 0 코스트 스킬 저장
            }
        }
    }

    const Skill* skillToUse = nullptr;
    if (!usableSkills.empty()) {
        // 사용할 수 있는 스킬이 있다면, 무작위로 하나 선택
        int skillIndex = rand() % usableSkills.size(); // 무작위 인덱스
        skillToUse = usableSkills[skillIndex];
    } else if (zeroCostSkill) {
        // 다른 스킬을 사용할 마나가 없다면, 0 코스트 스킬 사용
        std::cout << GetName() << "의 마나가 부족하여 기본 공격을 사용합니다!" << std::endl;
        skillToUse = zeroCostSkill;
    } else {
        // 0 코스트 스킬조차 없는 경우 (데이터 유효성 검사로 보장되어야 함)
        std::cout << GetName() << "은(는) 사용할 수 있는 스킬이 없습니다! 턴을 넘깁니다." << std::endl;
        return nullptr; // 행동 불가
    }

    // --- AI 대상 선택 ---
    auto& playerSide = battle.GetPlayerSide(); // Battle에서 플레이어 목록 가져오기
    std::vector<Character*> alivePlayers;
    for (Character* player : playerSide) {
        if (player && player->IsAlive()) {
            alivePlayers.push_back(player);
        }
    }

    if (alivePlayers.empty()) {
        // 보통 전투 종료 조건에서 잡히지만, 방어적으로 처리
        std::cout << GetName() << "이(가) 행동하려 했지만 대상이 없습니다." << std::endl;
        return nullptr; // 행동 완료 불가
    }

    // --- 스킬 실행 ---
    bool skillUsedSuccessfully = false;
    if (skillToUse->IsAreaAttack()) {
        // 모든 살아있는 플레이어에게 광역 스킬 사용
        skillUsedSuccessfully = UseSkill(*skillToUse, alivePlayers);
    } else {
        // 살아있는 플레이어 중 무작위로 한 명에게 단일 대상 스킬 사용
        int targetIndex = rand() % alivePlayers.size();
        Character* target = alivePlayers[targetIndex];
        skillUsedSuccessfully = UseSkill(*skillToUse, *target);
    }

    // 성공 시 사용된 스킬 반환, 아니면 nullptr 반환
    return skillUsedSuccessfully ? skillToUse : nullptr;
}
