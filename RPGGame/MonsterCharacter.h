#pragma once
#include "Character.h" // Character 클래스 정의 필요

// 전방 선언
class Battle;
class Skill; // 반환 타입 위해 필요

class MonsterCharacter : public Character {
public:
    // 기본 생성자: 타입 설정
    MonsterCharacter() : Character() {
        charType = CharType::MONSTER;
    }

    // 파라미터 생성자 (기본 클래스 생성자 호출)
    // 불필요한 hp/mp 파라미터 제거
    MonsterCharacter(std::string name, int maxHp, int maxMp, int atk, int spd)
        : Character(std::move(name), maxHp, maxMp, atk, spd, CharType::MONSTER) {}

    // 복사 생성자 (DataTable 템플릿에서 인스턴스 생성 시 필요)
     MonsterCharacter(const Character& base) : Character(base) { // 기본 클래스 복사 수행
        charType = CharType::MONSTER; // 올바른 타입 보장

        // --- 중요: 복사 후 HP/MP를 Max 값으로 재설정 ---
        this->healthPoint = this->maxHealthPoint;
        this->manaPoint = this->maxManaPoint;
        this->isAlive = (this->healthPoint > 0); // HP 기반으로 생존 상태 재설정
        // -------------------------------------------------

        // MonsterCharacter 특정 멤버가 있다면 여기서 복사
    }

    // DecideAction 오버라이드 (몬스터 AI 로직, 사용된 스킬 또는 nullptr 반환)
    const Skill* DecideAction(Battle& battle) override;

private:
    // 몬스터 특정 데이터 추가 가능 (예: 드랍 아이템, 특수 패턴)

    // 0 코스트 스킬 찾는 헬퍼 함수 (내부용, const 표시)
    const Skill* FindZeroCostSkill() const;
};
