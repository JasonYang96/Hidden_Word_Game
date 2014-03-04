#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <ctime>
using namespace std;

//global constants

//maximum number of words in the list of words
const int MAXWORDS = 10000;
//minimum length of each word in the list of words
const int MINWORDLENGTH = 4;
//maximum length of each word in the list of words
const int MAXWORDLENGTH = 6;
//maximum length of each trial word user inputs
const int MAXTRIALWORDLENGTH = 100;
//location of text file containing list of words
const char WORDFILENAME[] = "C:/Users/Jasoniful/Dropbox/projects/P5/words.txt";

//functions
int randInt(int lowest, int highest);
int loadWords(char words[][MAXWORDLENGTH+1], int maxWords);
int manageOneRound(const char words[][MAXWORDLENGTH+1], int num, int wordnum);

int main()
{
	//array holding the list of words
	char wordList[MAXWORDS][MAXWORDLENGTH +1];
	//represents number of words in the wordList array
	int nWords = loadWords(wordList, MAXWORDS);

	//checks if any words were loaded
	if (nWords < 1)
	{
		cout << "No words were loaded, so I can't play the game" << endl;
		return 0;
	}

	//asks user for number of rounds
	cout << "How many rounds do you want to play? ";
	int rounds;
	cin >> rounds;
	cin.ignore(10000, '\n');

	//checks if it is a valid number of rounds
	if (rounds <= 0)
	{
		cout << "The number of rounds must be positive" << endl;
		return 0;
	}

	//variables used to calculate end-game results
	double totalScore = 0;
	int minimum;
	int maximum;

	//creates a new seed every time game is played
	srand(time(0));

	//for loop that runs through each round
	for (int k = 1; k < rounds + 1; k++)
	{
		//output Round number and hidden word length
		cout << "\nRound " << k << endl;
		int randNumber = randInt(0,nWords - 1);
		cout << "The hidden word is " << strlen(wordList[randNumber]) << " letters long" << endl;
		int score = manageOneRound(wordList, nWords, randNumber);

		//outputting correct output based on score
		if (score == -1)
		{
			cerr << "Invalid arguments for manageOneRoundFunctions" << endl;;
			return 0;
		}
		else if(score == 1)
			cout << "You got it in 1 try" << endl;
		else
			cout << "You got it in " << score << " tries" << endl;

		//calculating average
		double average = (totalScore += score)/k;

		//calculating minimum and maximum
		if (k == 1)
		{
			minimum = score;
			maximum = score;
		}
		if (minimum > score)
			minimum = score;
		if (maximum < score)
			maximum = score;

		//output results
		cout.setf(ios::fixed);
		cout.setf(ios::showpoint);
		cout.precision(2);
		cout << "Average: " << average << ", minimum: " << minimum << ", maximum: " << maximum << endl;
	}
}

//random number generator
int randInt(int lowest, int highest)
	{
	    if (highest < lowest)
	        swap(highest, lowest);
	    return lowest + (rand() % (highest - lowest + 1));
	}

//Precondition: words is a C-string array with strings that are at most MAXWORDLENGTH characters long
//Postcondition: loads up to maxWords words into the array
//Returns number of words loaded into array
int loadWords(char words[][MAXWORDLENGTH+1], int maxWords)
{
    ifstream wordfile(WORDFILENAME);
    if ( ! wordfile)
    {
        cout << "Cannot open " << WORDFILENAME << endl;
        return -1;
    }
    const int LINELIMIT = 10000;
    char line[LINELIMIT];
    int numWords = 0;
    while (wordfile.getline(line, LINELIMIT))
    {
        if (numWords == maxWords)
        {
            cout << "Using only the first " << numWords
                 << " words from " << WORDFILENAME << endl;
            break;
        }
        int k;
        for (k = 0; islower(line[k]); k++)
            ;

          // The following two lines fix things for non-Windows users and
          // are harmless for Windows users.
        if (line[k] == '\r')
                line[k] = '\0';

        if (line[k] == '\0'  &&  k >= MINWORDLENGTH  &&  k <= MAXWORDLENGTH)
        {
            strcpy(words[numWords], line);
            numWords++;
        }
    }
    return numWords;
}

//Precondition: words is a C-string array with strings that are at most MAXWORDLENGTH characters long where
//num represents the number of words in the array and wordnum represents the seed used to provide the hidden word
//Postcondition: plays one round of the Word Deduction TV game show
//returns the score for that round
int manageOneRound(const char words[][MAXWORDLENGTH + 1], int num, int wordnum)
{
	//making sure arguments are valid
	if (num < 0 || wordnum < 0 || wordnum >= num)
		return -1;

	int score = 0; //score represents number of times player took to guess the word

	//represents an array of the positions where matches occur so loop will skip those positions
	int matchPositions[MAXWORDLENGTH];
	char trialWord[MAXTRIALWORDLENGTH+1];
	char hiddenWord[MAXWORDLENGTH+1];
	strcpy(hiddenWord, words[wordnum]);
	
	//do-while loop that loops if incorrect trial word input
	do
	{
		int repeat = 0; //repeat represents if trial word input will loop due to invalid trial word input
		int trialScore = 0; //represents # of tries used to guess correct word
		score++;
		
		//do-while loop if invalid trial word input
		do
		{
			//set repeat back to 0 if looped
			repeat = 0;

			cout << "Trial word: ";
			cin.getline(trialWord, MAXTRIALWORDLENGTH, '\n');

			//makes sure trial word is all lowercase letters
			for (int k = 0; trialWord[k] != '\0'; k++)
			{
				if (!(isalpha(trialWord[k]) && islower(trialWord[k])))
				{
						cout << "Your trial word must be a word of 4 to 6 lower case letters" << endl;
						repeat = 1;
						score++;
						break;
				}
			}
			//if trial word is not all lowercase letter, it won't check any other tests
			if (repeat == 1)
				continue;

			//makes sure trial word is between 4-6 letters long
			if (strlen(trialWord) > 6 || strlen(trialWord) < 4)
			{
					cout << "Your trial word must be a word of 4 to 6 lower case letters" << endl;
					repeat = 1;
					score++;
					continue;
			}

			//check if word is in array
			for (int k = 0; k < num; k++)
			{
				if (strcmp(words[k], trialWord) == 0)
					break;

				if (k == num-1)
				{
					cout << "I don't know that word" << endl;
					repeat = 1;
					score++;
					break;
				}
			}
		} while (repeat == 1);

		//if trial word and hidden word are the same, break and return score
		if (strcmp(trialWord, hiddenWord) == 0)
				break;

		//matchPositions initialized to -1 in case position of match is 0
		for (int k = 0; k != MAXWORDLENGTH; k++)
		{
			matchPositions[k] = -1;
		}

		int positionInArray = 0; //represents position in array matchPositions
		//calculating number of right letters in trial word
		for (int j = 0; trialWord[j] != '\0'; j++)
		{
			for (int k = 0; hiddenWord[k] != '\0'; k++)
			{
				//if checking a position that is already matched, skip it
				for (int i = 0; i != positionInArray + 1; i++)
				{
					if (k == matchPositions[i] && k != MAXWORDLENGTH + 1)
					{
						k++;
						continue;
					}
				}

				if (trialWord[j] == hiddenWord[k])
				{
					matchPositions[positionInArray] = k;
					trialScore++;
					positionInArray++;
					break;
				}

				//error is created if k becomes last position
				//in the loop, solves this problem
				if (hiddenWord[k] == '\0')
					k--;
			}
		}
		cout << trialScore << endl;
	} while (strcmp(trialWord, hiddenWord) != 0);
	return score;
}