// Program 3: A Dynamically Evil World Guessing Game
// Course: CS 211, Fall 2023, UIC
// System: Advanced zyLab
// Author: Shaan Kohli

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Pattern_struct {
    char* pat;  //string pattern - exs: "hello", "-ell-", "-----"
    int count;  //frequency of pattern in the current word list
    int changes;//number of occurences of new letter in this pattern
} Pattern;

// Adds a new word to a dynamic heap-allocated array of C-strings;   
// parameters include :    
//      char*** words - array of C-strings passed-by-pointer     
//      int* numWords - current # of words in *words array
//      int* maxWords - current capacity of *words array
//      char* newWord - C-string word to be added to words
// no return value
void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {
    // checks if you need to reallocate the words array
    if (*numWords >= *maxWords) {
        *maxWords *= 2;
        char** newWords = (char**)malloc(*maxWords * sizeof(char*));

        // copies old array into new array
        for (int i = 0; i < *numWords; i++) {
            newWords[i] = (*words)[i];
        }

        // frees old array
        free(*words);
        
        // updates old array to be the new array
        *words = newWords;

    }

    // allocates memory for new word
    (*words)[*numWords] = (char*)malloc((strlen(newWord) + 1) * sizeof(char));

    // copies new word into the words array
    strcpy((*words)[*numWords], newWord);

    // increments our total number of words
    (*numWords)++;
}

// returns count of character differences between two strings;
// also includes extra characters in longer string if different lengths
int strNumMods(char* str1, char* str2) {
    int count = 0;

    // iterates character by character for each string until the end is reached
    while (*str1 != '\0' || *str2 != '\0') {

        // compares characters at current position
        if (*str1 != *str2) {
            count++;
        }

        // moves to next character in each string if they aren't at the end
        if (*str1 != '\0') {
            str1++;
        }
        if (*str2 != '\0') {
            str2++;
        }
    }

    return count;
}

// returns index of the character where two strings first differ
// if there are no differences, returns strlen(str1) which equals strlen(str2)
int strDiffInd(char* str1, char* str2) {

    int idx = 0;
    
    // loops through each character and returns index of first difference if found
    while (str1[idx] != '\0' && str2[idx] != '\0') {
        if (str1[idx] != str2[idx]) {
            return idx;
        }
        idx++;
    }

    // checks if one string is longer/shorter than the other and returns index
    if (str1[idx] == '\0' && str2[idx] != '\0') {
        return idx;
    }
    else if (str1[idx] != '\0' && str2[idx] == '\0') {
        return idx;
    } 

    // making it here means the strings are equals
    return strlen(str1);
}

// Prints out pattenrs when pattern mode is enabled
void printPatternMode(char guess, int numPatterns, Pattern* patterns) {

    printf("\nAll patterns for letter %c: \n", guess);
    for (int i = 0; i < numPatterns; i++){
        printf("  %s \tcount = %d \tchanges = %d\n", patterns[i].pat, patterns[i].count, patterns[i].changes);
    }
}

// prints out possible words that are left in the word list
void printWordMode(char** wordList, int numWords) {
    printf("Possible words are now:\n");

    // loops through all words left and prints them
    for (int i = 0; i < numWords; i++) {
        printf("  %s\n", wordList[i]);
    }
}

// prints out number of possible words remaining
void printStatsMode(int numWords) {
    printf("Number of possible words remaining: %d\n", numWords);
}

// prints out previously guessed letters
void printLetterMode(char lettersGuessed[]) {

    // loops through first 25 letters in alphabet
    for (int i = 0; i < 25; i++) {
        // loops through last 25 characters in alphabet
        for (int j = i + 1; j < 26; j++) {
            // swaps letters if not in alphabetical order
            if (lettersGuessed[i] > lettersGuessed[j]) {
                char temp = lettersGuessed[i];
                lettersGuessed[i] = lettersGuessed[j];
                lettersGuessed[j] = temp;
            }
        }
    }

    printf("Previously guessed letters: ");
   
    for (int i = 0; i < 26; i++) {
        // prints out guess only if it isn't equal to the null character
        if (lettersGuessed[i] != '\0') {
            printf("%c ", lettersGuessed[i]);
        }
    }
    printf("\n");
}
 
int main(int argc, char* argv[]) {

    printf("Welcome to the (Evil) Word Guessing Game!\n\n");

    // initial variables and default values
    bool solved = false;
    bool statsMode = false;
    bool wordListMode = false;
    bool letterListMode = false;
    bool patternListMode = false;
    int wordSize = 5;
    int numGuesses = 26;
    
    // loops through command line arguments starting at 1 (starts after file name)
    for (int i = 1; i < argc; i++) {
        //  gets word size for user input if it is valid, otherwise uses default value (5)
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            wordSize = atoi(argv[i + 1]);
            if (wordSize < 2) {
                printf("Invalid word size.\n");
                printf("Terminating program...\n");
                return 0;
            }
            // iterates i to skip the arg value
            i++;
        }
        // gets number of guesses from user input if it is valid, otherwise uses default value (26)
        else if (strcmp(argv[i], "-g") == 0 && i + 1 < argc) {
            numGuesses = atoi(argv[i + 1]);
            if (numGuesses < 1) {
                printf("Invalid number of guesses.\n");
                printf("Terminating program...\n");
                return 0;
            }
            // iterates i to skip arg value
            i++;
        }
        // turns stats mode on if selected
        else if (strcmp(argv[i], "-s") == 0) {
            statsMode = true;
        }
        // turns word list mode on if selected
        else if (strcmp(argv[i], "-w") == 0) {
            wordListMode = true;
        }
        // turns letter list mode on if selected
        else if (strcmp(argv[i], "-l") == 0) {
            letterListMode = true;
        }
        // turns pattern list mode on if selected
        else if (strcmp(argv[i], "-p") == 0) {
            patternListMode = true;
        }
        // turns all modes on if selected
        else if (strcmp(argv[i], "-v") == 0) {
            statsMode = true;
            wordListMode = true;
            letterListMode = true;
            patternListMode = true;
        }
        
        // ends program if argument type is invalid
        else {
            printf("Invalid command-line argument.\n");
            printf("Terminating program...\n");
            return 0;
        }
    }
 
    // All Stats for Game Settings
    printf("Game Settings:\n");
    printf("  Word Size = %d\n", wordSize);
    printf("  Number of Guesses = %d\n", numGuesses);
    printf("  View Stats Mode = %s\n", statsMode ? "ON" : "OFF");
    printf("  View Word List Mode = %s\n", wordListMode ? "ON" : "OFF");
    printf("  View Letter List Mode = %s\n", letterListMode ? "ON" : "OFF");
    printf("  View Pattern List Mode = %s\n", patternListMode ? "ON" : "OFF");
    
    int capacity = 4;
    char** wordList = (char**)malloc(capacity*sizeof(char*));
    
    // opens file
    FILE* fileOpened = fopen("dictionary.txt", "r");

    // variables needed to print out game settings
    char maxWordLength[100];
    char longestWord[100] = "";
    int totalWords = 0;
    int numWords = 0;
    
    // goes line by line in the dictionary
    while (fgets(maxWordLength, sizeof(maxWordLength), fileOpened)) {

        // removes newline and replaces it with null character for each word
        int length = strlen(maxWordLength);
        if (maxWordLength[length - 1] == '\n') {
            maxWordLength[length - 1] = '\0';
        }

        // adds word to list
        if (strlen(maxWordLength) == wordSize) {
            addWord(&wordList, &numWords, &capacity, maxWordLength);
        }

        // counts all words
        totalWords++;

        // finds max word length
        if (strlen(maxWordLength) > strlen(longestWord)) {
            strcpy(longestWord, maxWordLength);
        }
    }

    // closes the file
    fclose(fileOpened);
 
    // prints only if stats mode is turned on
    if (statsMode) {
        printf("The dictionary contains %d words total.\n", totalWords);
        printf("The longest word %s has %zu chars.\n", longestWord, strlen(longestWord));
        printf("The dictionary contains %d words of length %d.\n", numWords, wordSize);
        printf("Max size of the dynamic words array is %d.\n", capacity);
    }

    // prints only if word list mode is turned on
    if (wordListMode) {
        printf("Words of length %d:\n", wordSize);
        for (int i = 0; i < numWords; i++) {
            printf("  %s\n", wordList[i]);
        }
    }

    // checks if the dictionary contains words
    if (numWords == 0) {
        printf("Dictionary has no words of length %d.\n", wordSize);
        printf("Terminating program...\n");
        return 0;
    }

    // prints out the blank pattern of the word given the word size
    printf("The word pattern is: ");
    for (int i = 0; i < wordSize; i++) {
        printf("-");
    }
    printf("\n");
    
    // keeps track of all letters guessed
    char lettersGuessed[26] = "";
    printf("\nNumber of guesses remaining: %d\n", numGuesses);

    // creates our final word pattern
    char finalWord[wordSize];
    for (int i = 0; i < wordSize; i++) {
        finalWord[i] = '-';
    }
    finalWord[wordSize] = '\0';


    // makes sure you haven't solved the word yet and you have enough guesses
    while (!solved && numGuesses > 0) {
        
        // initialize pattern variables
        int maxPatterns = 4;
        int numPatterns = 0;

        // allocates memory for our patterns
        Pattern* patterns = (Pattern*)malloc(sizeof(Pattern) * maxPatterns);
        for (int i = 0; i < maxPatterns; i++) {
            patterns[i].count = 0;
            patterns[i].changes = 0;
            patterns[i].pat = NULL;
        }

        // initializes most common pattern and its count
        Pattern* mostCommonPat = NULL;
        int maxCount = 0;

        if (letterListMode) {
            printLetterMode(lettersGuessed);
        }
        printf("Guess a letter (# to end game): ");
        
        char guess;
        scanf(" %c", &guess);

        // ends the game if you get the #
        if (guess == '#') {
            printf("Terminating game...\n");
            break;
        }

        // checks for all uppercase letters entered
        if (!islower(guess)) {
            printf("\nInvalid letter...\n");
            continue;
        }

        // checks if user has already guessed the letter
        if (strchr(lettersGuessed, guess) != NULL) {
            printf("Letter previously guessed...\n");
            continue;
        }

        lettersGuessed[strlen(lettersGuessed)] = guess;

        // loops through all words
        for (int i = 0; i < numWords; i++) {
            // creates a temp and copies the final word into it
            char temp[wordSize];
            strcpy(temp, finalWord);
            int changes = 0;

            // loops through each character in the word
            for (int j = 0; j < wordSize; j++) {
                // updates temp with the guess if it is in there and increments its changes count
                if (wordList[i][j] == guess) {
                    temp[j] = guess;
                    changes++;
                }    
            }
            bool flag = false;

            // loops through all the patterns
            for (int k = 0; k < numPatterns; k++) {
                // updates pattern count and changes if the pattern is equal to our temp. Also sets the flag to true
                if (strcmp(patterns[k].pat, temp) == 0) {
                    patterns[k].count++;
                    patterns[k].changes = changes;
                    flag = true;
                }
            }

            // allocates more memory if we reach our max patterns
            if (!flag) {
                if (numPatterns == maxPatterns) {
                    maxPatterns *= 2;  
                    patterns = (Pattern*)realloc(patterns, maxPatterns * sizeof(Pattern));
                }

                patterns[numPatterns].pat = (char*)malloc((wordSize + 1) * sizeof(char));
                strcpy(patterns[numPatterns].pat, temp);
                patterns[numPatterns].count = 1;
                patterns[numPatterns].changes = changes;
                numPatterns++; 
            }
        }

        // loops through all patterns
        for (int i = 0; i < numPatterns; i++) {
            // gets easy access to the count and changes of each pattern
            int count = patterns[i].count;
            int changes = patterns[i].changes;

            // finds the max count
            if (count > maxCount) {
                mostCommonPat = &patterns[i];
                maxCount = count;
            }
            // handles if count and max count are equal
            else if (count == maxCount) {
                if (changes < mostCommonPat->changes) {
                    mostCommonPat = &patterns[i];
                }
                // handles tiebreaker edge case
                else if (changes == mostCommonPat->changes) {
                    // gets position of guessed letter and the current position
                    int guessedLetPos = strDiffInd(patterns[i].pat, finalWord);
                    int currentPos = strDiffInd(mostCommonPat->pat, finalWord);

                    // keeps looping through until you find the correct pattern
                    while (guessedLetPos == currentPos) {
                        char* finalTemp = &(finalWord[guessedLetPos + 1]);
                        char* mostCommonPatTemp = &(mostCommonPat->pat[guessedLetPos + 1]);
                        char* patTemp = &(patterns[i].pat[guessedLetPos + 1]);
                        guessedLetPos = strDiffInd(patTemp, finalTemp);
                        currentPos = strDiffInd(mostCommonPatTemp, finalTemp);
                    }

                    if (guessedLetPos < currentPos) {
                        mostCommonPat = &patterns[i];
                    }
                }
            }
        }

        // creates updated word list and a count for the number of words
        char** updatedWordList = (char**)malloc(numWords * sizeof(char*));
        int updatedNumWords = 0;

        // loops through all words
        for (int i = 0; i < numWords; i++) {

            // creates a new word temp and copies final word into it
            char wordTemp[wordSize];
            strcpy(wordTemp, finalWord);
            int wordChanges = 0;

            // loops through each character in the word
            for (int j = 0; j < wordSize; j++) {
                // updates word temp and the number of changes if the guess is in the pattern
                if (wordList[i][j] == guess) {
                    wordTemp[j] = guess;
                    wordChanges++;
                }
            }

            // handles if the temp matches the most common pattern
            if (strcmp(wordTemp, mostCommonPat->pat) == 0) {
                // INSERTS WORD[i][j]
                updatedWordList[updatedNumWords] = wordList[i];
                updatedNumWords++;
            }
            // handles if temp and most common pattern are different
            else {
                free(wordList[i]);
            }
        }

        free(wordList);
        // updates the word list and the number of words
        wordList = updatedWordList;
        numWords = updatedNumWords;
        

        // loops through the characters in words
        bool letterFound = false;
        for (int i = 0; i < wordSize; i++) {
            // sets the flag to true indiciating that the letter is in the pattern
            if (mostCommonPat->pat[i] == guess) {
                letterFound = true;
                break;
            }
        }

        // creates the current final word
        strcpy(finalWord, mostCommonPat->pat);

        // prints statements if letter is in the pattern
        if (letterFound) {

            if (patternListMode) {
                printPatternMode(guess, numPatterns, patterns);
            }
            printf("Good guess! The word has at least one %c.\n", guess);
            if (statsMode) {
                printStatsMode(numWords);
            }
            if (wordListMode) {
                printWordMode(wordList, numWords);
            }
            printf("The word pattern is: %s\n", finalWord);
        }
        // decrements guesses and prints statements if the letter isn't in the pattern
        else {
            numGuesses--;
            if (patternListMode) {
                printPatternMode(guess, numPatterns, patterns);
            }
            printf("Oops, there are no %c's. You used a guess.\n", guess);
            
            if (statsMode) {
                printStatsMode(numWords);
            }
            if (wordListMode) {
                printWordMode(wordList, numWords);
            }
            
            printf("The word pattern is: %s\n", finalWord);
        }
        
        // sets solved to true/false based on whether or not it has been solved 
        solved = (strchr(finalWord, '-') == NULL);

        // makes sure statement is only printed when you haven't solved the word and have enough guesses
        if (numGuesses > 0 && !solved) {
            printf("\nNumber of guesses remaining: %d\n", numGuesses);
        }

        // frees all the patterns after each iteration
        for (int i = 0; i < numPatterns; i++) {
            free(patterns[i].pat);
        }
        free(patterns);

    }


    if (solved) {
        printf("\nYou solved the word!\n");
        printf("The word is: %s\n", finalWord);
        printf("Game over.\n");
    }
    else if (numGuesses < 1){
        printf("You ran out of guesses and did not solve the word.\n");
        printf("The word is: %s\n", wordList[0]);
        printf("Game over.\n");
    }

    // frees the word list
    for (int i = 0; i < numWords; i++) {
        free(wordList[i]);
    }
    free(wordList);

    return 0;
}
