// BBPlayerState.cpp


#include "BBPlayerState.h"

#include "Net/UnrealNetwork.h"

ABBPlayerState::ABBPlayerState()
	: CurrentGuessCount(0)
	, MaxGuessCount(3)
{
	// 서버가 관리하는 개인별 시도 횟수를 각 클라이언트에 복제합니다.
	bReplicates = true;
}

void ABBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 모든 클라이언트가 각 플레이어의 현재/최대 시도 횟수를 확인할 수 있게 합니다.
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

int32 ABBPlayerState::GetCurrentGuessCount() const
{
	return CurrentGuessCount;
}

int32 ABBPlayerState::GetMaxGuessCount() const
{
	return MaxGuessCount;
}

FString ABBPlayerState::GetGuessCountString() const
{
	return FString::Printf(TEXT("[%d / %d]"), CurrentGuessCount, MaxGuessCount);
}

bool ABBPlayerState::HasRemainingGuess() const
{
	return CurrentGuessCount < MaxGuessCount;
}

void ABBPlayerState::IncreaseGuessCount()
{
	// 최대 시도 횟수를 넘지 않도록 현재 횟수를 1 증가시킵니다.
	CurrentGuessCount = FMath::Min(CurrentGuessCount + 1, MaxGuessCount);
}

void ABBPlayerState::ResetGuessCount()
{
	// 새 게임을 시작할 수 있도록 사용한 시도 횟수를 초기화합니다.
	CurrentGuessCount = 0;
}
