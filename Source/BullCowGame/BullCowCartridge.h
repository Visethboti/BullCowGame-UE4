// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& PlayerInput) override;
	void GameSetup();
	void PrintIntro();
	void EndGame();
	void EndTurn();
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& Word) const;
	void CalcBullCow(const FString& Guess, int32& CalcBull, int32& CalcCow) const;
	TArray<FString> GetValidWords(const TArray<FString>& WordList) const;

	// Your declarations go below!
	private:
	FString HiddenWord;
	int32 NumLives;
	bool bTurnOver;
	bool bGameOver;

	int32 NumBull;
	int32 NumCow;

	TArray<FString> Words;
};
