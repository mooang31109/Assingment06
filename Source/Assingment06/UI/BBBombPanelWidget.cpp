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
