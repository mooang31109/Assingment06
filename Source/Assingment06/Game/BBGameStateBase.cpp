// BBGameStateBase.cpp


#include "BBGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Assingment06/Player/BBPlayerController.h"

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
