#pragma once

#include <vector>
#include <string>
#include <memory> // unique_ptr ��� ���� �߰�

// ���� ����
class Character;
class PlayerCharacter;
class MonsterCharacter;
class Skill;

class Battle {
public:
    // ������: unique_ptr ������ const ������ �޵��� ����
    Battle(const std::vector<std::unique_ptr<PlayerCharacter>>& playerParty,
           const std::vector<std::unique_ptr<MonsterCharacter>>& monsterParty);

    // �Ҹ���
    ~Battle() = default;

    // �� ���� ���� ���� �� ����
    void StartFight();

    // �ӵ� ���� �� ���̺� ���� (��������)
    void SortTurnTableBySpeed();
    // ���� �� ���� ��� (������)
    void PrintTurnTable() const;

    // ĳ���� ��� Getter (const ���� ��ȯ)
    const std::vector<Character*>& GetPlayerSide() const { return playerSide; }
    const std::vector<Character*>& GetMonsterSide() const { return monsterSide; }
    const std::vector<Character*>& GetTurnTable() const { return turnTable; }


private:
    // ���� ���� ĳ���� ������ ���� (Battle�� ������ ����)
    std::vector<Character*> playerSide;
    std::vector<Character*> monsterSide;
    std::vector<Character*> turnTable;

    // --- ���� ���� �Լ� ---
    void BuildTurnTable();
    bool CheckBattleEndCondition() const;
    void RemoveDeadCharactersFromTurnTable();
    const Skill* ProcessTurn(Character* currentCharacter);
};
