#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept> // std::runtime_error ���
#include <memory> // ������ �̷� ����Ʈ ������ ��� ����

// ���� ����
class PlayerCharacter;
class MonsterCharacter;
class Skill;

// CSV ������ �÷� �ε��� (Ÿ�� �������� �������� ���� enum class ���)
enum class PlayerCsvColumn : int {
    NAME = 0,
    HEALTH,
    MANA,
    ATTACK,
    SPEED,
    // SKILL_START // ��ų �̸��� �������̹Ƿ� ���� �ε��� ���� �����
};

enum class MonsterCsvColumn : int {
    NAME = 0,
    HEALTH,
    MANA,
    ATTACK,
    SPEED,
    // SKILL_START
};

enum class SkillCsvColumn : int {
    NAME = 0,
    POWER,
    MANA_COST,
    IS_AREA,
    BONUS_ATTACK, // BONUSATTACK���� �̸� ����
    CRITICAL_CHANCE // CRITICALATTACK���� �̸� ����
};


class DataTable {
public:
    // �̱��� �ν��Ͻ� ���� (Meyers' Singleton - ������ ����, �޸� ���� ����)
    static DataTable& getInstance() {
        static DataTable instance; // C++11���� ������ �����ϰ� �ʱ�ȭ��
        // ���� ���� �� ���⼭ ������ �ε��� Ʈ������ �� ����
        // if (!instance.loaded) instance.LoadAllData();
        return instance;
    }

    // ���� �� �̵� ������/���� ������ ���� (�̱��� ����)
    DataTable(const DataTable&) = delete;
    DataTable& operator=(const DataTable&) = delete;
    DataTable(DataTable&&) = delete;
    DataTable& operator=(DataTable&&) = delete;

    // ������ �ε� �Լ�
    void LoadPlayerData(const std::string& playerCharacterPath);
    void LoadMonsterData(const std::string& monsterCharacterPath);
    void LoadSkillData(const std::string& skillPath);
    // void LoadAllData(); // ��� �����͸� �� ���� �ε��ϴ� ������ �Լ�

    // ������ ���̺� Getter (���ʿ��� ���� ���� ���� const ���� ��ȯ)
    const std::unordered_map<std::string, PlayerCharacter>& GetPlayerCharacterTable() const;
    const std::unordered_map<std::string, MonsterCharacter>& GetMonsterCharacterTable() const;
    const std::unordered_map<std::string, Skill>& GetSkillTable() const;

    // Ư�� ������ �����ϰ� ã�� (const ���� ��ȯ, ������ ���� �߻�)
    const PlayerCharacter& FindPlayerCharacter(const std::string& name) const;
    const MonsterCharacter& FindMonsterCharacter(const std::string& name) const;
    const Skill& FindSkill(const std::string& name) const;

    // Ư�� ������ ã�� �õ� (const ������ ��ȯ, ������ nullptr ��ȯ)
    const PlayerCharacter* TryFindPlayerCharacter(const std::string& name) const;
    const MonsterCharacter* TryFindMonsterCharacter(const std::string& name) const;
    const Skill* TryFindSkill(const std::string& name) const;

private:
    // ������ �� �Ҹ��ڴ� private (�̱��� ����)
    DataTable() = default;
    ~DataTable() = default;

    // ������ ���� ���
    std::unordered_map<std::string, PlayerCharacter> playerCharacterTable;
    std::unordered_map<std::string, MonsterCharacter> monsterCharacterTable;
    std::unordered_map<std::string, Skill> skillTable;
    // bool loaded = false; // ������ �ε� ���� �÷��� (������)

    // CSV �Ľ� ���� �Լ� (���� ���� ��)
    std::vector<std::string> parseCsvLine(const std::string& line);
};

// ���� ������ ����: extern DataTable* dataTable;
