// BBChatInputWidget.cpp


#include "BBChatInputWidget.h"

#include "Assingment06/Player/BBPlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"


void UBBChatInputWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	/** 
	 * 위젯이 정상적으로 존재하고,
	 * 아직 이 함수가 델리게이트에 등록되지 않았을 때만 AddDynamic으로 등록하라.
	 */
	// IsAlreadyBound : 특정 함수가 이미 바인딩되어 있는지 확인만 하는 함수. bool값을 반환한다.
	if (IsValid(EditableTextBox_ChatInput)
		&& EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		// AddDynamic : 델리게이트에 함수를 바인딩 하는 매크로
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
	
	if (IsValid(Button_Send)
		&& Button_Send->OnClicked.IsAlreadyBound(this, &ThisClass::OnSendButtonClicked) == false)
	{
		Button_Send->OnClicked.AddDynamic(this, &ThisClass::OnSendButtonClicked);
	}
}

void UBBChatInputWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	/** 
	 * 위젯이 정상적으로 존재하고,
	 * 아직 이 함수가 델리게이트에 등록되어 있을 때만 RemoveDynamic으로 제거하라.
	 */
	if (IsValid(EditableTextBox_ChatInput)
		&& EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		// RemoveDynamic : 델리게이트에 바인딩된 함수를 제거하는 매크로
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
	
	if (IsValid(Button_Send)
		&& Button_Send->OnClicked.IsAlreadyBound(this, &ThisClass::OnSendButtonClicked) == true)
	{
		Button_Send->OnClicked.RemoveDynamic(this, &ThisClass::OnSendButtonClicked);
	}
}

void UBBChatInputWidget::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 텍스트 입력이 확정된 이유가 Enter 키를 눌러서인 경우에만 이 안의 로직을 실행해라.
	if (CommitMethod == ETextCommit::OnEnter)
	{
		// 이 위젯을 생성한 로컬 PlayerController를 가져온다.
		APlayerController* OwningPlayerController = GetOwningPlayer();
		
		// 이 위젯의 소유자 컨트롤러를 안전하게 가져왔는가.
		if (IsValid(OwningPlayerController) == true)
		{
			ABBPlayerController* OwningBBPlayerController = Cast<ABBPlayerController>(OwningPlayerController);
			
			if (IsValid(OwningBBPlayerController) == true)
			{
				// 입력 문자열을 PlayerController에 전달하여 Client → Server 채팅 전송을 시작합니다.
				OwningBBPlayerController->SubmitChatMessage(Text.ToString());
				
				if (IsValid(EditableTextBox_ChatInput) == true)
				{
					EditableTextBox_ChatInput->SetText(FText::GetEmpty());
				}
			}
		}
	}
}

void UBBChatInputWidget::OnSendButtonClicked()
{
	if (IsValid(EditableTextBox_ChatInput))
	{
		OnChatInputTextCommitted(EditableTextBox_ChatInput->GetText(),ETextCommit::OnEnter);
	}
}

void UBBChatInputWidget::AddChatMessage(const FString& Message)
{
	// 채팅 기록을 표시할 ScrollBox와 위젯 트리가 유효한지 확인합니다.
	if (IsValid(ScrollBox_ChatHistory) == false || IsValid(WidgetTree) == false)
	{
		return;
	}

	// 수신한 메시지 하나를 표시할 TextBlock을 현재 채팅 위젯에 생성합니다.
	UTextBlock* ChatMessageTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	if (IsValid(ChatMessageTextBlock) == false)
	{
		return;
	}

	ChatMessageTextBlock->SetText(FText::FromString(Message));
	// 채팅창 너비를 넘는 긴 메시지는 문자 단위로 자동 줄바꿈합니다.
	ChatMessageTextBlock->SetAutoWrapText(true);
	ChatMessageTextBlock->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);
	ScrollBox_ChatHistory->AddChild(ChatMessageTextBlock);

	// 새 메시지가 추가될 때 가장 최근 채팅이 보이도록 아래로 이동합니다.
	ScrollBox_ChatHistory->ScrollToEnd();
}
