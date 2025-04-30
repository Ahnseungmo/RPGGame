#pragma once

#include <vector>
#include <string>
#include <memory> // unique_ptr 사용 위해 추가

// 전방 선언
class Character;
class PlayerCharacter;
class MonsterCharacter;
class Skill;

class Battle {
public:
    // 생성자: unique_ptr 벡터의 const 참조를 받도록 수정
    Battle(const std::vector<std::unique_ptr<PlayerCharacter>>& playerParty,
           const std::vector<std::unique_ptr<MonsterCharacter>>& monsterParty);

    // 소멸자
    ~Battle() = default;

    // 주 전투 루프 시작 및 관리
    void StartFight();

    // 속도 기준 턴 테이블 정렬 (내림차순)
    void SortTurnTableBySpeed();
    // 현재 턴 순서 출력 (디버깅용)
    void PrintTurnTable() const;

    // 캐릭터 목록 Getter (const 참조 반환)
    const std::vector<Character*>& GetPlayerSide() const { return playerSide; }
    const std::vector<Character*>& GetMonsterSide() const { return monsterSide; }
    const std::vector<Character*>& GetTurnTable() const { return turnTable; }


private:
    // 전투 참여 캐릭터 포인터 저장 (Battle은 소유권 없음)
    std::vector<Character*> playerSide;
    std::vector<Character*> monsterSide;
    std::vector<Character*> turnTable;

    // --- 내부 헬퍼 함수 ---
    void BuildTurnTable();
    bool CheckBattleEndCondition() const;
    void RemoveDeadCharactersFromTurnTable();
    const Skill* ProcessTurn(Character* currentCharacter);
};
