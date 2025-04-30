#include "Framework.h" // Framework.h�� ���� ��� �ʿ��� ��� ����
#include <fstream>     // ifstream �ʿ�
#include <sstream>     // stringstream �ʿ�
#include <stdexcept>   // runtime_error, out_of_range, invalid_argument �ʿ�
#include <iostream>    // cerr �ʿ�

// --- DataTable ��� �Լ� ���� ---

// Getters�� ����� ���ǵ� ��� const ���� ��ȯ
const std::unordered_map<std::string, PlayerCharacter>& DataTable::GetPlayerCharacterTable() const {
    return playerCharacterTable;
}

const std::unordered_map<std::string, MonsterCharacter>& DataTable::GetMonsterCharacterTable() const {
    return monsterCharacterTable;
}

const std::unordered_map<std::string, Skill>& DataTable::GetSkillTable() const {
    return skillTable;
}

// Find �޼���� ã�� ���ϸ� ���� �߻�
const PlayerCharacter& DataTable::FindPlayerCharacter(const std::string& name) const {
    try {
        return playerCharacterTable.at(name); // .at()�� std::out_of_range �߻�
    } catch (const std::out_of_range& oor) {
        throw std::runtime_error("DataTable���� �÷��̾� ĳ���͸� ã�� �� �����ϴ�: " + name);
    }
}

const MonsterCharacter& DataTable::FindMonsterCharacter(const std::string& name) const {
    try {
        return monsterCharacterTable.at(name);
    } catch (const std::out_of_range& oor) {
        throw std::runtime_error("DataTable���� ���� ĳ���͸� ã�� �� �����ϴ�: " + name);
    }
}

const Skill& DataTable::FindSkill(const std::string& name) const {
    try {
        return skillTable.at(name);
    } catch (const std::out_of_range& oor) {
        throw std::runtime_error("DataTable���� ��ų�� ã�� �� �����ϴ�: " + name);
    }
}

// TryFind �޼���� ã�� ���ϸ� nullptr ��ȯ
const PlayerCharacter* DataTable::TryFindPlayerCharacter(const std::string& name) const {
    auto it = playerCharacterTable.find(name);
    return (it != playerCharacterTable.end()) ? &(it->second) : nullptr;
}

const MonsterCharacter* DataTable::TryFindMonsterCharacter(const std::string& name) const {
    auto it = monsterCharacterTable.find(name);
    return (it != monsterCharacterTable.end()) ? &(it->second) : nullptr;
}

const Skill* DataTable::TryFindSkill(const std::string& name) const {
    auto it = skillTable.find(name);
    return (it != skillTable.end()) ? &(it->second) : nullptr;
}


// CSV �� �� �Ľ� ���� �Լ�
std::vector<std::string> DataTable::parseCsvLine(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ',')) {
        // �⺻���� �յ� ���� ����
        cell.erase(0, cell.find_first_not_of(" \t\n\r\f\v"));
        cell.erase(cell.find_last_not_of(" \t\n\r\f\v") + 1);
        result.push_back(cell);
    }
    return result;
}

// CSV���� �÷��̾� ������ �ε�
void DataTable::LoadPlayerData(const std::string& playerCharacterPath) {
    std::ifstream fs(playerCharacterPath);
    if (!fs.is_open()) {
        throw std::runtime_error("�÷��̾� ������ ���� ���� ����: " + playerCharacterPath);
    }

    std::string line;
    // ��� �� �ǳʶٱ�
    if (!std::getline(fs, line)) {
        std::cerr << "���: �÷��̾� ������ ������ ����ְų� ��� �б� ����: " << playerCharacterPath << std::endl;
        return;
    }

    int lineNumber = 1; // ��� �������� �� ��ȣ ��� ����
    while (std::getline(fs, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') continue; // �� �ٰ� �ּ� �ǳʶٱ�

        std::vector<std::string> cells = parseCsvLine(line);
        // �⺻ ���ȿ� �ʿ��� �ּ� �÷� �� Ȯ��
        if (cells.size() < static_cast<int>(PlayerCsvColumn::SPEED) + 1) {
             std::cerr << "���: " << playerCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. �÷� ���� (�߰�: " << cells.size() << ", �ּ� �ʿ�: " << static_cast<int>(PlayerCsvColumn::SPEED) + 1 << ")." << std::endl;
             continue;
        }

        try {
            std::string characterName = cells[static_cast<int>(PlayerCsvColumn::NAME)];
            if (characterName.empty()) {
                 std::cerr << "���: " << playerCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. ĳ���� �̸��� ����ֽ��ϴ�." << std::endl;
                 continue;
            }

            // �̸��� �������� ������ emplace�� ����Ͽ� PlayerCharacter�� ���ڸ� ����
            auto result = playerCharacterTable.emplace(characterName, PlayerCharacter()); // ���� �⺻ ����
            bool inserted = result.second; // ���� �߻� ���� (true�� ���Ե�, false�� Ű�� �̹� ����)
            auto it = result.first;       // ���(���� ���ԵǾ��ų� ���� ���)�� �ݺ���

            if (inserted) {
                PlayerCharacter& playerData = it->second; // ���� ���Ե� ��ü ���� ��������

                // ���� �Ľ�
                int health = std::stoi(cells[static_cast<int>(PlayerCsvColumn::HEALTH)]);
                int mana = std::stoi(cells[static_cast<int>(PlayerCsvColumn::MANA)]);
                int attack = std::stoi(cells[static_cast<int>(PlayerCsvColumn::ATTACK)]);
                int speed = std::stoi(cells[static_cast<int>(PlayerCsvColumn::SPEED)]);

                // setter�� ����Ͽ� ���� ���� (���� �� ���� �� ��ȿ�� �˻� ����)
                playerData.SetName(characterName); // �̸� ����� ����
                playerData.SetMaxHealthPoint(health);
                playerData.SetHealthPoint(health); // ���� HP �ʱ�ȭ
                playerData.SetMaxManaPoint(mana);
                playerData.SetManaPoint(mana);     // ���� MP �ʱ�ȭ
                playerData.SetAttackPoint(attack);
                playerData.SetSpeedPoint(speed);

                // ��ų �߰� (������ �÷�)
                bool hasZeroCostSkill = false;
                for (size_t i = static_cast<size_t>(PlayerCsvColumn::SPEED) + 1; i < cells.size(); ++i) {
                    if (!cells[i].empty()) {
                        playerData.AddSkill(cells[i]);
                        // �� ��ų�� 0 �ڽ�Ʈ���� Ȯ�� (skillTable �ε� �ʿ�)
                        const Skill* skillPtr = TryFindSkill(cells[i]);
                        if(skillPtr && skillPtr->GetUseMana() == 0) {
                            hasZeroCostSkill = true;
                        } else if (!skillPtr) {
                             std::cerr << "���: �÷��̾� '" << characterName << "' (" << lineNumber << "��° ��)�� ��ų '" << cells[i] << "'��(��) ��ų ���̺��� ã�� �� �����ϴ�!" << std::endl;
                        }
                    }
                }

                 // �ּ� �ϳ��� 0 �ڽ�Ʈ ��ų�� �߰��Ǿ����� Ȯ��
                if (!hasZeroCostSkill) {
                     std::cerr << "����: �÷��̾� ĳ���� '" << characterName << "' (" << lineNumber << "��° ��)��(��) �ּ� �ϳ��� ���� �Ҹ� 0�� ��ų�� ������ �մϴ�!" << std::endl;
                     // ���� ����: �� �׸��� ġ������ ������ ��� ĳ���� ����
                     // playerCharacterTable.erase(it);
                     // �Ǵ� ���� �߻�
                     // throw std::runtime_error("�÷��̾� ĳ���� '" + characterName + "'���� 0 �ڽ�Ʈ ��ų�� �����ϴ�.");
                }

            } else {
                 // �̸��� �ʿ� �̹� ������
                 std::cerr << "���: �ߺ��� �÷��̾� ĳ���� �̸� '" << characterName << "'��(��) " << playerCharacterPath << " ������ " << lineNumber << "��° �ٿ��� �߰ߵǾ����ϴ�. �� ���� ���õ˴ϴ�." << std::endl;
            }

        } catch (const std::invalid_argument& ia) {
            std::cerr << "���: " << playerCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. �߸��� ���� ����: " << ia.what() << " ����: " << line << std::endl;
        } catch (const std::out_of_range& oor) {
            std::cerr << "���: " << playerCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. ���� ���� �ʰ�: " << oor.what() << " ����: " << line << std::endl;
        } catch (const std::exception& e) {
             std::cerr << "���: " << playerCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. ����: " << e.what() << " ����: " << line << std::endl;
        }
    }
     std::cout << "�÷��̾� ������ �ε� ����: " << playerCharacterPath << std::endl;
}

// CSV���� ���� ������ �ε� (LoadPlayerData�� ���� ����)
void DataTable::LoadMonsterData(const std::string& monsterCharacterPath) {
    std::ifstream fs(monsterCharacterPath);
    if (!fs.is_open()) {
        throw std::runtime_error("���� ������ ���� ���� ����: " + monsterCharacterPath);
    }
    std::string line;
    if (!std::getline(fs, line)) { // ��� �ǳʶٱ�
        std::cerr << "���: ���� ������ ������ ����ְų� ��� �б� ����: " << monsterCharacterPath << std::endl;
        return;
    }

    int lineNumber = 1;
    while (std::getline(fs, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') continue;

        std::vector<std::string> cells = parseCsvLine(line);
        if (cells.size() < static_cast<int>(MonsterCsvColumn::SPEED) + 1) {
             std::cerr << "���: " << monsterCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. �÷� ����." << std::endl;
             continue;
        }

        try {
            std::string characterName = cells[static_cast<int>(MonsterCsvColumn::NAME)];
             if (characterName.empty()) {
                 std::cerr << "���: " << monsterCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. ĳ���� �̸��� ����ֽ��ϴ�." << std::endl;
                 continue;
            }

            auto result = monsterCharacterTable.emplace(characterName, MonsterCharacter());
            bool inserted = result.second;
            auto it = result.first;

            if (inserted) {
                MonsterCharacter& monsterData = it->second;
                int health = std::stoi(cells[static_cast<int>(MonsterCsvColumn::HEALTH)]);
                int mana = std::stoi(cells[static_cast<int>(MonsterCsvColumn::MANA)]);
                int attack = std::stoi(cells[static_cast<int>(MonsterCsvColumn::ATTACK)]);
                int speed = std::stoi(cells[static_cast<int>(MonsterCsvColumn::SPEED)]);

                monsterData.SetName(characterName);
                monsterData.SetMaxHealthPoint(health);
                monsterData.SetHealthPoint(health);
                monsterData.SetMaxManaPoint(mana);
                monsterData.SetManaPoint(mana);
                monsterData.SetAttackPoint(attack);
                monsterData.SetSpeedPoint(speed);

                bool hasZeroCostSkill = false;
                for (size_t i = static_cast<size_t>(MonsterCsvColumn::SPEED) + 1; i < cells.size(); ++i) {
                     if (!cells[i].empty()) {
                        monsterData.AddSkill(cells[i]);
                        const Skill* skillPtr = TryFindSkill(cells[i]);
                        if(skillPtr && skillPtr->GetUseMana() == 0) {
                            hasZeroCostSkill = true;
                        } else if (!skillPtr) {
                             std::cerr << "���: ���� '" << characterName << "' (" << lineNumber << "��° ��)�� ��ų '" << cells[i] << "'��(��) ��ų ���̺��� ã�� �� �����ϴ�!" << std::endl;
                        }
                     }
                }
                 if (!hasZeroCostSkill) {
                     std::cerr << "����: ���� ĳ���� '" << characterName << "' (" << lineNumber << "��° ��)��(��) �ּ� �ϳ��� ���� �Ҹ� 0�� ��ų�� ������ �մϴ�!" << std::endl;
                     // monsterCharacterTable.erase(it); // ���� ����: �߸��� �׸� ����
                 }
            } else {
                 std::cerr << "���: �ߺ��� ���� ĳ���� �̸� '" << characterName << "'��(��) " << monsterCharacterPath << " ������ " << lineNumber << "��° �ٿ��� �߰ߵǾ����ϴ�. �� ���� ���õ˴ϴ�." << std::endl;
            }

        } catch (const std::invalid_argument& ia) {
            std::cerr << "���: " << monsterCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. �߸��� ���� ����: " << ia.what() << " ����: " << line << std::endl;
        } catch (const std::out_of_range& oor) {
            std::cerr << "���: " << monsterCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. ���� ���� �ʰ�: " << oor.what() << " ����: " << line << std::endl;
        } catch (const std::exception& e) {
             std::cerr << "���: " << monsterCharacterPath << " ������ " << lineNumber << "��° �� �ǳʶ�. ����: " << e.what() << " ����: " << line << std::endl;
        }
    }
    std::cout << "���� ������ �ε� ����: " << monsterCharacterPath << std::endl;
}

// CSV���� ��ų ������ �ε�
void DataTable::LoadSkillData(const std::string& skillPath) {
    std::ifstream fs(skillPath);
    if (!fs.is_open()) {
        throw std::runtime_error("��ų ������ ���� ���� ����: " + skillPath);
    }
    std::string line;
     if (!std::getline(fs, line)) { // ��� �ǳʶٱ�
        std::cerr << "���: ��ų ������ ������ ����ְų� ��� �б� ����: " << skillPath << std::endl;
        return;
    }

    int lineNumber = 1;
    while (std::getline(fs, line)) {
        lineNumber++;
        if (line.empty() || line[0] == '#') continue;

        std::vector<std::string> cells = parseCsvLine(line);
        // ��� ��ų �Ӽ��� �ʿ��� �ּ� �÷� �� Ȯ��
        if (cells.size() < static_cast<int>(SkillCsvColumn::CRITICAL_CHANCE) + 1) {
             std::cerr << "���: " << skillPath << " ������ " << lineNumber << "��° �� �ǳʶ�. �÷� ���� (�߰�: " << cells.size() << ", �ּ� �ʿ�: " << static_cast<int>(SkillCsvColumn::CRITICAL_CHANCE) + 1 << ")." << std::endl;
             continue;
        }

        try {
            std::string skillName = cells[static_cast<int>(SkillCsvColumn::NAME)];
             if (skillName.empty()) {
                 std::cerr << "���: " << skillPath << " ������ " << lineNumber << "��° �� �ǳʶ�. ��ų �̸��� ����ֽ��ϴ�." << std::endl;
                 continue;
            }

            // �̸��� �������� ������ emplace�� ����Ͽ� Skill�� ���ڸ� ����
            auto result = skillTable.emplace(skillName, Skill()); // ���� �⺻ ����
            bool inserted = result.second;
            auto it = result.first;

            if(inserted) {
                Skill& skillData = it->second; // ���� ���Ե� ��ü ���� ��������

                // �Ӽ� �Ľ�
                float power = std::stof(cells[static_cast<int>(SkillCsvColumn::POWER)]);
                int manaCost = std::stoi(cells[static_cast<int>(SkillCsvColumn::MANA_COST)]);
                // �Ҹ��� �÷��״� 0/1�� bool�� ��ȯ
                bool isArea = (std::stoi(cells[static_cast<int>(SkillCsvColumn::IS_AREA)]) != 0);
                bool isBonus = (std::stoi(cells[static_cast<int>(SkillCsvColumn::BONUS_ATTACK)]) != 0);
                bool isCrit = (std::stoi(cells[static_cast<int>(SkillCsvColumn::CRITICAL_CHANCE)]) != 0);

                // setter�� ����Ͽ� �Ӽ� ����
                skillData.SetName(skillName);
                skillData.SetPower(power);
                skillData.SetUseMana(manaCost);
                skillData.SetAreaAttack(isArea);
                skillData.SetBonusAttack(isBonus); // �ùٸ� setter �̸� ���
                skillData.SetCriticalAttack(isCrit); // �ùٸ� setter �̸� ���
            } else {
                 // �̸��� �̹� ������
                 std::cerr << "���: �ߺ��� ��ų �̸� '" << skillName << "'��(��) " << skillPath << " ������ " << lineNumber << "��° �ٿ��� �߰ߵǾ����ϴ�. �� ���� ���õ˴ϴ�." << std::endl;
            }

        } catch (const std::invalid_argument& ia) {
            std::cerr << "���: " << skillPath << " ������ " << lineNumber << "��° �� �ǳʶ�. �߸��� ����/����: " << ia.what() << " ����: " << line << std::endl;
        } catch (const std::out_of_range& oor) {
            std::cerr << "���: " << skillPath << " ������ " << lineNumber << "��° �� �ǳʶ�. ���� ���� �ʰ�: " << oor.what() << " ����: " << line << std::endl;
        } catch (const std::exception& e) {
             std::cerr << "���: " << skillPath << " ������ " << lineNumber << "��° �� �ǳʶ�. ����: " << e.what() << " ����: " << line << std::endl;
        }
    }
    std::cout << "��ų ������ �ε� ����: " << skillPath << std::endl;
}
