#pragma once
#include "Character.h" // Character 클래스 정의 필요

// 전방 선언
class Battle;
class Skill; // 반환 타입 위해 추가

class PlayerCharacter : public Character {
public:
    // 기본 생성자: 타입 설정
    PlayerCharacter() : Character() {
        charType = CharType::PLAYER;
    }

    // 파라미터 생성자 (기본 클래스 생성자 호출)
    // 불필요한 hp/mp 파라미터 제거, 초기값으로 maxHp/maxMp 사용
    PlayerCharacter(std::string name, int maxHp, int maxMp, int atk, int spd)
        : Character(std::move(name), maxHp, maxMp, atk, spd, CharType::PLAYER) {}

    // 복사 생성자 (DataTable 템플릿에서 인스턴스 생성 시 필요)
    PlayerCharacter(const Character& base) : Character(base) { // 기본 클래스 복사 수행
        charType = CharType::PLAYER; // 올바른 타입 보장

        // --- 중요: 복사 후 HP/MP를 Max 값으로 재설정 ---
        // 이는 DataTable의 템플릿 객체가 수정되었더라도 (수정되면 안 됨)
        // 전투 인스턴스가 항상 최대 체력/마나로 시작하도록 보장합니다.
        this->healthPoint = this->maxHealthPoint;
        this->manaPoint = this->maxManaPoint;
        this->isAlive = (this->healthPoint > 0); // HP 기반으로 생존 상태 재설정
        // -------------------------------------------------

        // PlayerCharacter 특정 멤버가 있다면 여기서 복사
    }

    // DecideAction 오버라이드 (플레이어 행동 로직, 사용된 스킬 또는 nullptr 반환)
    const Skill* DecideAction(Battle& battle) override;

    // 사용 가능한 스킬 목록 출력 (const 표시)
    void PrintSkills() const;

private:
    // 플레이어 특정 데이터 추가 가능 (예: 경험치, 레벨)

    // 0 코스트 스킬 찾는 헬퍼 함수 (내부용, const 표시)
    const Skill* FindZeroCostSkill() const;
};
