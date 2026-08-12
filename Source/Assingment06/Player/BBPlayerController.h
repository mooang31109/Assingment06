// BBPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBPlayerController.generated.h"

class UBBBombPanelWidget;

/**
 * @brief 플레이어의 입력을 서버로 전달하는 클래스
 * 
 * 로컬 UI 생성
 * 일반 채팅 전송
 * 채팅으로 입력한 숫자 제출
 * 키패드로 입력한 숫자 제출
 * 서버 RPC 호출
 * 자기 턴인지 확인하여 로컬 입력 상태 갱신
 * 서버가 전송한 채팅 메시지 수신
 */
UCLASS()
class ASSINGMENT06_API ABBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	/** @brief PlayerController의 네트워크 사용을 초기화합니다. */
	ABBPlayerController();
	
	/** @brief PlayerController가 게임에 참가했을 때 기본 처리를 수행합니다. */
	virtual void BeginPlay() override;
	
	/**
	 * @brief 로컬 플레이어가 입력한 채팅 메시지를 검사한 뒤 서버로 전송합니다.
	 * @param Message 채팅 입력창에서 전달받은 원본 메시지입니다.
	 */
	void SubmitChatMessage(const FString& Message);
	
	/**
	 * @brief 서버가 이 PlayerController의 소유 클라이언트에 채팅 메시지를 전달합니다.
	 * @param Message 서버에서 검증을 마친 채팅 메시지입니다.
	 */
	UFUNCTION(Client, Reliable)
	void ClientRPCReceiveChatMessage(const FString& Message);

	/** @brief 서버가 소유 클라이언트의 폭탄 상태 화면을 갱신합니다. */
	UFUNCTION(Client, Reliable)
	void ClientRPCSetBombStatus(const FString& StatusText);
	
	/**
	 * @brief 소유 클라이언트가 서버에 채팅 메시지 방송을 요청합니다.
	 * @param Message 로컬에서 공백 제거와 길이 제한을 적용한 메시지입니다.
	 */
	UFUNCTION(Server, Reliable)
	void ServerRPCSubmitChatMessage(const FString& Message);

protected:
	/** @brief 에디터에서 지정하는 폭탄 패널 위젯 클래스입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBBBombPanelWidget> BombPanelWidgetClass;

	/** @brief 로컬 플레이어 화면에 생성된 폭탄 패널 위젯입니다. */
	UPROPERTY()
	TObjectPtr<UBBBombPanelWidget> BombPanelWidgetInstance;

private:
	/** @brief 한 번에 전송할 수 있는 채팅 메시지의 최대 문자 수입니다. */
	static constexpr int32 MaxChatMessageLength = 100;
};
