// BBGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGameModeBase.generated.h"

class ABBPlayerController;

/**
 * @brief 서버 전용 판정과 게임 진행을 담당하는 클래스
 * 
 * 정답 생성
 * 입력 유효성 검사
 * S/B/OUT 판정
 * 로그인 플레이어 등록
 * 현재 플레이어 입력 권한 검증
 * 시도 횟수 차감
 * 승리 및 무승부 판정
 * 턴 변경
 * 시간 초과 처리
 * 게임 리셋
 */
UCLASS()
class ASSINGMENT06_API ABBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	/** @brief 게임 시작 시 서버에서 첫 번째 정답 숫자를 생성합니다. */
	virtual void BeginPlay() override;

	/**
	 * @brief 새 플레이어의 로그인이 완료되면 표시 이름을 지정하고 접속 사실을 알립니다.
	 * @param NewPlayer 로그인을 완료한 플레이어의 Controller입니다.
	 */
	virtual void OnPostLogin(AController* NewPlayer) override;

	/**
	 * @brief 서버에서 발신자 정보를 채팅 내용에 붙여 모든 플레이어에게 전송합니다.
	 * @param Sender 채팅 메시지를 보낸 PlayerController입니다.
	 * @param Message 클라이언트가 전송한 채팅 본문입니다.
	 */
	void BroadcastChatMessage(ABBPlayerController* Sender, const FString& Message);

	/** @brief 1~9 사이에서 중복되지 않는 세 자리 정답 숫자를 생성합니다. */
	FString GenerateSecretNumber();

	/** @brief 입력 문자열이 1~9의 서로 다른 숫자 세 개인지 확인합니다. */
	bool IsGuessNumberString(const FString& NumberString) const;

	/** @brief 정답과 입력을 비교하여 OUT 또는 nSnB 형식의 문자열을 반환합니다. */
	FString JudgeResult(const FString& SecretNumber, const FString& GuessNumber) const;

protected:
	/** @brief 현재 게임에서 사용하는 서버 전용 정답 숫자입니다. */
	FString SecretNumberString;
};
