#include "Framework.h" // ��� ��� ����
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <string>
#include <memory> // unique_ptr ��� ���� �߰�

// ������: unique_ptr ���͸� �޾� ���� ���� ������ ���� �ʱ�ȭ
Battle::Battle(const std::vector<std::unique_ptr<PlayerCharacter>>& playerParty,
               const std::vector<std::unique_ptr<MonsterCharacter>>& monsterParty) {
    // �÷��̾� �� ���� ������ ���� �� ����
    playerSide.reserve(playerParty.size());
    for (const auto& p : playerParty) {
        if (p) {
            // unique_ptr���� ���� �����͸� ��� Character*�� ĳ���� �� ����
            playerSide.push_back(static_cast<Character*>(p.get()));
        } else {
            // �̷������� unique_ptr ���Ϳ��� null�� ���� ������, ��������� ó��
            std::cerr << "���: Battle �����ڿ� null PlayerCharacter unique_ptr�� �����Ǿ����ϴ�." << std::endl;
        }
    }

    // ���� �� ���� ������ ���� �� ����
    monsterSide.reserve(monsterParty.size());
    for (const auto& m : monsterParty) {
        if (m) {
            monsterSide.push_back(static_cast<Character*>(m.get()));
        } else {
            std::cerr << "���: Battle �����ڿ� null MonsterCharacter unique_ptr�� �����Ǿ����ϴ�." << std::endl;
        }
    }

    // �ʱ� �� ���� ����
    BuildTurnTable();
    SortTurnTableBySpeed();
}

// BuildTurnTable, SortTurnTableBySpeed, PrintTurnTable, CheckBattleEndCondition,
// RemoveDeadCharactersFromTurnTable, ProcessTurn, StartFight �Լ��� ������ �����ϰ� ����
// ... (���� Battle.cpp �ڵ� ���� �ٿ��ֱ�) ...

// ���� ����ִ� ĳ���ͷκ��� �� ���̺� ����
void Battle::BuildTurnTable() {
    turnTable.clear();
    // ���� ũ�� ����
    turnTable.reserve(playerSide.size() + monsterSide.size());

    // ����ִ� �÷��̾� �߰�
    for (Character* p : playerSide) {
        if (p && p->IsAlive()) { // null �� ���� ���� Ȯ��
            turnTable.push_back(p);
        }
    }
    // ����ִ� ���� �߰�
    for (Character* m : monsterSide) {
         if (m && m->IsAlive()) { // null �� ���� ���� Ȯ��
            turnTable.push_back(m);
         }
    }
}

// �ӵ� ���� �������� ����, ���� ó�� ����
void Battle::SortTurnTableBySpeed() {
    std::sort(turnTable.begin(), turnTable.end(), [](const Character* a, const Character* b) {
        // ������ null ������ ����� ó��
        if (!a) return false; // null�� ������
        if (!b) return true;

        // �� ����: �ӵ� ��������
        if (a->GetSpeedPoint() != b->GetSpeedPoint()) {
            return a->GetSpeedPoint() > b->GetSpeedPoint();
        }
        // ���� ó��: �÷��̾ ���ͺ��� ���� (���� enum �� �켱)
        return static_cast<int>(a->GetCharType()) < static_cast<int>(b->GetCharType());
        // �ʿ�� �߰� ���� ó�� ���� �߰� ���� (��: ���� ��ġ)
    });
}

// ���� �� ���� ��� (������)
void Battle::PrintTurnTable() const {
    std::cout << "\n--- �� ���� ---" << std::endl;
    if (turnTable.empty()) {
        std::cout << "(�� ������ ���� ĳ���� ����)" << std::endl;
    } else {
        int turnNum = 1;
        for (const auto* character : turnTable) {
            if (character) { // null ������ Ȯ��
                 std::cout << turnNum++ << ". " << character->GetName()
                           << " (�ӵ�: " << character->GetSpeedPoint()
                           << ", HP: " << character->GetHealthPoint() << ")" << std::endl;
            } else {
                 std::cout << turnNum++ << ". (�߸��� ĳ���� ������!)" << std::endl;
            }
        }
    }
    std::cout << "---------------" << std::endl;
}

// ���� ���� ���� Ȯ�� (�� �� ����)
bool Battle::CheckBattleEndCondition() const {
    // ��� �÷��̾ ���������� Ȯ�� (null ������ ó��)
    bool allPlayersDead = std::all_of(playerSide.begin(), playerSide.end(),
                                      [](const Character* p){ return !p || !p->IsAlive(); });

    // ��� ���Ͱ� ���������� Ȯ�� (null ������ ó��)
    bool allMonstersDead = std::all_of(monsterSide.begin(), monsterSide.end(),
                                       [](const Character* m){ return !m || !m->IsAlive(); });

    // ��� �� ���̶� �����ϸ� ���� ����
    return allPlayersDead || allMonstersDead;
}

// �� ���̺��� ������ ĳ���� ������ ����
void Battle::RemoveDeadCharactersFromTurnTable() {
    // erase-remove ���뱸 ���
    turnTable.erase(
        std::remove_if(turnTable.begin(), turnTable.end(),
                       [](const Character* c){ return !c || !c->IsAlive(); }), // ������: null�̰ų� ������� ������ ����
        turnTable.end() // erase�� �� �ݺ���
    );
}

// ���� ĳ���� �� ó��
const Skill* Battle::ProcessTurn(Character* currentCharacter) { // const Skill* ��ȯ
     // �ൿ �� ĳ���Ͱ� ��ȿ�ϰ� ����ִ��� Ȯ��
     if (!currentCharacter || !currentCharacter->IsAlive()) {
         return nullptr; // ĳ���� �ൿ �Ұ�
     }

     // �ൿ ������ ĳ���� ��ü�� ���� (������)
     // DecideAction�� ���� ��ų�� ��ȯ, �ൿ/��ų ��� �� ������ nullptr ��ȯ
     const Skill* skillUsed = currentCharacter->DecideAction(*this);

     return skillUsed; // ��ų ������ (�Ǵ� nullptr) ��ȯ
}

// �� ���� ����
void Battle::StartFight() {
    std::cout << "\n####################" << std::endl;
    std::cout << "#    ���� ����!    #" << std::endl;
    std::cout << "####################\n" << std::endl;
    PrintTurnTable(); // �ʱ� �� ���� ���

    int turnCount = 0;
    const int MAX_TURNS = 100; // ���� ���� ����

    // ���� ���� ���� ���� �Ǵ� �ִ� �� ���� �ñ��� �ݺ�
    while (!CheckBattleEndCondition() && turnCount < MAX_TURNS) {
        turnCount++;
        std::cout << "\n===== �� " << turnCount << " ���� =====" << std::endl;

        // --- ���� �� ���̺��� ĳ���͵� �� ó�� ---
        // �߰� ���� ��ȿ ������ ������ �� �����Ƿ� �ε��� ��� �ݺ�
        for (size_t i = 0; i < turnTable.size(); ++i) {
            Character* currentCharacter = turnTable[i];

            // �� ó�� �� ĳ���Ͱ� ������ ��ȿ�ϰ� ����ִ��� ��Ȯ��
            // (���� �� �� ���� �ൿ���� �������� �� ����)
            if (!currentCharacter || !currentCharacter->IsAlive()) {
                continue; // �������ų� ��ȿ���� ���� ĳ���� �ǳʶٱ�
            }

            // ĳ���� �� ó�� �� ���� ��ų �������� (�ִٸ�)
            const Skill* skillUsed = ProcessTurn(currentCharacter);

            // --- �� �ൿ �� ���� ���� ���� Ȯ�� ---
            if (CheckBattleEndCondition()) {
                goto battle_end; // ��ø ���� ��� Ż�� ���� goto ���
            }

            // --- �߰� �� ���� ó�� ---
            if (skillUsed && skillUsed->IsBonusAttack()) { // �ùٸ� �Լ� �̸� ���
                std::cout << "\n*** " << currentCharacter->GetName() << "��(��) �߰� ���� ������ϴ�! ***" << std::endl;

                // �߰� ���� ������ �� �ֵ��� ĳ���Ͱ� ������ ����ִ��� Ȯ��
                if (currentCharacter->IsAlive()) {
                    // ���� ī���� ���ҽ��� ���� �ݺ����� ���� ĳ���Ͱ� �ٽ� �ൿ�ϵ��� ��
                    i--;
                    std::cout << "(�߰� �� ����...)" << std::endl;
                    // continue; // ���� ���� �ݺ����� �ǳʶٱ� (�� ĳ���͸� �ٽ� ó��)
                } else {
                    std::cout << "(...������ " << currentCharacter->GetName() << "��(��) �ൿ ���� ���������ϴ�.)" << std::endl;
                    // ĳ���� ���, �߰� �� ���. ������ ���� ����.
                }
            }
             // --- �߰� �� ���� ���� ---

        } // --- ���� �� ���̺� ��ȸ ���� ---

        // --- �� ���� �ܰ� ---
        std::cout << "\n===== �� " << turnCount << " ���� =====" << std::endl;

        // �� ���� ������ ĳ���͵��� �� ���̺��� ����
        RemoveDeadCharactersFromTurnTable();

        // �ӵ��� ����� �� �ִٸ�(��: ����/�����) �� ���̺� ������
        // �ӵ��� �����̶�� ���� �� �� ���� �����ص� ���.
        // SortTurnTableBySpeed(); // �ӵ��� �������� ���� �� �ִٸ� �ּ� ����

        PrintTurnTable(); // ���� �� ���� ǥ��

        // ����� ������ ���� �Ͻ� ����
        std::cout << "\n(���� ������ �����Ϸ��� Enter Ű�� ��������...)";
        // ������ �ܿ� �Է� ���� Enter ���
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ���� ���� ����
        std::cin.get(); // Enter Ű �Է� ���

    } // --- �� ���� ���� ���� ---

    // ���� ���� ���� Ȯ��
    if (turnCount >= MAX_TURNS && !CheckBattleEndCondition()) {
        std::cout << "\n####################" << std::endl;
        std::cout << "#   ���� ���� ����!  #" << std::endl;
        std::cout << "#  (�ִ� �� ����)  #" << std::endl;
        std::cout << "####################\n" << std::endl;
        goto battle_summary; // ������� �ǳʶٱ�
    }


battle_end: // ���� ���� �� ���� Ż�� ����
    std::cout << "\n####################" << std::endl;
    std::cout << "#    ���� ����!    #" << std::endl;
    std::cout << "####################\n" << std::endl;

battle_summary: // ��� ǥ�� ����
    // ���� ����
    bool anyPlayerAlive = std::any_of(playerSide.begin(), playerSide.end(),
                                      [](const Character* p){ return p && p->IsAlive(); });
    bool anyMonsterAlive = std::any_of(monsterSide.begin(), monsterSide.end(),
                                       [](const Character* m){ return m && m->IsAlive(); });

    // ��� ��ǥ
    if (anyPlayerAlive && !anyMonsterAlive) {
        std::cout << "�ڡڡڡڡ� �¸�! �÷��̾� ��! �ڡڡڡڡ�" << std::endl;
    } else if (!anyPlayerAlive && anyMonsterAlive) {
        std::cout << "�١١١١� �й�! ���� ��... �١١١١�" << std::endl;
    } else if (!anyPlayerAlive && !anyMonsterAlive) {
        std::cout << "���������� ��ȣ �ı�! ���� ����... ����������" << std::endl;
    } else { // ���� ������ ���� (���� �Ǵ� ���º� ����)
        std::cout << "���������� ���º�! ������ ��� ���� �������ϴ�. ����������" << std::endl;
    }

    // ��� ������ ���� ���� ���
    std::cout << "\n--- ���� ���� ---" << std::endl;
     std::cout << "-- �÷��̾� --" << std::endl;
     for (const auto* p : playerSide) { if(p) p->PrintStatus(); }
     std::cout << "-- ���� --" << std::endl;
     for (const auto* m : monsterSide) { if(m) m->PrintStatus(); }
     std::cout << "-----------------" << std::endl;
}
