// BBGameStateBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BBGameStateBase.generated.h"

class ABBPlayerState;

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

	/** @brief 서버에서 현재 턴 플레이어를 설정합니다. */
	void SetCurrentTurnPlayerState(ABBPlayerState* PlayerState);

	/** @brief 현재 턴을 가진 플레이어의 PlayerState를 반환합니다. */
	ABBPlayerState* GetCurrentTurnPlayerState() const;

	/** @brief 서버에서 현재 턴의 남은 시간을 설정합니다. */
	void SetRemainingTurnTime(int32 RemainingTime);

	/** @brief 현재 턴의 남은 시간을 반환합니다. */
	int32 GetRemainingTurnTime() const;

protected:
	/** @brief 모든 클라이언트에 복제되는 현재 턴 플레이어입니다. */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
	TObjectPtr<ABBPlayerState> CurrentTurnPlayerState;

	/** @brief 서버가 관리하고 모든 클라이언트에 복제하는 남은 턴 시간입니다. */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
	int32 RemainingTurnTime = 0;

public:
	/** @brief 네트워크로 복제할 턴 상태 프로퍼티를 등록합니다. */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
