// BBChatInputWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BBChatInputWidget.generated.h"

/**
 * @brief 입력 위젯의 C++ 기반 클래스
 * 
 * EditableTextBox의 Enter 입력 감지
 * 입력 문자열을 BBPlayerController로 전달
 * 일반 채팅과 숫자 입력 모두 지원
 */
UCLASS()
class ASSINGMENT06_API UBBChatInputWidget : public UUserWidget
{
	GENERATED_BODY()
};
