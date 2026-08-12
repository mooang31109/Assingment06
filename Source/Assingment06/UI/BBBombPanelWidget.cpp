// BBBombPanelWidget.cpp


#include "BBBombPanelWidget.h"

#include "Assingment06/UI/BBChatInputWidget.h"
#include "Assingment06/Player/BBPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBBBombPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 숫자 키패드와 기능 버튼을 각각의 클릭 처리 함수에 연결합니다.
	if (IsValid(Button_1)) Button_1->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButton1Clicked);
	if (IsValid(Button_2)) Button_2->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButton2Clicked);
	if (IsValid(Button_3)) Button_3->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButton3Clicked);
	if (IsValid(Button_4)) Button_4->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButton4Clicked);
	if (IsValid(Button_5)) Button_5->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButton5Clicked);
	if (IsValid(Button_6)) Button_6->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButton6Clicked);
	if (IsValid(Button_7)) Button_7->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButton7Clicked);
	if (IsValid(Button_8)) Button_8->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButton8Clicked);
	if (IsValid(Button_9)) Button_9->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButton9Clicked);
	if (IsValid(Button_Clear)) Button_Clear->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClearButtonClicked);
	if (IsValid(Button_Submit)) Button_Submit->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSubmitButtonClicked);
	if (IsValid(Button_Backspace)) Button_Backspace->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackspaceButtonClicked);
}

void UBBBombPanelWidget::NativeDestruct()
{
	// 위젯이 제거된 뒤 버튼 Delegate가 남지 않도록 현재 위젯의 바인딩을 해제합니다.
	if (IsValid(Button_1)) Button_1->OnClicked.RemoveAll(this);
	if (IsValid(Button_2)) Button_2->OnClicked.RemoveAll(this);
	if (IsValid(Button_3)) Button_3->OnClicked.RemoveAll(this);
	if (IsValid(Button_4)) Button_4->OnClicked.RemoveAll(this);
	if (IsValid(Button_5)) Button_5->OnClicked.RemoveAll(this);
	if (IsValid(Button_6)) Button_6->OnClicked.RemoveAll(this);
	if (IsValid(Button_7)) Button_7->OnClicked.RemoveAll(this);
	if (IsValid(Button_8)) Button_8->OnClicked.RemoveAll(this);
	if (IsValid(Button_9)) Button_9->OnClicked.RemoveAll(this);
	if (IsValid(Button_Clear)) Button_Clear->OnClicked.RemoveAll(this);
	if (IsValid(Button_Submit)) Button_Submit->OnClicked.RemoveAll(this);
	if (IsValid(Button_Backspace)) Button_Backspace->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

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

void UBBBombPanelWidget::AppendKeypadDigit(const TCHAR Digit)
{
	// 숫자 야구 입력은 세 자리까지만 상단 화면에 입력할 수 있습니다.
	if (KeypadInputString.Len() < 3)
	{
		KeypadInputString.AppendChar(Digit);
		UpdateKeypadDisplay();
	}
}

void UBBBombPanelWidget::UpdateKeypadDisplay()
{
	if (IsValid(TextBlock_BombStatus))
	{
		const FString DisplayString = KeypadInputString.IsEmpty() ? TEXT("ENTER CODE") : KeypadInputString;
		TextBlock_BombStatus->SetText(FText::FromString(DisplayString));
	}
}

void UBBBombPanelWidget::OnButton1Clicked() { AppendKeypadDigit('1'); }
void UBBBombPanelWidget::OnButton2Clicked() { AppendKeypadDigit('2'); }
void UBBBombPanelWidget::OnButton3Clicked() { AppendKeypadDigit('3'); }
void UBBBombPanelWidget::OnButton4Clicked() { AppendKeypadDigit('4'); }
void UBBBombPanelWidget::OnButton5Clicked() { AppendKeypadDigit('5'); }
void UBBBombPanelWidget::OnButton6Clicked() { AppendKeypadDigit('6'); }
void UBBBombPanelWidget::OnButton7Clicked() { AppendKeypadDigit('7'); }
void UBBBombPanelWidget::OnButton8Clicked() { AppendKeypadDigit('8'); }
void UBBBombPanelWidget::OnButton9Clicked() { AppendKeypadDigit('9'); }

void UBBBombPanelWidget::OnClearButtonClicked()
{
	// C 버튼은 현재 입력한 숫자를 모두 지웁니다.
	KeypadInputString.Empty();
	UpdateKeypadDisplay();
}

void UBBBombPanelWidget::OnBackspaceButtonClicked()
{
	// ← 버튼은 마지막에 입력한 숫자 한 자리만 지웁니다.
	if (KeypadInputString.IsEmpty() == false)
	{
		KeypadInputString.LeftChopInline(1);
		UpdateKeypadDisplay();
	}
}

void UBBBombPanelWidget::OnSubmitButtonClicked()
{
	// 체크 버튼은 채팅 숫자 입력과 동일한 서버 제출 경로를 사용합니다.
	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(GetOwningPlayer());
	if (IsValid(BBPlayerController) && KeypadInputString.IsEmpty() == false)
	{
		BBPlayerController->SubmitChatMessage(KeypadInputString);
		KeypadInputString.Empty();
	}
}
