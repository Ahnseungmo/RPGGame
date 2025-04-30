#include "Framework.h" // 모든 헤더 포함
#include <iostream>    // cout, cerr 사용
#include <algorithm>   // std::find, std::max, std::min 사용
#include <vector>      // std::vector 파라미터 사용
#include <cstdlib>     // rand() 사용

// using namespace std; // .cpp 파일에서는 필요시 사용 가능

void Character::SetHealthPoint(int hp) {
    int previousHp = healthPoint;
    // HP를 0과 maxHealthPoint 사이로 제한
    healthPoint = std::max(0, std::min(hp, maxHealthPoint));

    if (healthPoint <= 0 && isAlive) {
        // HP가 0 이하가 되었고, 캐릭터가 살아있었음
        Die(); // 사망 처리
    } else if (healthPoint > 0 && !isAlive && previousHp <= 0) {
        // HP가 양수가 되었고, 캐릭터가 죽어있었으며, 이전 HP가 0 이하였음 (초기 설정 시 부활 방지)
        // 기본적인 부활 조건
        isAlive = true;
        std::cout << name << "이(가) 부활했습니다! (HP: " << healthPoint << ")" << std::endl;
    }
    // HP가 0 경계선을 넘지 않고 변경된 경우, isAlive는 변경되지 않음.
}

void Character::SetManaPoint(int mp) {
    // MP를 0과 maxManaPoint 사이로 제한
    manaPoint = std::max(0, std::min(mp, maxManaPoint));
}

void Character::AddSkill(const std::string& skillName) {
    // 중복 스킬 이름 추가 방지
    if (std::find(skillNames.begin(), skillNames.end(), skillName) == skillNames.end()) {
        skillNames.push_back(skillName);
    } else {
        // 중복 시도 시 경고 (선택적)
        // std::cerr << "경고: 캐릭터 '" << name << "'은(는) 이미 '" << skillName << "' 스킬을 가지고 있습니다." << std::endl;
    }
}

// 데미지를 받는 함수
bool Character::TakeDamage(int damage) {
    if (!isAlive) return false; // 쓰러진 캐릭터는 데미지 받지 않음

    int actualDamage = std::max(0, damage); // 음수 데미지 (회복) 방지

    // 새 체력 설정으로 데미지 적용
    SetHealthPoint(healthPoint - actualDamage);

    // 데미지 메시지 자체는 UseSkill에서 TakeDamage 호출 전에 출력됨.
    // 여기서는 결과만 출력.
    std::cout << " (HP: " << healthPoint << "/" << maxHealthPoint << ")"; // 현재/최대 HP 표시

    if (!isAlive) {
        std::cout << " - 쓰러짐!"; // 데미지가 치명적이었을 경우 메시지 추가
    }
    std::cout << std::endl; // 데미지 결과 후 줄바꿈

    return isAlive; // 여전히 살아있으면 true, 아니면 false 반환
}

// 스킬 사용 함수 (단일 대상)
bool Character::UseSkill(const Skill& skill, Character& target) {
    if (!isAlive) {
        std::cout << name << "은(는) 쓰러져 있어 스킬을 사용할 수 없습니다." << std::endl;
        return false;
    }
    if (!target.IsAlive()) {
        std::cout << target.GetName() << "은(는) 이미 쓰러져 있습니다." << std::endl;
        return false; // 쓰러진 캐릭터 대상 불가
    }
    if (skill.IsAreaAttack()) {
        // 이 함수는 단일 대상 스킬을 기대함
        std::cerr << "경고: 단일 대상 UseSkill 함수가 광역 공격 스킬 '" << skill.GetName() << "'로 호출되었습니다. 벡터 오버로드를 사용하세요." << std::endl;
        return false;
    }

    // 1. 마나 비용 확인
    if (manaPoint < skill.GetUseMana()) {
        std::cout << name << "의 마나가 부족하여 " << skill.GetName() << " 스킬을 사용할 수 없습니다."
                  << " (필요 MP: " << skill.GetUseMana() << ", 현재 MP: " << manaPoint << ")" << std::endl;
        return false; // 스킬 사용 실패
    }

    // 2. 마나 소모
    SetManaPoint(manaPoint - skill.GetUseMana());
    std::cout << name << "이(가) " << target.GetName() << "에게 " << skill.GetName() << " 사용!"
              << " (MP: " << manaPoint << "/" << maxManaPoint << ")"; // 마나 소모 암시적 표시

    // 3. 데미지 계산 (기본 치명타 로직 포함)
    int baseDamage = static_cast<int>(GetAttackPoint() * skill.GetPower());
    int finalDamage = baseDamage;
    bool isCritical = false;

    // 스킬이 치명타 가능하고, 확률 굴림
    if (skill.IsCriticalAttack()) { // Skill.h의 올바른 getter 사용
        // 간단한 50% 치명타 확률 예시
        if (rand() % 2 == 0) {
            finalDamage *= 2; // 치명타 시 데미지 2배
            isCritical = true;
        }
    }
    finalDamage = std::max(0, finalDamage); // 데미지가 음수가 되지 않도록 보장

    // 4. 결과 출력 및 데미지 적용
    if (isCritical) {
        std::cout << " ★치명타!★";
    }
    std::cout << " -> " << finalDamage << " 데미지를 " << target.GetName() << "에게 입혔습니다.";

    target.TakeDamage(finalDamage); // 데미지 적용 (TakeDamage가 HP 결과 및 사망 처리)

    return true; // 스킬 사용 성공
}

// 스킬 사용 함수 (광역 대상)
bool Character::UseSkill(const Skill& skill, std::vector<Character*>& targets) {
     if (!isAlive) {
         std::cout << name << "은(는) 쓰러져 있어 스킬을 사용할 수 없습니다." << std::endl;
         return false;
     }
     if (!skill.IsAreaAttack()) {
         // 이 함수는 광역 스킬을 기대함
         std::cerr << "경고: 광역 대상 UseSkill 함수가 단일 대상 스킬 '" << skill.GetName() << "'로 호출되었습니다. 단일 대상 오버로드를 사용하세요." << std::endl;
         return false;
     }
     if (targets.empty()) {
         std::cout << name << "이(가) " << skill.GetName() << "을(를) 사용했지만 대상이 없습니다!" << std::endl;
         // 기술적으로 스킬은 "사용"되었지만(마나 소모), 효과는 없었음. true 반환? false 반환?
         // 마나를 소모하고 행동을 취했으므로 true를 반환.
         if (manaPoint < skill.GetUseMana()) { // 여전히 마나 먼저 확인
            std::cout << name << "의 마나가 부족하여 " << skill.GetName() << " 스킬을 사용할 수 없습니다."
                      << " (필요 MP: " << skill.GetUseMana() << ", 현재 MP: " << manaPoint << ")" << std::endl;
            return false;
         }
         SetManaPoint(manaPoint - skill.GetUseMana());
         return true;
     }

     // 1. 마나 비용 확인
     if (manaPoint < skill.GetUseMana()) {
         std::cout << name << "의 마나가 부족하여 " << skill.GetName() << " 스킬을 사용할 수 없습니다."
                   << " (필요 MP: " << skill.GetUseMana() << ", 현재 MP: " << manaPoint << ")" << std::endl;
         return false;
     }

     // 2. 마나 소모
     SetManaPoint(manaPoint - skill.GetUseMana());
     std::cout << name << "이(가) 광역 스킬 " << skill.GetName() << " 사용!"
               << " (MP: " << manaPoint << "/" << maxManaPoint << ")";

     // 3. 데미지 계산 (광역 스킬은 한 번 계산, 치명타는 모든 타격에 적용)
     int baseDamage = static_cast<int>(GetAttackPoint() * skill.GetPower());
     int finalDamage = baseDamage;
     bool isCritical = false;

     if (skill.IsCriticalAttack()) { // 올바른 getter 사용
         if (rand() % 2 == 0) { // 간단한 50% 치명타 확률
             finalDamage *= 2;
             isCritical = true;
         }
     }
     finalDamage = std::max(0, finalDamage);

     // 4. 결과 출력 및 모든 유효 대상에게 데미지 적용
     if (isCritical) {
         std::cout << " ★치명타!★";
     }
     std::cout << " -> 대상들에게 " << finalDamage << " 데미지:" << std::endl;

     int hitCount = 0;
     for (Character* targetPtr : targets) {
         // 목록 내 유효하고 살아있는 대상에게만 데미지 적용
         if (targetPtr && targetPtr->IsAlive()) {
             std::cout << "    - " << targetPtr->GetName(); // 대상 이름 들여쓰기
             targetPtr->TakeDamage(finalDamage); // 데미지 적용 (TakeDamage가 결과 출력)
             hitCount++;
         }
     }

     if (hitCount == 0) {
         std::cout << "    (유효한 대상이 없었습니다.)" << std::endl;
     }

     return true; // 스킬 사용 성공 (마나 확인 후 대상이 없었더라도)
}

// 캐릭터 사망 처리
void Character::Die() {
    if (!isAlive) return; // 이미 죽었으면 아무것도 안 함

    std::cout << "--- " << name << "이(가) 쓰러졌습니다! ---" << std::endl;
    isAlive = false;
    healthPoint = 0; // HP를 정확히 0으로 설정
    // 참고: 전투/턴 순서에서 캐릭터 제거는 Battle 클래스에서 처리함
}

// 캐릭터 상태 출력
void Character::PrintStatus() const {
    std::string typeStr = "알 수 없음";
    switch (charType) {
        case CharType::PLAYER: typeStr = "플레이어"; break;
        case CharType::MONSTER: typeStr = "몬스터"; break;
        default: break; // "알 수 없음" 유지
    }

    std::cout << "[" << name << " (" << typeStr << ")] "
              << "HP: " << healthPoint << "/" << maxHealthPoint
              << ", MP: " << manaPoint << "/" << maxManaPoint
              << ", ATK: " << attackPoint << ", SPD: " << speedPoint;
    if (!isAlive) {
        std::cout << " (쓰러짐)";
    }
    std::cout << std::endl;
}
