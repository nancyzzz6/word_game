#include "utilities.h"
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

    const int MAXWORDS = 9000;
    const char WORDFILENAME[] = "/Users/nancyzzz/Desktop/CS 31/words.txt";
    int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum);
    bool IsAllLower(char word[]);
    int Lookup (const char words[][MAXWORDLEN+1], int nWords, char word[]);
    int getNumSilver(char answerWithoutGold[], char probeWithoutGold[]);
    void update(char answer[], char probe[], int& numGold, int& numSilver);

    int main()
    {
        //Creat a word list for randomly selecting hidden words.
        char wordList[MAXWORDS][MAXWORDLEN+1];
        int nWords = getWords(wordList, MAXWORDS, WORDFILENAME);
        if (nWords < 1)
        {
            cout << "No words were loaded, so I can't play the game.";
            return 0;
        }
        
        //Ask for number of rounds want to play.
        cout << "How many rounds do you want to play? " ;
        int numRounds;
        cin >> numRounds;
        cin.ignore(10000, '\n');
        
        if(numRounds <= 0)
        {
            //Test if the rounds are valid.
            cout << "The number of rounds must be positive."<<endl;
            return 0;
        }
        else
        {
            cout << endl;
            int min = 9999;
            int max = -9999;
            double sum = 0;
            for(int i = 1 ; i <= numRounds; ++i)
            {
                cout << "Round " << i << endl;
                
                // Generate a random index
                int wordnum = randInt(0, nWords-1);
                
                // Find the number of letters of the word at that index from the word list.
                size_t numLetters = strlen(wordList[wordnum]);
                cout << "The hidden word is " << numLetters <<" letters long."<<endl;
                
                // Play each round.
                int numTries = playOneRound(wordList, nWords, wordnum);
                
                // Update min, max, sum, avg after each round of game.
                if(numTries < min)
                {
                    min = numTries;
                }
                if(numTries > max)
                {
                    max = numTries;
                }
                sum += numTries;
                double avg = sum / i;
                if(numTries == 1)
                    cout << "You got it in " << numTries << " try."<<endl;
                else
                    cout << "You got it in " << numTries << " tries."<<endl;
                
                cout.setf(ios::fixed);
                cout.precision(2);
                cout << "Average: " << avg << ", minimum: " << min << ", maximum: " << max << endl;
                cout << endl;
            }
        }
    }

int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum)
{
    if (nWords <= 0 || wordnum < 0 || wordnum >= nWords)
    {
        return -1;
    }
    
    // Get the hidden answer word from the word list.
    char answer[MAXWORDLEN+1];
    strcpy(answer, words[wordnum]);
    char probe[20];
    int numTries = 0;
    
    // Repetedly getting probe word when probe word doesn't equal to answer.
    while (strcmp(answer, probe) != 0)
    {
        cout << "Probe word: ";
        cin.getline(probe,200);
        bool noError = true;
        // Verify that if the probe is valid.
        if (strlen(probe) < 4 || strlen(probe) > 6 || !IsAllLower(probe))
        {
            cout << "Your probe word must be a word of 4 to 6 lower case letters."<< endl;
            noError = false;
        }
        else if (Lookup(words, nWords, probe) == -1)
        {
            cout << "I don't know that word." << endl;
            noError = false;
        }
        
        if ( noError )
        {
            // Get num gold and num silver
            numTries++;
            int numGold = 0;
            int numSilver = 0;
            update(answer, probe, numGold, numSilver);
            
            if (numGold != strlen(probe) || numGold != strlen(answer))
            {
                cout << "Golds: "<< numGold <<", Silvers: "<< numSilver << endl;
            }
        }
    }
    return numTries;
}

// Check if the input probe is all lower case
bool IsAllLower(char word[])
{
    for(int i = 0; i < strlen(word); ++i)
    {
        if ( !islower(word[i]))
        {
            return false;
        }
    }
    return true;
}

// Verify if the probe is in the word list.
int Lookup (const char words[][MAXWORDLEN+1], int nWords, char word[])
{
    for(int i = 0; i < nWords; ++i)
    {
        if (strcmp(word , words[i]) == 0)
        {
            return 0;
        }
    }
    return -1;
}

// Update the number of gold and siver.
void update(char answer[], char probe[], int& numGold, int& numSilver)
{
    char answerWithoutGold[MAXWORDLEN+1] = "";
    char probeWithoutGold[MAXWORDLEN+1] = "";
    for (int i = 0; i < strlen(probe); i++)
    {
        // Find the gold.
        if (i < strlen(answer)) {
            if (answer[i] == probe[i])
            {
                numGold++;
            }
            else
            {
                // Generated new probe and answer without gold
                char currAnsChar[MAXWORDLEN+1] = "";
                currAnsChar[0] = answer[i];
                char currProbeChar[MAXWORDLEN+1] = "";
                currProbeChar[0] = probe[i];
                strcat(answerWithoutGold, currAnsChar);
                strcat(probeWithoutGold, currProbeChar);
            }
        }
    }
    
    // when lengths are different,
    // Fill the non compared part to the withoutgold C string.
    for(int i = 0; i<(int)strlen(answer)-(int)strlen(probe); i++)
    {
        char ansWithProbeLenth[MAXWORDLEN+1] = "";
        ansWithProbeLenth[0] = answer[strlen(probe)+i];
        strcat(answerWithoutGold, ansWithProbeLenth);
    }
    for(int i = 0; i<(int)strlen(probe)-(int)strlen(answer); i++)
    {
        char probeWithAnswerLenth[MAXWORDLEN+1] = "";
        probeWithAnswerLenth[0] = probe[strlen(answer)+i];
        strcat(probeWithoutGold, probeWithAnswerLenth);
    }
    numSilver += getNumSilver(answerWithoutGold, probeWithoutGold);
}

// Compare and contrust answer with probe, and Compare and contrust probe with answer.
// The minimum same values is the silver.
int getNumSilver(char answerWithoutGold[], char probeWithoutGold[])
{
    int ijSilver = 0;
    int jiSilver = 0;
    for (int i = 0; i < strlen(answerWithoutGold); i++)
    {
        for(int j = 0; j < strlen(probeWithoutGold); j++)
        {
            if (answerWithoutGold[i] == probeWithoutGold[j])
            {
                ijSilver++;
                break;
            }
                
        }
    }
    for(int j = 0; j < strlen(probeWithoutGold); j++)
    {
        for (int i = 0; i < strlen(answerWithoutGold); i++)
        {
            if (answerWithoutGold[i] == probeWithoutGold[j])
            {
                jiSilver++;
                break;
            }
                
        }
    }
    if(ijSilver < jiSilver)
        return ijSilver;
    else
        return jiSilver;
}
