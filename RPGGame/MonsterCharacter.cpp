#include "Framework.h" // ��� ��� ����
#include <iostream>    // cout, cerr ���
#include <vector>      // vector ���
#include <cstdlib>     // rand() ��� - srand()�� main���� �� �� ȣ��
#include <string>      // string ���

// using namespace std;

// 0 �ڽ�Ʈ ��ų ã�� ���� �Լ� ����
const Skill* MonsterCharacter::FindZeroCostSkill() const {
    DataTable& dataTable = DataTable::getInstance();
    for (const std::string& skillName : GetSkillNames()) {
        const Skill* skillPtr = dataTable.TryFindSkill(skillName);
        if (skillPtr && skillPtr->GetUseMana() == 0) {
            return skillPtr;
        }
    }
    // ���� �� ������ ���� �ǹ�
    std::cerr << "ġ���� ����: ���� '" << GetName() << "'���� ���� �Ҹ� 0�� ��ų�� �����ϴ�!" << std::endl;
    return nullptr;
}

// ���� �ൿ ���� �Լ� (����� ��ų ������ ��ȯ)
const Skill* MonsterCharacter::DecideAction(Battle& battle) {
    if (!IsAlive()) return nullptr; // ���������� �ൿ �Ұ�

    std::cout << "\n>>> [" << GetName() << "]�� �� <<<" << std::endl;
    // ���ʹ� ���� �ڽ��� ���¸� ������� ����, �ൿ�� ����� �� ����

    DataTable& dataTable = DataTable::getInstance();
    const auto& skillNames = GetSkillNames();
    if (skillNames.empty()) {
        std::cout << GetName() << "��(��) ����� ��ų�� ���� ���� �ѱ�ϴ�." << std::endl;
        return nullptr;
    }

    // --- AI ��ų ���� ---
    std::vector<const Skill*> usableSkills; // ���� ����� ��ų ���
    const Skill* zeroCostSkill = nullptr;   // ����� �⺻ ����

    // ��� ������ ��ų�� 0 �ڽ�Ʈ ��ų ã��
    for (const auto& name : skillNames) {
        const Skill* skillPtr = dataTable.TryFindSkill(name);
        if (skillPtr) {
            if (GetManaPoint() >= skillPtr->GetUseMana()) {
                usableSkills.push_back(skillPtr); // ���� ����ϸ� �߰�
            }
            if (skillPtr->GetUseMana() == 0) {
                zeroCostSkill = skillPtr; // 0 �ڽ�Ʈ ��ų ����
            }
        }
    }

    const Skill* skillToUse = nullptr;
    if (!usableSkills.empty()) {
        // ����� �� �ִ� ��ų�� �ִٸ�, �������� �ϳ� ����
        int skillIndex = rand() % usableSkills.size(); // ������ �ε���
        skillToUse = usableSkills[skillIndex];
    } else if (zeroCostSkill) {
        // �ٸ� ��ų�� ����� ������ ���ٸ�, 0 �ڽ�Ʈ ��ų ���
        std::cout << GetName() << "�� ������ �����Ͽ� �⺻ ������ ����մϴ�!" << std::endl;
        skillToUse = zeroCostSkill;
    } else {
        // 0 �ڽ�Ʈ ��ų���� ���� ��� (������ ��ȿ�� �˻�� ����Ǿ�� ��)
        std::cout << GetName() << "��(��) ����� �� �ִ� ��ų�� �����ϴ�! ���� �ѱ�ϴ�." << std::endl;
        return nullptr; // �ൿ �Ұ�
    }

    // --- AI ��� ���� ---
    auto& playerSide = battle.GetPlayerSide(); // Battle���� �÷��̾� ��� ��������
    std::vector<Character*> alivePlayers;
    for (Character* player : playerSide) {
        if (player && player->IsAlive()) {
            alivePlayers.push_back(player);
        }
    }

    if (alivePlayers.empty()) {
        // ���� ���� ���� ���ǿ��� ��������, ��������� ó��
        std::cout << GetName() << "��(��) �ൿ�Ϸ� ������ ����� �����ϴ�." << std::endl;
        return nullptr; // �ൿ �Ϸ� �Ұ�
    }

    // --- ��ų ���� ---
    bool skillUsedSuccessfully = false;
    if (skillToUse->IsAreaAttack()) {
        // ��� ����ִ� �÷��̾�� ���� ��ų ���
        skillUsedSuccessfully = UseSkill(*skillToUse, alivePlayers);
    } else {
        // ����ִ� �÷��̾� �� �������� �� ���� ���� ��� ��ų ���
        int targetIndex = rand() % alivePlayers.size();
        Character* target = alivePlayers[targetIndex];
        skillUsedSuccessfully = UseSkill(*skillToUse, *target);
    }

    // ���� �� ���� ��ų ��ȯ, �ƴϸ� nullptr ��ȯ
    return skillUsedSuccessfully ? skillToUse : nullptr;
}
