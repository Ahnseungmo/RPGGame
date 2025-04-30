#pragma once

#include <string>
#include <utility> // std::move

class Skill {
public:
    // 기본 생성자
    Skill() = default;

    // 파라미터 생성자 (일관된 이름 사용)
    Skill(std::string name, float power, int useMana, bool isAreaAttack, bool isBonusAttack, bool isCriticalAttack)
        : name(std::move(name)), power(power), useMana(useMana),
          isAreaAttack(isAreaAttack), isBonusAttack(isBonusAttack), isCriticalAttack(isCriticalAttack) {}

    // Getter 함수 (const 표시)
    const std::string& GetName() const { return name; }
    float GetPower() const { return power; }
    int GetUseMana() const { return useMana; }
    bool IsAreaAttack() const { return isAreaAttack; }
    bool IsBonusAttack() const { return isBonusAttack; }    // 일관된 이름
    bool IsCriticalAttack() const { return isCriticalAttack; } // 일관된 이름

    // Setter 함수 (데이터 로딩 시 필요)
    void SetName(std::string name) { this->name = std::move(name); }
    void SetPower(float power) { this->power = power; }
    void SetUseMana(int useMana) { this->useMana = useMana; }
    void SetAreaAttack(bool areaAttack) { this->isAreaAttack = areaAttack; }
    void SetBonusAttack(bool isBonusAttack) { this->isBonusAttack = isBonusAttack; }       // 일관된 이름
    void SetCriticalAttack(bool isCriticalAttack) { this->isCriticalAttack = isCriticalAttack; } // 일관된 이름

private:
    std::string name;
    float power = 0.0f;         // 스킬 위력 계수 또는 고정 데미지
    int useMana = 0;            // 소모 마나
    bool isAreaAttack = false;  // 광역 공격 여부
    bool isBonusAttack = false;    // 추가 턴 획득 여부
    bool isCriticalAttack = false; // 치명타 발생 가능 여부 (실제 확률은 다른 곳에서 결정)
};
