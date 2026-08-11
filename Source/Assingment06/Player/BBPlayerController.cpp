// BBPlayerController.cpp


#include "BBPlayerController.h"

#include "Assingment06/Game/BBGameModeBase.h"
#include "Assingment06/UI/BBBombPanelWidget.h"
#include "Assingment06/UI/BBChatInputWidget.h"
#include "Kismet/GameplayStatics.h"

ABBPlayerController::ABBPlayerController()
{
	// 서버와 소유 클라이언트 사이에서 RPC를 사용할 수 있도록 복제를 활성화합니다.
	bReplicates = true;
}

void ABBPlayerController::BeginPlay()
{
	// APlayerController의 기본 시작 처리를 보존합니다.
	Super::BeginPlay();

	// 서버의 원격 PlayerController에는 UI가 필요하지 않으므로 로컬 Controller만 처리합니다.
	if (IsLocalController() == false)
	{
		return;
	}

	// 채팅창과 키패드를 조작할 수 있도록 입력을 UI에 전달합니다.
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	bShowMouseCursor = true;

	// 에디터에서 지정한 폭탄 패널 위젯을 로컬 플레이어 화면에 생성합니다.
	if (IsValid(BombPanelWidgetClass))
	{
		BombPanelWidgetInstance = CreateWidget<UBBBombPanelWidget>(this, BombPanelWidgetClass);
		if (IsValid(BombPanelWidgetInstance))
		{
			BombPanelWidgetInstance->AddToViewport();
		}
	}
}

void ABBPlayerController::SubmitChatMessage(const FString& Message)
{
	// 채팅 UI를 소유한 로컬 PlayerController만 서버 전송을 시작합니다.
	if (IsLocalController() == false)
	{
		return;
	}

	// 앞뒤 공백을 제거하여 공백만 입력된 메시지가 전송되지 않게 합니다.
	FString SanitizedMessage = Message;
	SanitizedMessage.TrimStartAndEndInline();

	if (SanitizedMessage.IsEmpty())
	{
		return;
	}

	// 지나치게 긴 RPC 문자열을 막기 위해 허용 길이까지만 서버에 전송합니다.
	SanitizedMessage.LeftInline(MaxChatMessageLength);
	ServerRPCSubmitChatMessage(SanitizedMessage);
}

void ABBPlayerController::ServerRPCSubmitChatMessage_Implementation(const FString& Message)
{
	// 클라이언트 검사를 신뢰하지 않고 서버에서도 공백과 길이를 다시 검증합니다.
	FString SanitizedMessage = Message;
	SanitizedMessage.TrimStartAndEndInline();
	SanitizedMessage.LeftInline(MaxChatMessageLength);

	if (SanitizedMessage.IsEmpty())
	{
		return;
	}

	// 서버 전용 GameMode에 발신자 표시와 전체 채팅 방송을 위임합니다.
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	ABBGameModeBase* BBGameMode = Cast<ABBGameModeBase>(GameMode);
	if (IsValid(BBGameMode))
	{
		BBGameMode->BroadcastChatMessage(this, SanitizedMessage);
	}
}

void ABBPlayerController::ClientRPCReceiveChatMessage_Implementation(const FString& Message)
{
	// 현재 단계에서는 네트워크 수신 여부를 확인할 수 있도록 화면에 출력합니다.
	// 채팅 위젯 연결 후 이 부분을 BBChatInputWidget::AddChatMessage 호출로 교체합니다.
	// 폭탄 패널 내부의 채팅 위젯에 서버가 전송한 메시지를 표시합니다.
	if (IsValid(BombPanelWidgetInstance))
	{
		UBBChatInputWidget* ChatInputWidget = BombPanelWidgetInstance->GetChatInputWidget();
		if (IsValid(ChatInputWidget))
		{
			ChatInputWidget->AddChatMessage(Message);
		}
	}
}
