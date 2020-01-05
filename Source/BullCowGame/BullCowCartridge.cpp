
// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordsList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    FBullCowCount Count;

    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PlayerGuess
    {
       ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num()-1)]; 
    Lives = HiddenWord.Len() * 2; 
    bGameOver = false;

    PrintLine(TEXT("Welcome to Bull Cow Game!"));
    //PrintLine(TEXT("The hidden words is %s"), *HiddenWord);
    PrintLine(TEXT("The number of possible hidden words is %i"), Isograms.Num());
    PrintLine(TEXT("The hidden word is %i characters long"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives); 
    PrintLine(TEXT("Type in your guess."));
    PrintLine(TEXT("Press enter to continue..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == HiddenWord)
    {
        PrintLine(TEXT("Yea, you won!"));
        EndGame();
        return;
    }

    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The Hidden word is %i letters long, try again!"), HiddenWord.Len());
        return;
    }

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again."));
        return;
    }
    
    if(--Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have lost!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    PrintLine(TEXT("Wrong."));
    PrintLine(TEXT("You have %i lives left"), Lives);   
    // Check Bulls and Cows 
    FBullCowCount Count = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 CurrLetterIdx = 1; CurrLetterIdx < Word.Len(); CurrLetterIdx++)
    {
        for(int32 PastLetterIdx = CurrLetterIdx-1; PastLetterIdx >= 0; PastLetterIdx--)
        {
            if(Word[CurrLetterIdx] == Word[PastLetterIdx])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const
{
    TArray<FString> ValidWords;
    for(auto word: Words)
    {
        if( word.Len() >= 4 && word.Len() <= 8 && IsIsogram(word))
        {
            ValidWords.Emplace(word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIdx = 0; GuessIdx < Guess.Len(); GuessIdx++)
    {
       if(Guess[GuessIdx] == HiddenWord[GuessIdx])
       {
           Count.Bulls++;
           continue;
       }

       for(auto letter: HiddenWord)
       {
           if(letter == Guess[GuessIdx])
           {
               Count.Cows++;
               break;
           }
       }
       
    }
    return Count;
    
}