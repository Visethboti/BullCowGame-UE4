// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

// For importing the words list from HiddenWordList.txt
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

// For using RandRange
#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    // Get all words from HiddenWordList.txt into our array of FString (Words)
    // FPaths::ProjectContentDir()  is the path of this project
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt"); // This will find the txt file in content folder
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    //PrintLine(FString::Printf(TEXT("!!!!Word List is: %s"), *Words[1])); // this is for testing the hiddenword list import correctly

    // Filter the word list to get only isogram word
    Words = GetValidWords(Words);

    GameSetup(); // Setting up game
    PrintIntro();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bTurnOver) {
        ClearScreen();
        PrintIntro();
        bTurnOver = false;
        return;
    }

    if (bGameOver) {
        ClearScreen();
        PrintLine(TEXT("Do you wish to play again? Y/N"));
        if (PlayerInput == "Y" || PlayerInput == "y") {
            ClearScreen();
            GameSetup();
            PrintIntro();
        }
        return;
    }
    else {   
        PrintLine(TEXT("You entered " + PlayerInput));

        ProcessGuess(*PlayerInput);
    }
}

void UBullCowCartridge::GameSetup() {
    HiddenWord = *Words[FMath::RandRange(0, Words.Num() - 1)];
    PrintLine(FString::Printf(TEXT("!!!! hiddenword is: %s"), *HiddenWord)); // Debug Line
    NumLives = 5;
    bGameOver = false;
    bTurnOver = false;

    int32 NumBull = 0;
    int32 NumCow = 0;
}

void UBullCowCartridge::PrintIntro() {
    // Print Welcoming the player


    PrintLine(TEXT("Welcome to Bull Cow!"));
    PrintLine(FString::Printf(TEXT("Guess the %i letter long word"), HiddenWord.Len()));
    PrintLine(TEXT("Type in your guess"));
}

void UBullCowCartridge::EndGame() {
    PrintLine(TEXT("Press enter to continue..."));
    bGameOver = true;
}

void UBullCowCartridge::EndTurn() {
    PrintLine(TEXT("Press enter to continue..."));
    bTurnOver = true;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    if (Guess == HiddenWord) {
        PrintLine("It is correct! You Win");
        EndGame();
    }
    else {
        if (Guess.Len() != HiddenWord.Len()) {
            PrintLine(FString::Printf(TEXT("Word input is not %i characters long"), HiddenWord.Len()));
            EndTurn();
        }
        else if (!IsIsogram(*Guess)){
            PrintLine(FString::Printf(TEXT("Word input is not isogram"), HiddenWord.Len()));
            EndTurn();
        }
        else {
            --NumLives; //remove number of lives

            //check number lives
            if (NumLives < 1) {
                PrintLine(TEXT("Game Over! You ran out of lives"));
                EndGame();
            }
            else {
                // Calculate bull and cow
                CalcBullCow(*Guess);

                // Print bull and cow
                PrintLine(TEXT("It is Incorrect!"));
                PrintLine(FString::Printf(TEXT("There is %i bull and %i cow."), NumBull, NumCow));
                PrintLine(FString::Printf(TEXT("You have %i live left"), NumLives));
                EndTurn();      
            }
        }
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const {
    for (int32 i = 0; i < Word.Len(); i++) {
        for (int32 j = i+1; j < Word.Len(); j++) {
            if (Word[i] == Word[j]) {
                return false;
            }
        }
    }

    return true;
}

void UBullCowCartridge::CalcBullCow(const FString& Guess, int32& CalcBull, int32& CalcCow) const {
    CalcBull = 0;
    CalcCow = 0;
    
    for (int32 i = 0; i < Guess.Len(); i++) {
        for (int32 j = 0; j < Guess.Len(); j++) {
            if (Guess[i] == HiddenWord[j]) {
                if (i == j) {
                    ++CalcBull;
                }
                else {
                    ++CalcCow;
                }
            }
        }
    }
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const {
    TArray<FString> ValidWords;
    for (int i = 0; i < WordList.Num(); i++) { // .Num() return number of elements (same as .length())
        if (IsIsogram(*WordList[i])) {
            if (Words[i].Len() >= 3) {
                ValidWords.Add(Words[i]);
            }
        }
    }

    return ValidWords;
}