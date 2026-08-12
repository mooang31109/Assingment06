// BBGameStateBase.cpp


#include "BBGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Assingment06/Player/BBPlayerController.h"
#include "Assingment06/Player/BBPlayerState.h"
#include "Net/UnrealNetwork.h"

void ABBGameStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 현재 턴 플레이어와 서버의 남은 시간을 모든 클라이언트에 동기화합니다.
	DOREPLIFETIME(ThisClass, CurrentTurnPlayerState);
	DOREPLIFETIME(ThisClass, RemainingTurnTime);
}

void ABBGameStateBase::SetCurrentTurnPlayerState(ABBPlayerState* PlayerState)
{
	// 공용 턴 상태는 서버에서만 변경합니다.
	if (HasAuthority())
	{
		CurrentTurnPlayerState = PlayerState;
	}
}

ABBPlayerState* ABBGameStateBase::GetCurrentTurnPlayerState() const
{
	return CurrentTurnPlayerState;
}

void ABBGameStateBase::SetRemainingTurnTime(int32 RemainingTime)
{
	// 시간이 음수가 되지 않도록 보정한 뒤 서버 상태에 저장합니다.
	if (HasAuthority())
	{
		RemainingTurnTime = FMath::Max(RemainingTime, 0);
	}
}

int32 ABBGameStateBase::GetRemainingTurnTime() const
{
	return RemainingTurnTime;
}

void ABBGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& PlayerName)
{
	// Multicast RPC는 서버와 모든 클라이언트에서 실행됩니다.
	// 서버 측 중복 출력을 제외하고 각 클라이언트의 로컬 화면에만 알림을 전달합니다.
	if (HasAuthority())
	{
		return;
	}

	// 현재 클라이언트 월드의 첫 번째 로컬 PlayerController를 가져옵니다.
	APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(LocalPlayerController);

	if (IsValid(BBPlayerController))
	{
		// 현재는 PlayerController의 수신 함수를 통해 화면 메시지로 출력합니다.
		// 채팅 UI가 연결되면 같은 경로로 ScrollBox에 표시됩니다.
		const FString LoginMessage = PlayerName + TEXT(" has joined the game.");
		BBPlayerController->ClientRPCReceiveChatMessage(LoginMessage);
	}
}
