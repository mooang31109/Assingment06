// BBBombPanelWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BBBombPanelWidget.generated.h"

class UBBChatInputWidget;
class UTextBlock;

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
	/** @brief 폭탄 패널 내부에 배치된 채팅 위젯을 반환합니다. */
	UBBChatInputWidget* GetChatInputWidget() const;

	/** @brief 폭탄 상단 화면에 현재 해제 상태를 표시합니다. */
	void SetBombStatusText(const FString& StatusText);

protected:
	/** @brief WBP_BBBombPanel 안에 자식으로 배치된 채팅 입력 위젯입니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBBChatInputWidget> WBP_BBChatInput;

	/** @brief 폭탄 해제 상태를 표시하는 상단 TextBlock입니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_BombStatus;
};
