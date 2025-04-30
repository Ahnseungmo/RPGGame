#include "Framework.h" // 모든 필요한 헤더 포함
#include <iostream>    // cout, cerr 사용
#include <vector>      // vector 사용
#include <string>      // string 사용
#include <cstdlib>     // srand 사용
#include <ctime>       // time 사용
#include <stdexcept>   // 예외 처리 사용
#include <memory>      // unique_ptr 사용
#include <vector>      // vector 사용

// --- 함수 선언 ---

// 게임 데이터 로딩 함수
void LoadGameData(DataTable& dataTable);

// 파티 생성 템플릿 함수
template <typename CharType, typename TemplateType>
std::vector<std::unique_ptr<CharType>> CreateParty(
    DataTable& dataTable,
    const std::vector<std::string>& characterNames,
    const TemplateType* (DataTable::*findFunc)(const std::string&) const);

// 게임 실행 함수 (인자로 파티 이름 목록 받도록 수정)
void RunGame(const std::vector<std::string>& playerNamesToCreate,
             const std::vector<std::string>& monsterNamesToCreate);

// --- main 함수 ---

int main() {
    // 프로그램 시작 시 난수 생성기 시드 한 번 설정
    srand(static_cast<unsigned int>(time(0)));

    try {
        // 생성할 플레이어 및 몬스터 이름 목록 정의
        const std::vector<std::string> playerNames = {"전사", "궁수", "도적"};
        const std::vector<std::string> monsterNames = {"오크", "고블린", "슬라임"};

        // RunGame 함수에 이름 목록 전달
        RunGame(playerNames, monsterNames);

    } catch (const std::exception& e) {
        // 표준 예외 처리
        std::cerr << "\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        std::cerr << "오류 발생: " << e.what() << std::endl;
        std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << std::endl;
        std::cout << "\n오류로 인해 프로그램이 종료되었습니다." << std::endl;
        return 1; // 실패 표시
    } catch (...) {
        // 다른 모든 알 수 없는 예외 처리
        std::cerr << "\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        std::cerr << "예상치 못한 오류 발생!" << std::endl;
        std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << std::endl;
        std::cout << "\n알 수 없는 오류로 인해 프로그램이 종료되었습니다." << std::endl;
        return 2; // 실패 표시
    }

    std::cout << "\n프로그램이 성공적으로 종료되었습니다." << std::endl;
    return 0; // 성공 표시
}

// --- 함수 정의 ---

// 게임 데이터 로딩 함수 구현
void LoadGameData(DataTable& dataTable) {
    std::cout << "게임 데이터 로딩 중..." << std::endl;
    // 캐릭터가 스킬 데이터에 의존하므로 스킬 먼저 로드
    dataTable.LoadSkillData("Data/SkillTable.csv");
    dataTable.LoadPlayerData("Data/PlayerTable.csv");
    dataTable.LoadMonsterData("Data/MonsterTable.csv");
    std::cout << "데이터 로딩 완료.\n" << std::endl;
}

// 파티 생성 템플릿 함수 구현
template <typename CharType, typename TemplateType>
std::vector<std::unique_ptr<CharType>> CreateParty(
    DataTable& dataTable,
    const std::vector<std::string>& characterNames,
    const TemplateType* (DataTable::*findFunc)(const std::string&) const)
{
    std::vector<std::unique_ptr<CharType>> party;
    party.reserve(characterNames.size()); // 미리 공간 확보

    // 파티 타입에 따라 출력 메시지 변경
    std::string partyTypeStr = std::is_same_v<CharType, PlayerCharacter> ? "플레이어" : "몬스터";
    std::cout << partyTypeStr << " 파티 생성 중..." << std::endl;

    for (const auto& name : characterNames) {
        const TemplateType* characterTemplate = (dataTable.*findFunc)(name);
        if (characterTemplate) {
            // 복사 생성자를 사용하여 unique_ptr 생성 및 추가
            party.push_back(std::make_unique<CharType>(*characterTemplate));
            std::cout << "- 추가됨: ";
            party.back()->PrintStatus(); // 추가된 캐릭터 상태 표시
        } else {
            std::cerr << "경고: 캐릭터 템플릿 '" << name << "'을(를) DataTable에서 찾을 수 없습니다." << std::endl;
        }
    }
    std::cout << partyTypeStr << " 파티 생성 완료.\n" << std::endl;
    return party; // 생성된 파티 반환 (RVO 또는 이동 시맨틱스 활용)
}

// 게임 실행 함수 구현 (인자로 받은 이름 목록 사용)
void RunGame(const std::vector<std::string>& playerNamesToCreate,
             const std::vector<std::string>& monsterNamesToCreate) {
    // --- 데이터 로딩 ---
    DataTable& dataTable = DataTable::getInstance();
    LoadGameData(dataTable);

    // --- 파티 생성 (인자로 받은 이름 목록 사용) ---
    auto playerParty = CreateParty<PlayerCharacter, PlayerCharacter>(
        dataTable, playerNamesToCreate, &DataTable::TryFindPlayerCharacter);

    auto monsterParty = CreateParty<MonsterCharacter, MonsterCharacter>(
        dataTable, monsterNamesToCreate, &DataTable::TryFindMonsterCharacter);

    // --- 전투 전 유효성 검사 ---
    if (playerParty.empty()) {
        throw std::runtime_error("전투를 시작할 수 없습니다: 플레이어 파티가 비어있습니다.");
    }
    if (monsterParty.empty()) {
        throw std::runtime_error("전투를 시작할 수 없습니다: 몬스터 파티가 비어있습니다.");
    }

    // --- 전투 시작 ---
    // Battle 생성자에 unique_ptr 벡터를 직접 전달
    Battle battle(playerParty, monsterParty);
    battle.StartFight(); // 전투 시뮬레이션 실행

    // --- 전투 후 정리 ---
    // unique_ptr 덕분에 별도의 정리 코드 불필요
}
