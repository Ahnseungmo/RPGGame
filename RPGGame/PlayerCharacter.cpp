#include "Framework.h" // ��� ��� ����
#include <limits> // numeric_limits ���
#include <iostream> // cout, cin, cerr ���
#include <vector>   // vector ���
#include <string>   // string ���

// using namespace std;

// 0 �ڽ�Ʈ ��ų ã�� ���� �Լ� ����
const Skill* PlayerCharacter::FindZeroCostSkill() const {
    DataTable& dataTable = DataTable::getInstance();
    for (const std::string& skillName : GetSkillNames()) {
        const Skill* skillPtr = dataTable.TryFindSkill(skillName);
        // ��ų�� �����ϰ� ���� �Ҹ� 0���� Ȯ��
        if (skillPtr && skillPtr->GetUseMana() == 0) {
            return skillPtr; // ã����
        }
    }
    // ������ ��ȿ�� �˻�� ��� ĳ���Ͱ� 0 �ڽ�Ʈ ��ų�� �������� �����Ѵٸ� ���� �����ϸ� �� ��
    std::cerr << "ġ���� ����: ĳ���� '" << GetName() << "'���� ���� �Ҹ� 0�� ��ų�� �����ϴ�!" << std::endl;
    return nullptr; // ���� ǥ��
}

// ��ų ��� ��� �Լ�
void PlayerCharacter::PrintSkills() const {
    std::cout << "--- ��� ������ ��ų ---" << std::endl;
    const auto& skills = GetSkillNames();
    if (skills.empty()) {
        std::cout << "��� ������ ��ų�� �����ϴ�." << std::endl;
        return;
    }

    DataTable& dataTable = DataTable::getInstance(); // �̱��� �ν��Ͻ�
    for (size_t i = 0; i < skills.size(); ++i) {
        const Skill* skillPtr = dataTable.TryFindSkill(skills[i]);
        std::cout << i + 1 << ". "; // ����ڿ� 1 ��� �ε���
        if (skillPtr) {
             // ��ų �� ���� ���
             std::cout << skillPtr->GetName()
                       << " (MP: " << skillPtr->GetUseMana()
                       << ", ����: " << skillPtr->GetPower(); // ª�� "����" ���
             if (skillPtr->IsAreaAttack()) std::cout << ", ����";
             if (skillPtr->IsCriticalAttack()) std::cout << ", ġ��Ÿ"; // ª�� "ġ��Ÿ" ���
             if (skillPtr->IsBonusAttack()) std::cout << ", �߰���"; // ª�� "�߰���" ���
             std::cout << ")" << std::endl;
        } else {
             // ��ų �����Ͱ� ���� ��� ��� (���� �ε� �� �߻� �� ��)
             std::cout << skills[i] << " (��ų ���� ����!)" << std::endl;
        }
    }
     std::cout << "----------------------" << std::endl;
}

// �÷��̾� �ൿ ���� �Լ� (����� ��ų ������ ��ȯ)
const Skill* PlayerCharacter::DecideAction(Battle& battle) {
    if (!IsAlive()) return nullptr; // ���������� �ൿ �Ұ�

    std::cout << "\n>>> [" << GetName() << "]�� �� <<<" << std::endl;
    PrintStatus(); // ���� ���� ���

    const auto& skillNames = GetSkillNames();
    if (skillNames.empty()) {
        std::cout << GetName() << "��(��) ����� ��ų�� ���� ���� �ѱ�ϴ�." << std::endl;
        return nullptr; // �ൿ ����
    }

    PrintSkills(); // ��� ������ ��ų ��� ���

    int skillChoice = 0;
    const Skill* selectedSkill = nullptr;
    bool skillChosen = false;

    // --- ��ų ���� ���� ---
    while (!skillChosen) {
        std::cout << "����� ��ų ��ȣ (1-" << skillNames.size() << ") �Ǵ� 0 (�� �ѱ��): ";
        // �Է� ��ȿ�� �˻�
        if (!(std::cin >> skillChoice)) {
            std::cout << "�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���." << std::endl;
            std::cin.clear(); // ���� �÷��� �ʱ�ȭ
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // �߸��� �Է� ������
            continue;
        }
        // ���� ������ ���� (��: "1 abc" �Է� ��)
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (skillChoice == 0) {
            std::cout << GetName() << "��(��) ���� �ѱ�ϴ�." << std::endl;
            return nullptr; // ����ڰ� �� �ѱ�� ����
        }

        if (skillChoice < 1 || skillChoice > static_cast<int>(skillNames.size())) {
            std::cout << "�߸��� ��ų ��ȣ�Դϴ�. 1�� " << skillNames.size() << " ������ ��ȣ�� �����ϼ���." << std::endl;
            continue;
        }

        // DataTable���� ���õ� ��ų ��������
        DataTable& dataTable = DataTable::getInstance();
        selectedSkill = dataTable.TryFindSkill(skillNames[skillChoice - 1]); // 0 ��� �ε���

        if (!selectedSkill) {
            // ������ ����ġ ��Ȳ
            std::cerr << "����: ������ ��ų '" << skillNames[skillChoice - 1] << "' �����͸� ã�� �� �����ϴ�! �ٽ� �����ϼ���." << std::endl;
            continue; // ����ڰ� �ٽ� �����ϵ��� ��
        }

        // ���� Ȯ��
        if (GetManaPoint() < selectedSkill->GetUseMana()) {
            std::cout << "������ �����մϴ�! (�ʿ� MP: " << selectedSkill->GetUseMana() << ", ���� MP: " << GetManaPoint() << ")" << std::endl;
            // ������ ��ų�� �̹� 0 �ڽ�Ʈ�� �ƴϰ�, 0 �ڽ�Ʈ ��ų�� �ִٸ� ��ü ��� ����
            const Skill* zeroCostSkill = (selectedSkill->GetUseMana() > 0) ? FindZeroCostSkill() : nullptr;
            if (zeroCostSkill) {
                char useZeroCostChoice = ' ';
                std::cout << "��� �⺻ ���� '" << zeroCostSkill->GetName() << "' (0 MP)�� ����Ͻðڽ��ϱ�? (y/n): ";
                std::cin >> useZeroCostChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ���� ����
                if (useZeroCostChoice == 'y' || useZeroCostChoice == 'Y') {
                    selectedSkill = zeroCostSkill; // 0 �ڽ�Ʈ ��ų�� ����
                    skillChosen = true; // �� ��ų�� ����
                } else {
                    std::cout << "�ٸ� ��ų�� �����ϰų� ���� �ѱ⼼��." << std::endl;
                    continue; // ��ų ���� ������Ʈ�� ���ư�
                }
            } else {
                 // ������ ���� 0 �ڽ�Ʈ ��ų�� ���� ��� (�Ǵ� ������ ��ų�� �̹� 0 �ڽ�Ʈ)
                 std::cout << "�� ��ų�� ����� �� �����ϴ�. �ٸ� ��ų�� �����ϰų� ���� �ѱ⼼��." << std::endl;
                 continue; // ��ų ���� ������Ʈ�� ���ư�
            }
        } else {
            // ���� ���
            skillChosen = true;
        }
    } // --- ��ų ���� ���� ���� ---

    // --- ��� ���� �� ��ų ���� ---
    bool skillUsedSuccessfully = false;
    if (selectedSkill->IsAreaAttack()) {
        // ��� ����ִ� �� ���
        std::vector<Character*> monsterTargets;
        for (Character* monster : battle.GetMonsterSide()) { // Battle���� ���� ��� ��������
            if (monster && monster->IsAlive()) {
                monsterTargets.push_back(monster);
            }
        }
        if (!monsterTargets.empty()) {
            skillUsedSuccessfully = UseSkill(*selectedSkill, monsterTargets); // ���� ��ų ���
        } else {
            std::cout << "���� ���� ����� �����ϴ�." << std::endl;
            // ��������� ��󿡰� "���"������ �ʾ�����, ������ Ȯ��/�Ҹ��.
            // �������� �������� ����. ������ ����ߴٸ� �������� ����.
            skillUsedSuccessfully = (GetManaPoint() + selectedSkill->GetUseMana() <= GetMaxManaPoint()); // ��� �� ������ �־����� Ȯ��
        }
    } else {
        // ���� ��� ��ų: ����ִ� �� �� �ϳ� ����
        auto& monsterSide = battle.GetMonsterSide();
        std::vector<Character*> aliveMonsters;
        std::cout << "--- ��� ���� ---" << std::endl;
        int aliveCount = 0;
        for (size_t i = 0; i < monsterSide.size(); ++i) {
            if (monsterSide[i] && monsterSide[i]->IsAlive()) {
                aliveCount++;
                std::cout << aliveCount << ". ";
                monsterSide[i]->PrintStatus(); // ��� ���� ǥ��
                aliveMonsters.push_back(monsterSide[i]);
            }
        }
         std::cout << "-----------------" << std::endl;


        if (aliveMonsters.empty()) {
            std::cout << "���� ���� ����� �����ϴ�." << std::endl;
            // ���� ���ݰ� �����ϰ� ó�� ����.
             skillUsedSuccessfully = (GetManaPoint() + selectedSkill->GetUseMana() <= GetMaxManaPoint());
        } else {
            int targetChoice = 0;
            bool targetChosen = false;
            // ��� ���� ����
            while(!targetChosen) {
                std::cout << "��� ��ȣ (1-" << aliveCount << "): ";
                if (!(std::cin >> targetChoice)) {
                    std::cout << "�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (targetChoice < 1 || targetChoice > aliveCount) {
                    std::cout << "�߸��� ��� ��ȣ�Դϴ�. 1�� " << aliveCount << " ������ ��ȣ�� �����ϼ���." << std::endl;
                    continue;
                }
                targetChosen = true;
            } // ��� ���� ���� ����

            Character* target = aliveMonsters[targetChoice - 1]; // ���õ� ��� ��������
            skillUsedSuccessfully = UseSkill(*selectedSkill, *target); // ���� ��� ��ų ���
        }
    }

    // ���������� ���� ��ų ������ ��ȯ, �ƴϸ� nullptr ��ȯ
    return skillUsedSuccessfully ? selectedSkill : nullptr;
}
