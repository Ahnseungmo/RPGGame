#include "Framework.h" // ��� ��� ����
#include <iostream>    // cout, cerr ���
#include <algorithm>   // std::find, std::max, std::min ���
#include <vector>      // std::vector �Ķ���� ���
#include <cstdlib>     // rand() ���

// using namespace std; // .cpp ���Ͽ����� �ʿ�� ��� ����

void Character::SetHealthPoint(int hp) {
    int previousHp = healthPoint;
    // HP�� 0�� maxHealthPoint ���̷� ����
    healthPoint = std::max(0, std::min(hp, maxHealthPoint));

    if (healthPoint <= 0 && isAlive) {
        // HP�� 0 ���ϰ� �Ǿ���, ĳ���Ͱ� ����־���
        Die(); // ��� ó��
    } else if (healthPoint > 0 && !isAlive && previousHp <= 0) {
        // HP�� ����� �Ǿ���, ĳ���Ͱ� �׾��־�����, ���� HP�� 0 ���Ͽ��� (�ʱ� ���� �� ��Ȱ ����)
        // �⺻���� ��Ȱ ����
        isAlive = true;
        std::cout << name << "��(��) ��Ȱ�߽��ϴ�! (HP: " << healthPoint << ")" << std::endl;
    }
    // HP�� 0 ��輱�� ���� �ʰ� ����� ���, isAlive�� ������� ����.
}

void Character::SetManaPoint(int mp) {
    // MP�� 0�� maxManaPoint ���̷� ����
    manaPoint = std::max(0, std::min(mp, maxManaPoint));
}

void Character::AddSkill(const std::string& skillName) {
    // �ߺ� ��ų �̸� �߰� ����
    if (std::find(skillNames.begin(), skillNames.end(), skillName) == skillNames.end()) {
        skillNames.push_back(skillName);
    } else {
        // �ߺ� �õ� �� ��� (������)
        // std::cerr << "���: ĳ���� '" << name << "'��(��) �̹� '" << skillName << "' ��ų�� ������ �ֽ��ϴ�." << std::endl;
    }
}

// �������� �޴� �Լ�
bool Character::TakeDamage(int damage) {
    if (!isAlive) return false; // ������ ĳ���ʹ� ������ ���� ����

    int actualDamage = std::max(0, damage); // ���� ������ (ȸ��) ����

    // �� ü�� �������� ������ ����
    SetHealthPoint(healthPoint - actualDamage);

    // ������ �޽��� ��ü�� UseSkill���� TakeDamage ȣ�� ���� ��µ�.
    // ���⼭�� ����� ���.
    std::cout << " (HP: " << healthPoint << "/" << maxHealthPoint << ")"; // ����/�ִ� HP ǥ��

    if (!isAlive) {
        std::cout << " - ������!"; // �������� ġ�����̾��� ��� �޽��� �߰�
    }
    std::cout << std::endl; // ������ ��� �� �ٹٲ�

    return isAlive; // ������ ��������� true, �ƴϸ� false ��ȯ
}

// ��ų ��� �Լ� (���� ���)
bool Character::UseSkill(const Skill& skill, Character& target) {
    if (!isAlive) {
        std::cout << name << "��(��) ������ �־� ��ų�� ����� �� �����ϴ�." << std::endl;
        return false;
    }
    if (!target.IsAlive()) {
        std::cout << target.GetName() << "��(��) �̹� ������ �ֽ��ϴ�." << std::endl;
        return false; // ������ ĳ���� ��� �Ұ�
    }
    if (skill.IsAreaAttack()) {
        // �� �Լ��� ���� ��� ��ų�� �����
        std::cerr << "���: ���� ��� UseSkill �Լ��� ���� ���� ��ų '" << skill.GetName() << "'�� ȣ��Ǿ����ϴ�. ���� �����ε带 ����ϼ���." << std::endl;
        return false;
    }

    // 1. ���� ��� Ȯ��
    if (manaPoint < skill.GetUseMana()) {
        std::cout << name << "�� ������ �����Ͽ� " << skill.GetName() << " ��ų�� ����� �� �����ϴ�."
                  << " (�ʿ� MP: " << skill.GetUseMana() << ", ���� MP: " << manaPoint << ")" << std::endl;
        return false; // ��ų ��� ����
    }

    // 2. ���� �Ҹ�
    SetManaPoint(manaPoint - skill.GetUseMana());
    std::cout << name << "��(��) " << target.GetName() << "���� " << skill.GetName() << " ���!"
              << " (MP: " << manaPoint << "/" << maxManaPoint << ")"; // ���� �Ҹ� �Ͻ��� ǥ��

    // 3. ������ ��� (�⺻ ġ��Ÿ ���� ����)
    int baseDamage = static_cast<int>(GetAttackPoint() * skill.GetPower());
    int finalDamage = baseDamage;
    bool isCritical = false;

    // ��ų�� ġ��Ÿ �����ϰ�, Ȯ�� ����
    if (skill.IsCriticalAttack()) { // Skill.h�� �ùٸ� getter ���
        // ������ 50% ġ��Ÿ Ȯ�� ����
        if (rand() % 2 == 0) {
            finalDamage *= 2; // ġ��Ÿ �� ������ 2��
            isCritical = true;
        }
    }
    finalDamage = std::max(0, finalDamage); // �������� ������ ���� �ʵ��� ����

    // 4. ��� ��� �� ������ ����
    if (isCritical) {
        std::cout << " ��ġ��Ÿ!��";
    }
    std::cout << " -> " << finalDamage << " �������� " << target.GetName() << "���� �������ϴ�.";

    target.TakeDamage(finalDamage); // ������ ���� (TakeDamage�� HP ��� �� ��� ó��)

    return true; // ��ų ��� ����
}

// ��ų ��� �Լ� (���� ���)
bool Character::UseSkill(const Skill& skill, std::vector<Character*>& targets) {
     if (!isAlive) {
         std::cout << name << "��(��) ������ �־� ��ų�� ����� �� �����ϴ�." << std::endl;
         return false;
     }
     if (!skill.IsAreaAttack()) {
         // �� �Լ��� ���� ��ų�� �����
         std::cerr << "���: ���� ��� UseSkill �Լ��� ���� ��� ��ų '" << skill.GetName() << "'�� ȣ��Ǿ����ϴ�. ���� ��� �����ε带 ����ϼ���." << std::endl;
         return false;
     }
     if (targets.empty()) {
         std::cout << name << "��(��) " << skill.GetName() << "��(��) ��������� ����� �����ϴ�!" << std::endl;
         // ��������� ��ų�� "���"�Ǿ�����(���� �Ҹ�), ȿ���� ������. true ��ȯ? false ��ȯ?
         // ������ �Ҹ��ϰ� �ൿ�� �������Ƿ� true�� ��ȯ.
         if (manaPoint < skill.GetUseMana()) { // ������ ���� ���� Ȯ��
            std::cout << name << "�� ������ �����Ͽ� " << skill.GetName() << " ��ų�� ����� �� �����ϴ�."
                      << " (�ʿ� MP: " << skill.GetUseMana() << ", ���� MP: " << manaPoint << ")" << std::endl;
            return false;
         }
         SetManaPoint(manaPoint - skill.GetUseMana());
         return true;
     }

     // 1. ���� ��� Ȯ��
     if (manaPoint < skill.GetUseMana()) {
         std::cout << name << "�� ������ �����Ͽ� " << skill.GetName() << " ��ų�� ����� �� �����ϴ�."
                   << " (�ʿ� MP: " << skill.GetUseMana() << ", ���� MP: " << manaPoint << ")" << std::endl;
         return false;
     }

     // 2. ���� �Ҹ�
     SetManaPoint(manaPoint - skill.GetUseMana());
     std::cout << name << "��(��) ���� ��ų " << skill.GetName() << " ���!"
               << " (MP: " << manaPoint << "/" << maxManaPoint << ")";

     // 3. ������ ��� (���� ��ų�� �� �� ���, ġ��Ÿ�� ��� Ÿ�ݿ� ����)
     int baseDamage = static_cast<int>(GetAttackPoint() * skill.GetPower());
     int finalDamage = baseDamage;
     bool isCritical = false;

     if (skill.IsCriticalAttack()) { // �ùٸ� getter ���
         if (rand() % 2 == 0) { // ������ 50% ġ��Ÿ Ȯ��
             finalDamage *= 2;
             isCritical = true;
         }
     }
     finalDamage = std::max(0, finalDamage);

     // 4. ��� ��� �� ��� ��ȿ ��󿡰� ������ ����
     if (isCritical) {
         std::cout << " ��ġ��Ÿ!��";
     }
     std::cout << " -> ���鿡�� " << finalDamage << " ������:" << std::endl;

     int hitCount = 0;
     for (Character* targetPtr : targets) {
         // ��� �� ��ȿ�ϰ� ����ִ� ��󿡰Ը� ������ ����
         if (targetPtr && targetPtr->IsAlive()) {
             std::cout << "    - " << targetPtr->GetName(); // ��� �̸� �鿩����
             targetPtr->TakeDamage(finalDamage); // ������ ���� (TakeDamage�� ��� ���)
             hitCount++;
         }
     }

     if (hitCount == 0) {
         std::cout << "    (��ȿ�� ����� �������ϴ�.)" << std::endl;
     }

     return true; // ��ų ��� ���� (���� Ȯ�� �� ����� ��������)
}

// ĳ���� ��� ó��
void Character::Die() {
    if (!isAlive) return; // �̹� �׾����� �ƹ��͵� �� ��

    std::cout << "--- " << name << "��(��) ���������ϴ�! ---" << std::endl;
    isAlive = false;
    healthPoint = 0; // HP�� ��Ȯ�� 0���� ����
    // ����: ����/�� �������� ĳ���� ���Ŵ� Battle Ŭ�������� ó����
}

// ĳ���� ���� ���
void Character::PrintStatus() const {
    std::string typeStr = "�� �� ����";
    switch (charType) {
        case CharType::PLAYER: typeStr = "�÷��̾�"; break;
        case CharType::MONSTER: typeStr = "����"; break;
        default: break; // "�� �� ����" ����
    }

    std::cout << "[" << name << " (" << typeStr << ")] "
              << "HP: " << healthPoint << "/" << maxHealthPoint
              << ", MP: " << manaPoint << "/" << maxManaPoint
              << ", ATK: " << attackPoint << ", SPD: " << speedPoint;
    if (!isAlive) {
        std::cout << " (������)";
    }
    std::cout << std::endl;
}
