// BBPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBPlayerController.generated.h"

/**
 * @brief 플레이어의 입력을 서버로 전달하는 클래스
 * 
 * 로컬 UI 생성
 * 일반 채팅 전송
 * 채팅으로 입력한 숫자 제출
 * 키패드로 입력한 숫자 제출
 * 서버 RPC 호출
 * 자기 턴인지 확인하여 로컬 입력 상태 갱신
 * 서버가 전송한 채팅 메시지 수신
 */
UCLASS()
class ASSINGMENT06_API ABBPlayerController : public APlayerController
{
	GENERATED_BODY()
};
