// BBGameStateBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BBGameStateBase.generated.h"

/**
 * @brief 모든 클라이언트가 알아야 하는 공용 상태를 담당하는 클래스
 */
UCLASS()
class ASSINGMENT06_API ABBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief 서버에서 새로 접속한 플레이어의 이름을 모든 클라이언트에 알립니다.
	 * @param PlayerName 접속 알림에 표시할 플레이어 이름입니다.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& PlayerName);
};
