// BBBombPanelWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BBBombPanelWidget.generated.h"

class UBBChatInputWidget;
class UTextBlock;
class UButton;

/**
 * @brief 화면 전체를 담당하는 클래스
 * 
 * 숫자 키패드 입력
 * CLEAR, BACKSPACE, ENTER 처리
 * 현재 입력된 숫자 표시
 * 남은 시간 표시
 * 현재 턴 플레이어 표시
 * 남은 시도 횟수 표시
 * 결과 표시
 * 자기 턴이 아닐 때 키패드 비활성화
 */
UCLASS()
class ASSINGMENT06_API UBBBombPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** @brief 위젯 생성 후 키패드 버튼 이벤트를 등록합니다. */
	virtual void NativeConstruct() override;

	/** @brief 위젯 제거 전에 키패드 버튼 이벤트를 해제합니다. */
	virtual void NativeDestruct() override;

	/** @brief 폭탄 패널 내부에 배치된 채팅 위젯을 반환합니다. */
	UBBChatInputWidget* GetChatInputWidget() const;

	/** @brief 폭탄 상단 화면에 현재 해제 상태를 표시합니다. */
	void SetBombStatusText(const FString& StatusText);

	/** @brief 서버에서 받은 현재 담당자, 남은 시간, 개인 시도 횟수를 표시합니다. */
	void SetTurnInfo(const FString& PlayerName, int32 RemainingTime, const FString& GuessCountString);

protected:
	/** @brief 지정한 숫자를 현재 키패드 입력에 추가합니다. */
	void AppendKeypadDigit(const TCHAR Digit);

	/** @brief 현재 키패드 입력을 폭탄 상단 화면에 표시합니다. */
	void UpdateKeypadDisplay();

	UFUNCTION() void OnButton1Clicked();
	UFUNCTION() void OnButton2Clicked();
	UFUNCTION() void OnButton3Clicked();
	UFUNCTION() void OnButton4Clicked();
	UFUNCTION() void OnButton5Clicked();
	UFUNCTION() void OnButton6Clicked();
	UFUNCTION() void OnButton7Clicked();
	UFUNCTION() void OnButton8Clicked();
	UFUNCTION() void OnButton9Clicked();
	UFUNCTION() void OnClearButtonClicked();
	UFUNCTION() void OnSubmitButtonClicked();
	UFUNCTION() void OnBackspaceButtonClicked();

	/** @brief WBP_BBBombPanel 안에 자식으로 배치된 채팅 입력 위젯입니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBBChatInputWidget> WBP_BBChatInput;

	/** @brief 폭탄 해제 상태를 표시하는 상단 TextBlock입니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_BombStatus;

	/** @brief 현재 폭탄 해제 담당자를 표시합니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CurrentPlayer;

	/** @brief 서버에서 동기화된 남은 턴 시간을 표시합니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_RemainingTime;

	/** @brief 로컬 플레이어의 현재/최대 시도 횟수를 표시합니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_GuessCount;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_1;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_2;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_3;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_4;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_5;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_6;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_7;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_8;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_9;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_Clear;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_Submit;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_Backspace;

	/** @brief 로컬 키패드로 입력 중인 최대 세 자리 숫자입니다. */
	FString KeypadInputString;
};
