#include "Framework.h" // 모든 헤더 포함
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <string>
#include <memory> // unique_ptr 사용 위해 추가

// 생성자: unique_ptr 벡터를 받아 내부 원시 포인터 벡터 초기화
Battle::Battle(const std::vector<std::unique_ptr<PlayerCharacter>>& playerParty,
               const std::vector<std::unique_ptr<MonsterCharacter>>& monsterParty) {
    // 플레이어 측 원시 포인터 추출 및 저장
    playerSide.reserve(playerParty.size());
    for (const auto& p : playerParty) {
        if (p) {
            // unique_ptr에서 원시 포인터를 얻어 Character*로 캐스팅 후 저장
            playerSide.push_back(static_cast<Character*>(p.get()));
        } else {
            // 이론적으로 unique_ptr 벡터에는 null이 들어가지 않지만, 방어적으로 처리
            std::cerr << "경고: Battle 생성자에 null PlayerCharacter unique_ptr이 감지되었습니다." << std::endl;
        }
    }

    // 몬스터 측 원시 포인터 추출 및 저장
    monsterSide.reserve(monsterParty.size());
    for (const auto& m : monsterParty) {
        if (m) {
            monsterSide.push_back(static_cast<Character*>(m.get()));
        } else {
            std::cerr << "경고: Battle 생성자에 null MonsterCharacter unique_ptr이 감지되었습니다." << std::endl;
        }
    }

    // 초기 턴 순서 설정
    BuildTurnTable();
    SortTurnTableBySpeed();
}

// BuildTurnTable, SortTurnTableBySpeed, PrintTurnTable, CheckBattleEndCondition,
// RemoveDeadCharactersFromTurnTable, ProcessTurn, StartFight 함수는 이전과 동일하게 유지
// ... (이전 Battle.cpp 코드 내용 붙여넣기) ...

// 현재 살아있는 캐릭터로부터 턴 테이블 생성
void Battle::BuildTurnTable() {
    turnTable.clear();
    // 예상 크기 예약
    turnTable.reserve(playerSide.size() + monsterSide.size());

    // 살아있는 플레이어 추가
    for (Character* p : playerSide) {
        if (p && p->IsAlive()) { // null 및 생존 상태 확인
            turnTable.push_back(p);
        }
    }
    // 살아있는 몬스터 추가
    for (Character* m : monsterSide) {
         if (m && m->IsAlive()) { // null 및 생존 상태 확인
            turnTable.push_back(m);
         }
    }
}

// 속도 기준 내림차순 정렬, 동점 처리 포함
void Battle::SortTurnTableBySpeed() {
    std::sort(turnTable.begin(), turnTable.end(), [](const Character* a, const Character* b) {
        // 잠재적 null 포인터 방어적 처리
        if (!a) return false; // null은 끝으로
        if (!b) return true;

        // 주 정렬: 속도 내림차순
        if (a->GetSpeedPoint() != b->GetSpeedPoint()) {
            return a->GetSpeedPoint() > b->GetSpeedPoint();
        }
        // 동점 처리: 플레이어가 몬스터보다 먼저 (낮은 enum 값 우선)
        return static_cast<int>(a->GetCharType()) < static_cast<int>(b->GetCharType());
        // 필요시 추가 동점 처리 기준 추가 가능 (예: 원래 위치)
    });
}

// 현재 턴 순서 출력 (디버깅용)
void Battle::PrintTurnTable() const {
    std::cout << "\n--- 턴 순서 ---" << std::endl;
    if (turnTable.empty()) {
        std::cout << "(턴 순서에 남은 캐릭터 없음)" << std::endl;
    } else {
        int turnNum = 1;
        for (const auto* character : turnTable) {
            if (character) { // null 포인터 확인
                 std::cout << turnNum++ << ". " << character->GetName()
                           << " (속도: " << character->GetSpeedPoint()
                           << ", HP: " << character->GetHealthPoint() << ")" << std::endl;
            } else {
                 std::cout << turnNum++ << ". (잘못된 캐릭터 포인터!)" << std::endl;
            }
        }
    }
    std::cout << "---------------" << std::endl;
}

// 전투 종료 조건 확인 (한 쪽 전멸)
bool Battle::CheckBattleEndCondition() const {
    // 모든 플레이어가 쓰러졌는지 확인 (null 포인터 처리)
    bool allPlayersDead = std::all_of(playerSide.begin(), playerSide.end(),
                                      [](const Character* p){ return !p || !p->IsAlive(); });

    // 모든 몬스터가 쓰러졌는지 확인 (null 포인터 처리)
    bool allMonstersDead = std::all_of(monsterSide.begin(), monsterSide.end(),
                                       [](const Character* m){ return !m || !m->IsAlive(); });

    // 어느 한 쪽이라도 전멸하면 전투 종료
    return allPlayersDead || allMonstersDead;
}

// 턴 테이블에서 쓰러진 캐릭터 포인터 제거
void Battle::RemoveDeadCharactersFromTurnTable() {
    // erase-remove 관용구 사용
    turnTable.erase(
        std::remove_if(turnTable.begin(), turnTable.end(),
                       [](const Character* c){ return !c || !c->IsAlive(); }), // 조건자: null이거나 살아있지 않으면 제거
        turnTable.end() // erase의 끝 반복자
    );
}

// 단일 캐릭터 턴 처리
const Skill* Battle::ProcessTurn(Character* currentCharacter) { // const Skill* 반환
     // 행동 전 캐릭터가 유효하고 살아있는지 확인
     if (!currentCharacter || !currentCharacter->IsAlive()) {
         return nullptr; // 캐릭터 행동 불가
     }

     // 행동 결정을 캐릭터 객체에 위임 (다형성)
     // DecideAction은 사용된 스킬을 반환, 행동/스킬 사용 안 했으면 nullptr 반환
     const Skill* skillUsed = currentCharacter->DecideAction(*this);

     return skillUsed; // 스킬 포인터 (또는 nullptr) 반환
}

// 주 전투 루프
void Battle::StartFight() {
    std::cout << "\n####################" << std::endl;
    std::cout << "#    전투 시작!    #" << std::endl;
    std::cout << "####################\n" << std::endl;
    PrintTurnTable(); // 초기 턴 순서 출력

    int turnCount = 0;
    const int MAX_TURNS = 100; // 무한 루프 방지

    // 전투 종료 조건 만족 또는 최대 턴 도달 시까지 반복
    while (!CheckBattleEndCondition() && turnCount < MAX_TURNS) {
        turnCount++;
        std::cout << "\n===== 턴 " << turnCount << " 시작 =====" << std::endl;

        // --- 현재 턴 테이블의 캐릭터들 턴 처리 ---
        // 추가 턴이 유효 순서를 변경할 수 있으므로 인덱스 기반 반복
        for (size_t i = 0; i < turnTable.size(); ++i) {
            Character* currentCharacter = turnTable[i];

            // 턴 처리 전 캐릭터가 여전히 유효하고 살아있는지 재확인
            // (같은 턴 내 이전 행동으로 쓰러졌을 수 있음)
            if (!currentCharacter || !currentCharacter->IsAlive()) {
                continue; // 쓰러졌거나 유효하지 않은 캐릭터 건너뛰기
            }

            // 캐릭터 턴 처리 및 사용된 스킬 가져오기 (있다면)
            const Skill* skillUsed = ProcessTurn(currentCharacter);

            // --- 각 행동 후 전투 종료 조건 확인 ---
            if (CheckBattleEndCondition()) {
                goto battle_end; // 중첩 루프 즉시 탈출 위해 goto 사용
            }

            // --- 추가 턴 로직 처리 ---
            if (skillUsed && skillUsed->IsBonusAttack()) { // 올바른 함수 이름 사용
                std::cout << "\n*** " << currentCharacter->GetName() << "이(가) 추가 턴을 얻었습니다! ***" << std::endl;

                // 추가 턴을 수행할 수 있도록 캐릭터가 여전히 살아있는지 확인
                if (currentCharacter->IsAlive()) {
                    // 루프 카운터 감소시켜 다음 반복에서 같은 캐릭터가 다시 행동하도록 함
                    i--;
                    std::cout << "(추가 턴 진행...)" << std::endl;
                    // continue; // 다음 루프 반복으로 건너뛰기 (이 캐릭터를 다시 처리)
                } else {
                    std::cout << "(...하지만 " << currentCharacter->GetName() << "은(는) 행동 전에 쓰러졌습니다.)" << std::endl;
                    // 캐릭터 사망, 추가 턴 상실. 루프는 정상 진행.
                }
            }
             // --- 추가 턴 로직 종료 ---

        } // --- 현재 턴 테이블 순회 종료 ---

        // --- 턴 종료 단계 ---
        std::cout << "\n===== 턴 " << turnCount << " 종료 =====" << std::endl;

        // 턴 동안 쓰러진 캐릭터들을 턴 테이블에서 제거
        RemoveDeadCharactersFromTurnTable();

        // 속도가 변경될 수 있다면(예: 버프/디버프) 턴 테이블 재정렬
        // 속도가 정적이라면 시작 시 한 번만 정렬해도 충분.
        // SortTurnTableBySpeed(); // 속도가 동적으로 변할 수 있다면 주석 해제

        PrintTurnTable(); // 다음 턴 순서 표시

        // 사용자 가독성 위해 일시 중지
        std::cout << "\n(다음 턴으로 진행하려면 Enter 키를 누르세요...)";
        // 잠재적 잔여 입력 비우고 Enter 대기
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 먼저 버퍼 비우기
        std::cin.get(); // Enter 키 입력 대기

    } // --- 주 전투 루프 종료 ---

    // 교착 상태 조건 확인
    if (turnCount >= MAX_TURNS && !CheckBattleEndCondition()) {
        std::cout << "\n####################" << std::endl;
        std::cout << "#   전투 교착 상태!  #" << std::endl;
        std::cout << "#  (최대 턴 도달)  #" << std::endl;
        std::cout << "####################\n" << std::endl;
        goto battle_summary; // 요약으로 건너뛰기
    }


battle_end: // 전투 종료 시 루프 탈출 지점
    std::cout << "\n####################" << std::endl;
    std::cout << "#    전투 종료!    #" << std::endl;
    std::cout << "####################\n" << std::endl;

battle_summary: // 결과 표시 지점
    // 승자 결정
    bool anyPlayerAlive = std::any_of(playerSide.begin(), playerSide.end(),
                                      [](const Character* p){ return p && p->IsAlive(); });
    bool anyMonsterAlive = std::any_of(monsterSide.begin(), monsterSide.end(),
                                       [](const Character* m){ return m && m->IsAlive(); });

    // 결과 발표
    if (anyPlayerAlive && !anyMonsterAlive) {
        std::cout << "★★★★★ 승리! 플레이어 승! ★★★★★" << std::endl;
    } else if (!anyPlayerAlive && anyMonsterAlive) {
        std::cout << "☆☆☆☆☆ 패배! 몬스터 승... ☆☆☆☆☆" << std::endl;
    } else if (!anyPlayerAlive && !anyMonsterAlive) {
        std::cout << "？？？？？ 상호 파괴! 양측 전멸... ？？？？？" << std::endl;
    } else { // 양측 생존자 있음 (교착 또는 무승부 조건)
        std::cout << "？？？？？ 무승부! 전투가 결론 없이 끝났습니다. ？？？？？" << std::endl;
    }

    // 모든 참가자 최종 상태 출력
    std::cout << "\n--- 최종 상태 ---" << std::endl;
     std::cout << "-- 플레이어 --" << std::endl;
     for (const auto* p : playerSide) { if(p) p->PrintStatus(); }
     std::cout << "-- 몬스터 --" << std::endl;
     for (const auto* m : monsterSide) { if(m) m->PrintStatus(); }
     std::cout << "-----------------" << std::endl;
}
