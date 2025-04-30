#include "Framework.h" // ��� �ʿ��� ��� ����
#include <iostream>    // cout, cerr ���
#include <vector>      // vector ���
#include <string>      // string ���
#include <cstdlib>     // srand ���
#include <ctime>       // time ���
#include <stdexcept>   // ���� ó�� ���
#include <memory>      // unique_ptr ���
#include <vector>      // vector ���

// --- �Լ� ���� ---

// ���� ������ �ε� �Լ�
void LoadGameData(DataTable& dataTable);

// ��Ƽ ���� ���ø� �Լ�
template <typename CharType, typename TemplateType>
std::vector<std::unique_ptr<CharType>> CreateParty(
    DataTable& dataTable,
    const std::vector<std::string>& characterNames,
    const TemplateType* (DataTable::*findFunc)(const std::string&) const);

// ���� ���� �Լ� (���ڷ� ��Ƽ �̸� ��� �޵��� ����)
void RunGame(const std::vector<std::string>& playerNamesToCreate,
             const std::vector<std::string>& monsterNamesToCreate);

// --- main �Լ� ---

int main() {
    // ���α׷� ���� �� ���� ������ �õ� �� �� ����
    srand(static_cast<unsigned int>(time(0)));

    try {
        // ������ �÷��̾� �� ���� �̸� ��� ����
        const std::vector<std::string> playerNames = {"����", "�ü�", "����"};
        const std::vector<std::string> monsterNames = {"��ũ", "���", "������"};

        // RunGame �Լ��� �̸� ��� ����
        RunGame(playerNames, monsterNames);

    } catch (const std::exception& e) {
        // ǥ�� ���� ó��
        std::cerr << "\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        std::cerr << "���� �߻�: " << e.what() << std::endl;
        std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << std::endl;
        std::cout << "\n������ ���� ���α׷��� ����Ǿ����ϴ�." << std::endl;
        return 1; // ���� ǥ��
    } catch (...) {
        // �ٸ� ��� �� �� ���� ���� ó��
        std::cerr << "\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        std::cerr << "����ġ ���� ���� �߻�!" << std::endl;
        std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << std::endl;
        std::cout << "\n�� �� ���� ������ ���� ���α׷��� ����Ǿ����ϴ�." << std::endl;
        return 2; // ���� ǥ��
    }

    std::cout << "\n���α׷��� ���������� ����Ǿ����ϴ�." << std::endl;
    return 0; // ���� ǥ��
}

// --- �Լ� ���� ---

// ���� ������ �ε� �Լ� ����
void LoadGameData(DataTable& dataTable) {
    std::cout << "���� ������ �ε� ��..." << std::endl;
    // ĳ���Ͱ� ��ų �����Ϳ� �����ϹǷ� ��ų ���� �ε�
    dataTable.LoadSkillData("Data/SkillTable.csv");
    dataTable.LoadPlayerData("Data/PlayerTable.csv");
    dataTable.LoadMonsterData("Data/MonsterTable.csv");
    std::cout << "������ �ε� �Ϸ�.\n" << std::endl;
}

// ��Ƽ ���� ���ø� �Լ� ����
template <typename CharType, typename TemplateType>
std::vector<std::unique_ptr<CharType>> CreateParty(
    DataTable& dataTable,
    const std::vector<std::string>& characterNames,
    const TemplateType* (DataTable::*findFunc)(const std::string&) const)
{
    std::vector<std::unique_ptr<CharType>> party;
    party.reserve(characterNames.size()); // �̸� ���� Ȯ��

    // ��Ƽ Ÿ�Կ� ���� ��� �޽��� ����
    std::string partyTypeStr = std::is_same_v<CharType, PlayerCharacter> ? "�÷��̾�" : "����";
    std::cout << partyTypeStr << " ��Ƽ ���� ��..." << std::endl;

    for (const auto& name : characterNames) {
        const TemplateType* characterTemplate = (dataTable.*findFunc)(name);
        if (characterTemplate) {
            // ���� �����ڸ� ����Ͽ� unique_ptr ���� �� �߰�
            party.push_back(std::make_unique<CharType>(*characterTemplate));
            std::cout << "- �߰���: ";
            party.back()->PrintStatus(); // �߰��� ĳ���� ���� ǥ��
        } else {
            std::cerr << "���: ĳ���� ���ø� '" << name << "'��(��) DataTable���� ã�� �� �����ϴ�." << std::endl;
        }
    }
    std::cout << partyTypeStr << " ��Ƽ ���� �Ϸ�.\n" << std::endl;
    return party; // ������ ��Ƽ ��ȯ (RVO �Ǵ� �̵� �ø�ƽ�� Ȱ��)
}

// ���� ���� �Լ� ���� (���ڷ� ���� �̸� ��� ���)
void RunGame(const std::vector<std::string>& playerNamesToCreate,
             const std::vector<std::string>& monsterNamesToCreate) {
    // --- ������ �ε� ---
    DataTable& dataTable = DataTable::getInstance();
    LoadGameData(dataTable);

    // --- ��Ƽ ���� (���ڷ� ���� �̸� ��� ���) ---
    auto playerParty = CreateParty<PlayerCharacter, PlayerCharacter>(
        dataTable, playerNamesToCreate, &DataTable::TryFindPlayerCharacter);

    auto monsterParty = CreateParty<MonsterCharacter, MonsterCharacter>(
        dataTable, monsterNamesToCreate, &DataTable::TryFindMonsterCharacter);

    // --- ���� �� ��ȿ�� �˻� ---
    if (playerParty.empty()) {
        throw std::runtime_error("������ ������ �� �����ϴ�: �÷��̾� ��Ƽ�� ����ֽ��ϴ�.");
    }
    if (monsterParty.empty()) {
        throw std::runtime_error("������ ������ �� �����ϴ�: ���� ��Ƽ�� ����ֽ��ϴ�.");
    }

    // --- ���� ���� ---
    // Battle �����ڿ� unique_ptr ���͸� ���� ����
    Battle battle(playerParty, monsterParty);
    battle.StartFight(); // ���� �ùķ��̼� ����

    // --- ���� �� ���� ---
    // unique_ptr ���п� ������ ���� �ڵ� ���ʿ�
}
