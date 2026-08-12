// BBGameModeBase.cpp


#include "BBGameModeBase.h"

#include "Assingment06/Game/BBGameStateBase.h"
#include "Assingment06/Player/BBPlayerController.h"
#include "Assingment06/Player/BBPlayerState.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerState.h"

void ABBGameModeBase::BeginPlay()
{
	// GameMode의 기본 게임 시작 처리를 보존합니다.
	Super::BeginPlay();

	// 정답은 클라이언트에 노출되지 않도록 서버 전용 GameMode에서 생성합니다.
	SecretNumberString = GenerateSecretNumber();

	// 개발 중 판정 결과를 확인할 수 있도록 서버 로그에 정답을 기록합니다.
	UE_LOG(LogTemp, Log, TEXT("Secret Number: %s"), *SecretNumberString);
}

void ABBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	// 엔진의 기본 로그인 완료 처리를 먼저 실행하여 PlayerState 등을 준비합니다.
	Super::OnPostLogin(NewPlayer);

	ABBPlayerController* BBPlayerController = Cast<ABBPlayerController>(NewPlayer);
	if (IsValid(BBPlayerController) == false)
	{
		return;
	}

	APlayerState* JoinedPlayerState = BBPlayerController->PlayerState;
	if (IsValid(JoinedPlayerState) == false)
	{
		return;
	}

	// 현재 접속 인원을 이용해 서버 권한으로 간단한 표시 이름을 부여합니다.
	const int32 PlayerNumber = GameState != nullptr ? GameState->PlayerArray.Num() : 1;
	const FString PlayerName = FString::Printf(TEXT("Player%d"), PlayerNumber);
	JoinedPlayerState->SetPlayerName(PlayerName);

	// GameState의 Multicast RPC를 통해 모든 클라이언트에 접속 알림을 전달합니다.
	ABBGameStateBase* BBGameState = GetGameState<ABBGameStateBase>();
	if (IsValid(BBGameState))
	{
		BBGameState->MulticastRPCBroadcastLoginMessage(PlayerName);
	}
}

void ABBGameModeBase::BroadcastChatMessage(ABBPlayerController* Sender, const FString& Message)
{
	// 이 함수는 서버 전용 GameMode에서만 실행되며 유효한 발신자만 처리합니다.
	if (IsValid(Sender) == false || HasAuthority() == false)
	{
		return;
	}

	// 발신자의 서버 권한 PlayerState에서 표시 이름을 가져옵니다.
	const APlayerState* SenderPlayerState = Sender->PlayerState;
	const FString PlayerName = IsValid(SenderPlayerState)
		? SenderPlayerState->GetPlayerName()
		: TEXT("Player");
	const FString FormattedMessage = PlayerName + TEXT(": ") + Message;

	// 숫자가 포함되었거나 길이가 세 자리이면 숫자 입력 시도로 처리합니다.
	bool bIsGuessAttempt = Message.Len() == 3;
	for (const TCHAR Character : Message)
	{
		if (FChar::IsDigit(Character))
		{
			bIsGuessAttempt = true;
			break;
		}
	}

	if (bIsGuessAttempt)
	{
		// 승리 또는 무승부 결과를 표시하는 동안에는 새로운 숫자를 판정하지 않습니다.
		if (bGameEnded)
		{
			Sender->ClientRPCReceiveChatMessage(TEXT("새 게임을 준비 중입니다"));
			return;
		}

		// 규칙에 맞지 않는 입력은 발신자에게만 안내하고 판정하지 않습니다.
		if (IsGuessNumberString(Message) == false)
		{
			Sender->ClientRPCReceiveChatMessage(TEXT("다시 입력하세요"));
			return;
		}

		// 유효한 숫자는 서버의 정답과 비교한 결과를 붙여 방송합니다.
		// 서버 권한의 PlayerState에서 발신자의 개인별 시도 횟수를 관리합니다.
		ABBPlayerState* BBPlayerState = Sender->GetPlayerState<ABBPlayerState>();
		if (IsValid(BBPlayerState) == false)
		{
			return;
		}

		// 최대 시도 횟수를 모두 사용한 플레이어의 추가 숫자 입력은 판정하지 않습니다.
		if (BBPlayerState->HasRemainingGuess() == false)
		{
			Sender->ClientRPCReceiveChatMessage(TEXT("남은 기회가 없습니다"));
			return;
		}

		// 유효한 숫자 입력만 시도 횟수를 1 증가시킵니다.
		BBPlayerState->IncreaseGuessCount();

		const FString JudgeResultString = JudgeResult(SecretNumberString, Message);
		const FString GuessResultMessage = FormattedMessage
			+ TEXT(" -> ") + JudgeResultString
			+ TEXT(" ") + BBPlayerState->GetGuessCountString();

		for (TActorIterator<ABBPlayerController> It(GetWorld()); It; ++It)
		{
			ABBPlayerController* TargetController = *It;
			if (IsValid(TargetController))
			{
				TargetController->ClientRPCReceiveChatMessage(GuessResultMessage);
				// 폭탄 상단 화면에는 플레이어 정보 없이 판정 상태만 표시합니다.
				TargetController->ClientRPCSetBombStatus(JudgeResultString);
			}
		}

		// 결과 방송 후 승리 또는 무승부 여부를 한 번만 판정합니다.
		const int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgeGame(Sender, StrikeCount);

		return;
	}

	// 서버 월드의 모든 PlayerController를 순회하여 각 소유 클라이언트에 전달합니다.
	for (TActorIterator<ABBPlayerController> It(GetWorld()); It; ++It)
	{
		ABBPlayerController* TargetController = *It;
		if (IsValid(TargetController))
		{
			TargetController->ClientRPCReceiveChatMessage(FormattedMessage);
		}
	}
}

FString ABBGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;

	// 0을 제외한 1~9 숫자를 정답 후보에 추가합니다.
	for (int32 Number = 1; Number <= 9; ++Number)
	{
		Numbers.Add(Number);
	}

	FString Result;

	// 뽑은 숫자를 후보에서 제거하며 중복되지 않는 세 자리 숫자를 만듭니다.
	for (int32 Index = 0; Index < 3; ++Index)
	{
		const int32 RandomIndex = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[RandomIndex]));
		Numbers.RemoveAt(RandomIndex);
	}

	return Result;
}

bool ABBGameModeBase::IsGuessNumberString(const FString& NumberString) const
{
	// 정확히 세 자리인지 먼저 확인합니다.
	if (NumberString.Len() != 3)
	{
		return false;
	}

	TSet<TCHAR> UniqueDigits;

	for (const TCHAR Character : NumberString)
	{
		// 문자가 포함되거나 0이 사용되면 유효하지 않은 입력입니다.
		if (FChar::IsDigit(Character) == false || Character == '0')
		{
			return false;
		}

		UniqueDigits.Add(Character);
	}

	// 집합 크기가 3이어야 모든 숫자가 서로 다릅니다.
	return UniqueDigits.Num() == 3;
}

FString ABBGameModeBase::JudgeResult(const FString& SecretNumber, const FString& GuessNumber) const
{
	int32 StrikeCount = 0;
	int32 BallCount = 0;

	for (int32 Index = 0; Index < 3; ++Index)
	{
		if (SecretNumber[Index] == GuessNumber[Index])
		{
			++StrikeCount;
		}
		else
		{
			const FString GuessCharacter = FString::Printf(TEXT("%c"), GuessNumber[Index]);
			if (SecretNumber.Contains(GuessCharacter))
			{
				++BallCount;
			}
		}
	}

	// 일치하는 숫자가 하나도 없으면 OUT을 반환합니다.
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ABBGameModeBase::JudgeGame(ABBPlayerController* Sender, int32 StrikeCount)
{
	if (bGameEnded || IsValid(Sender) == false)
	{
		return;
	}

	FString ResultMessage;
	FString BombStatus;

	// 세 자리 숫자를 모두 맞힌 플레이어를 즉시 승자로 판정합니다.
	if (StrikeCount == 3)
	{
		bGameEnded = true;

		const APlayerState* WinnerPlayerState = Sender->PlayerState;
		const FString WinnerName = IsValid(WinnerPlayerState)
			? WinnerPlayerState->GetPlayerName()
			: TEXT("Player");

		ResultMessage = TEXT("SYSTEM: ") + WinnerName + TEXT("이 승리했습니다.");
		BombStatus = TEXT("DISARMED");
	}
	else
	{
		// 한 명이라도 기회가 남아 있다면 현재 게임을 계속합니다.
		bool bIsDraw = true;
		for (const APlayerState* PlayerState : GameState->PlayerArray)
		{
			const ABBPlayerState* BBPlayerState = Cast<ABBPlayerState>(PlayerState);
			if (IsValid(BBPlayerState) && BBPlayerState->HasRemainingGuess())
			{
				bIsDraw = false;
				break;
			}
		}

		if (bIsDraw == false)
		{
			return;
		}

		bGameEnded = true;
		ResultMessage = TEXT("SYSTEM: 모든 플레이어가 기회를 소진하여 무승부입니다.");
		BombStatus = TEXT("DETONATED");
	}

	// 상세 결과는 채팅창에, 폭탄 상태는 상단 화면에 각각 표시합니다.
	for (TActorIterator<ABBPlayerController> It(GetWorld()); It; ++It)
	{
		ABBPlayerController* TargetController = *It;
		if (IsValid(TargetController))
		{
			TargetController->ClientRPCReceiveChatMessage(ResultMessage);
			TargetController->ClientRPCSetBombStatus(BombStatus);
		}
	}

	// 결과를 확인할 시간을 준 뒤 서버에서 다음 게임을 시작합니다.
	GetWorldTimerManager().SetTimer(ResetGameTimerHandle, this, &ThisClass::ResetGame, 3.f, false);
}

void ABBGameModeBase::ResetGame()
{
	// 다음 게임에 사용할 새로운 정답을 서버에서 생성합니다.
	SecretNumberString = GenerateSecretNumber();

	// 모든 플레이어의 개인별 시도 횟수를 0으로 초기화합니다.
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		ABBPlayerState* BBPlayerState = Cast<ABBPlayerState>(PlayerState);
		if (IsValid(BBPlayerState))
		{
			BBPlayerState->ResetGuessCount();
		}
	}

	bGameEnded = false;

	// 새 게임 시작 사실과 초기 폭탄 상태를 모든 클라이언트에 알립니다.
	for (TActorIterator<ABBPlayerController> It(GetWorld()); It; ++It)
	{
		ABBPlayerController* TargetController = *It;
		if (IsValid(TargetController))
		{
			TargetController->ClientRPCReceiveChatMessage(TEXT("SYSTEM: 새로운 게임을 시작합니다."));
			TargetController->ClientRPCSetBombStatus(TEXT("ENTER CODE"));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Secret Number: %s"), *SecretNumberString);
}
