// BBBombPanelWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BBBombPanelWidget.generated.h"

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
};
