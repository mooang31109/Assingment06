// BBBombPanelWidget.cpp


#include "BBBombPanelWidget.h"

#include "Assingment06/UI/BBChatInputWidget.h"

UBBChatInputWidget* UBBBombPanelWidget::GetChatInputWidget() const
{
	// PlayerController가 수신 채팅을 전달할 수 있도록 자식 채팅 위젯을 반환합니다.
	return WBP_BBChatInput;
}
