// BBPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BBPlayerState.generated.h"

/**
 * @brief 플레이어 개인별 상태를 관리하는 클래스
 */
UCLASS()
class ASSINGMENT06_API ABBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/** @brief 플레이어의 기본 시도 횟수와 최대 시도 횟수를 초기화합니다. */
	ABBPlayerState();

	/** @brief 네트워크로 복제할 시도 횟수 프로퍼티를 등록합니다. */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/** @brief 현재까지 사용한 시도 횟수를 반환합니다. */
	int32 GetCurrentGuessCount() const;

	/** @brief 플레이어에게 허용된 최대 시도 횟수를 반환합니다. */
	int32 GetMaxGuessCount() const;

	/** @brief "[현재 시도 횟수 / 최대 시도 횟수]" 형식의 문자열을 반환합니다. */
	FString GetGuessCountString() const;

	/** @brief 플레이어에게 숫자를 추측할 기회가 남아 있는지 반환합니다. */
	bool HasRemainingGuess() const;

	/** @brief 서버에서 유효한 숫자 입력이 처리될 때 현재 시도 횟수를 1 증가시킵니다. */
	void IncreaseGuessCount();

protected:
	/** @brief 현재 게임에서 플레이어가 사용한 시도 횟수입니다. */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Number Baseball")
	int32 CurrentGuessCount;

	/** @brief 현재 게임에서 플레이어에게 허용된 최대 시도 횟수입니다. */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Number Baseball")
	int32 MaxGuessCount;
};
