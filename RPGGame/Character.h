#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm> // std::max, std::min
#include <utility>   // std::move

// 전방 선언
class Skill;
class Battle; // DecideAction 파라미터 위해 필요

// 캐릭터 타입 enum (enum class 사용 권장)
enum class CharType {
    PLAYER,
    MONSTER,
    UNKNOWN // 기본값 또는 오류 상태
};

class Character {
public:
    // 기본 생성자: 기본값으로 초기화
    Character() : charType(CharType::UNKNOWN), isAlive(false) {}

    // 파라미터 생성자 (HP/MP는 Max 값으로 초기화)
    Character(std::string name, int maxHealthPoint, int maxManaPoint,
              int attackPoint, int speedPoint, CharType type)
        : name(std::move(name)),
          maxHealthPoint(std::max(0, maxHealthPoint)), // 음수 아닌 최대 HP 보장
          maxManaPoint(std::max(0, maxManaPoint)),     // 음수 아닌 최대 MP 보장
          attackPoint(std::max(0, attackPoint)),       // 음수 아닌 공격력 보장
          speedPoint(speedPoint),
          charType(type),
          healthPoint(this->maxHealthPoint), // 현재 HP를 최대 HP로 초기화
          manaPoint(this->maxManaPoint),     // 현재 MP를 최대 MP로 초기화
          isAlive(this->maxHealthPoint > 0)  // 초기 생존 상태는 최대 HP 기반
    {}

    // 가상 소멸자 (가상 함수가 있는 기본 클래스에 필수)
    virtual ~Character() = default;

    // --- Getters (const 표시, string/vector는 복사 방지 위해 const& 사용) ---
    const std::string& GetName() const { return name; }
    int GetHealthPoint() const { return healthPoint; }
    int GetMaxHealthPoint() const { return maxHealthPoint; }
    int GetManaPoint() const { return manaPoint; }
    int GetMaxManaPoint() const { return maxManaPoint; }
    int GetAttackPoint() const { return attackPoint; }
    int GetSpeedPoint() const { return speedPoint; }
    const std::vector<std::string>& GetSkillNames() const { return skillNames; } // const 참조 반환
    CharType GetCharType() const { return charType; }
    bool IsAlive() const { return isAlive; }

    // --- Setters (데이터 로딩 및 상태 변경용) ---
    void SetName(std::string name) { this->name = std::move(name); }
    void SetHealthPoint(int hp); // 로직에 사망 확인 포함
    void SetMaxHealthPoint(int maxHp) { this->maxHealthPoint = std::max(0, maxHp); } // 음수 방지
    void SetManaPoint(int mp); // 로직에 범위 확인 포함
    void SetMaxManaPoint(int maxMp) { this->maxManaPoint = std::max(0, maxMp); } // 음수 방지
    void SetAttackPoint(int ap) { this->attackPoint = std::max(0, ap); } // 음수 방지
    void SetSpeedPoint(int sp) { this->speedPoint = sp; }
    void AddSkill(const std::string& skillName); // 스킬 이름 추가 (중복 확인 가능)

    // --- 전투 관련 함수 ---
    // 데미지를 받는 함수 (캐릭터 사망 시 false 반환)
    virtual bool TakeDamage(int damage);

    // 스킬 사용 함수 (성공 시 true 반환)
    virtual bool UseSkill(const Skill& skill, Character& target); // 단일 대상
    virtual bool UseSkill(const Skill& skill, std::vector<Character*>& targets); // 광역 대상

    // 턴 시작 시 행동 결정 (사용한 스킬 포인터 반환, 없거나 실패 시 nullptr)
    // 파생 클래스에서 반드시 구현하도록 순수 가상 함수로 변경
    virtual const Skill* DecideAction(Battle& battle) = 0;

    // 캐릭터 상태 출력 (디버깅용, const 표시)
    virtual void PrintStatus() const;

protected: // 파생 클래스 접근 위해 protected로 변경
    std::string name;
    int healthPoint = 0;
    int maxHealthPoint = 0;
    int manaPoint = 0;
    int maxManaPoint = 0;
    int attackPoint = 0;
    int speedPoint = 0;
    CharType charType = CharType::UNKNOWN;
    bool isAlive = false; // 생성자에서 HP 기준으로 초기화됨
    std::vector<std::string> skillNames; // 스킬 이름 저장

    // 내부 상태 변경 함수 (protected)
    void Die(); // 사망 처리 로직
};
