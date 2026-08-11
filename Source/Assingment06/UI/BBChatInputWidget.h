// BBChatInputWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "BBChatInputWidget.generated.h"

/**
 * @brief 입력 위젯의 C++ 기반 클래스
 * 
 * EditableTextBox의 Enter 입력 감지
 * 입력 문자열을 BBPlayerController로 전달
 * 일반 채팅과 숫자 입력 모두 지원
 */

class UEditableTextBox;
class UButton;
class UScrollBox;

UCLASS()
class ASSINGMENT06_API UBBChatInputWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 위젯 생성 후 채팅 입력 이벤트 등록
	virtual void NativeConstruct() override;
	
	// 위젯 제거 전 채팅 입력 이벤트 등록 해제
	virtual void NativeDestruct() override;
protected:
	/**
	 * @brief 입력 완료 방식이 Enter일 때 채팅 메시지 전송
	 * @param Text 입력 위치에 작성된 텍스트
	 * @param CommitMethod 텍스트 입력을 완료한 방식
	 */
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	/** 
	 *  @brief SEND 버튼 클릭 시 채팅 메시지 전송
	 */
	UFUNCTION()
	void OnSendButtonClicked();
	
	// 위젯 블루프린트의 채팅 입력 상자
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
	
	// 채팅 입력 상자의 채팅 전송(SEND) 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Send;
	
	// 채팅 히스토리 스크롤 상자
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_ChatHistory;
	
public:
	void AddChatMessage(const FString& Message);
};
