#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept> // std::runtime_error 사용
#include <memory> // 잠재적 미래 스마트 포인터 사용 위해

// 전방 선언
class PlayerCharacter;
class MonsterCharacter;
class Skill;

// CSV 데이터 컬럼 인덱스 (타입 안전성과 가독성을 위해 enum class 사용)
enum class PlayerCsvColumn : int {
    NAME = 0,
    HEALTH,
    MANA,
    ATTACK,
    SPEED,
    // SKILL_START // 스킬 이름은 가변적이므로 고정 인덱스 정의 어려움
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
    BONUS_ATTACK, // BONUSATTACK에서 이름 변경
    CRITICAL_CHANCE // CRITICALATTACK에서 이름 변경
};


class DataTable {
public:
    // 싱글톤 인스턴스 접근 (Meyers' Singleton - 스레드 안전, 메모리 누수 방지)
    static DataTable& getInstance() {
        static DataTable instance; // C++11부터 스레드 안전하게 초기화됨
        // 최초 접근 시 여기서 데이터 로딩을 트리거할 수 있음
        // if (!instance.loaded) instance.LoadAllData();
        return instance;
    }

    // 복사 및 이동 생성자/대입 연산자 삭제 (싱글톤 패턴)
    DataTable(const DataTable&) = delete;
    DataTable& operator=(const DataTable&) = delete;
    DataTable(DataTable&&) = delete;
    DataTable& operator=(DataTable&&) = delete;

    // 데이터 로딩 함수
    void LoadPlayerData(const std::string& playerCharacterPath);
    void LoadMonsterData(const std::string& monsterCharacterPath);
    void LoadSkillData(const std::string& skillPath);
    // void LoadAllData(); // 모든 데이터를 한 번에 로드하는 선택적 함수

    // 데이터 테이블 Getter (불필요한 복사 방지 위해 const 참조 반환)
    const std::unordered_map<std::string, PlayerCharacter>& GetPlayerCharacterTable() const;
    const std::unordered_map<std::string, MonsterCharacter>& GetMonsterCharacterTable() const;
    const std::unordered_map<std::string, Skill>& GetSkillTable() const;

    // 특정 데이터 안전하게 찾기 (const 참조 반환, 없으면 예외 발생)
    const PlayerCharacter& FindPlayerCharacter(const std::string& name) const;
    const MonsterCharacter& FindMonsterCharacter(const std::string& name) const;
    const Skill& FindSkill(const std::string& name) const;

    // 특정 데이터 찾기 시도 (const 포인터 반환, 없으면 nullptr 반환)
    const PlayerCharacter* TryFindPlayerCharacter(const std::string& name) const;
    const MonsterCharacter* TryFindMonsterCharacter(const std::string& name) const;
    const Skill* TryFindSkill(const std::string& name) const;

private:
    // 생성자 및 소멸자는 private (싱글톤 패턴)
    DataTable() = default;
    ~DataTable() = default;

    // 데이터 저장 멤버
    std::unordered_map<std::string, PlayerCharacter> playerCharacterTable;
    std::unordered_map<std::string, MonsterCharacter> monsterCharacterTable;
    std::unordered_map<std::string, Skill> skillTable;
    // bool loaded = false; // 데이터 로딩 상태 플래그 (선택적)

    // CSV 파싱 헬퍼 함수 (내부 구현 상세)
    std::vector<std::string> parseCsvLine(const std::string& line);
};

// 전역 포인터 제거: extern DataTable* dataTable;
