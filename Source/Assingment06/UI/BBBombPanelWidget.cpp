// BBBombPanelWidget.cpp


#include "BBBombPanelWidget.h"

#include "Assingment06/UI/BBChatInputWidget.h"
#include "Components/TextBlock.h"

UBBChatInputWidget* UBBBombPanelWidget::GetChatInputWidget() const
{
	// PlayerController가 수신 채팅을 전달할 수 있도록 자식 채팅 위젯을 반환합니다.
	return WBP_BBChatInput;
}

void UBBBombPanelWidget::SetBombStatusText(const FString& StatusText)
{
	// 서버에서 전달받은 폭탄 상태를 상단 화면에 표시합니다.
	if (IsValid(TextBlock_BombStatus))
	{
		TextBlock_BombStatus->SetText(FText::FromString(StatusText));
	}
}

void UBBBombPanelWidget::SetTurnInfo(const FString& PlayerName, int32 RemainingTime, const FString& GuessCountString)
{
	// 서버가 지정한 현재 폭탄 해제 담당자를 표시합니다.
	if (IsValid(TextBlock_CurrentPlayer))
	{
		TextBlock_CurrentPlayer->SetText(FText::FromString(TEXT("현재 해제 담당자: ") + PlayerName));
	}

	// 모든 클라이언트가 동일한 서버 시간을 볼 수 있도록 정수 형태로 표시합니다.
	if (IsValid(TextBlock_RemainingTime))
	{
		TextBlock_RemainingTime->SetText(FText::FromString(FString::Printf(TEXT("남은 시간: %02d"), RemainingTime)));
	}

	// 로컬 플레이어의 현재 시도 횟수와 최대 시도 횟수를 표시합니다.
	if (IsValid(TextBlock_GuessCount))
	{
		TextBlock_GuessCount->SetText(FText::FromString(TEXT("시도 횟수: ") + GuessCountString));
	}
}
