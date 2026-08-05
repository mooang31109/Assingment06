// BBGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGameModeBase.generated.h"

/**
 * @brief 서버 전용 판정과 게임 진행을 담당하는 클래스
 * 
 * 정답 생성
 * 입력 유효성 검사
 * S/B/OUT 판정
 * 로그인 플레이어 등록
 * 현재 플레이어 입력 권한 검증
 * 시도 횟수 차감
 * 승리 및 무승부 판정
 * 턴 변경
 * 시간 초과 처리
 * 게임 리셋
 */
UCLASS()
class ASSINGMENT06_API ABBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
};
