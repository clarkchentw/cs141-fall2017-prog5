/* prog5Boggle.cpp
      Demo program to help get you started with program 5.

    Running this program looks like:
        Program: #5, Boggle
        TA: Grace Hopper, Tues 5-6
        Nov 5, 2017

        Welcome to the game of Boggle, where you play against the clock
        to see how many words you can find using adjacent letters on the
        board.  Each letter can be used only once for a given word.
 
 
        The dictionary total number of words is: 263533
        Number of words of the right length is:  259709

        Some random characters are: a c r l n e a p p u

        Do word lookups for 10 seconds.

        Enter a word: fish
        fish is in the dictionary.
        There are 3 seconds left.

        Enter a word: cat
        cat is in the dictionary.
        There are 3 seconds left.

        Enter a word: dog
        dog is in the dictionary.
        There are 1 seconds left.

        Enter a word: entropy
        entropy is in the dictionary.
        There are -3 seconds left.
        I let you finish your last move. Time is up!
 
*/
#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <cctype>     // Allows using the tolower() function
#include <ctime>      // Timer functions
#include <cstring>    // For strlen
using namespace std;

// Global constants
const char DictionaryFileName[] = "dictionary.txt";
const int MaxNumberOfWords = 263533; // Number of dictionary words
const int MinWordLength = 3;         // Minimum dictionary word size to be stored
const int MaxWordLength = 16;	     // Max word size.  Add 1 for null
const int MaxUserInputLength = 81;   // Max user input length
const int MaxWordsFoundOnBoard = 100;   // Max words found on one board
const int NumberOfLetters = 26;      // Letters in the alphabet
const int TotalSecondsToPlay = 10;   // Total number of seconds to play per board

// Game Class
class boggleBoard
{
    public: 
        boggleBoard()
        {
            letter = '?';
            posX = -1;
            posY = -1;
            setSearchPoint();
        };
        boggleBoard(char inputLetter, int inputPosX, int inputPosY){
            letter = inputLetter;
            posX = inputPosX;
            posY = inputPosY;
            setSearchPoint();
        }
        
        char getLetter(){
            return letter;
        }
        
        void setLetter(char inputLetter){
            letter = inputLetter;
        }
        
        void setSearchPoint(){
            // Right
            searchPoint[0][0] = posX+1;
            searchPoint[0][1] = posY;
            // Down-Right
            searchPoint[1][0] = posX+1;
            searchPoint[1][1] = posY+1;
            // Down
            searchPoint[2][0] = posX;
            searchPoint[2][1] = posY+1;
            // Down-Left
            searchPoint[3][0] = posX-1;
            searchPoint[3][1] = posY+1;
            // Left
            searchPoint[4][0] = posX-1;
            searchPoint[4][1] = posY;
            // Up-Left
            searchPoint[5][0] = posX-1;
            searchPoint[5][1] = posY-1;
            // Up
            searchPoint[6][0] = posX;
            searchPoint[6][1] = posY-1;
            // Up-Right
            searchPoint[7][0] = posX+1;
            searchPoint[7][1] = posY-1;
        }
        
        void getSearchPoint(int (&searchPointContainer)[8][2]){
            for (int i=0; i<8; i++){
                searchPointContainer[i][0] = searchPoint[i][0];
                searchPointContainer[i][1] = searchPoint[i][1];
            }
        }
        
        void setSearchHistory(bool inputSearchHistory){
            searchHistory = inputSearchHistory;
        }
        
        bool getSearchHistory(){
            return searchHistory;
        }
        
    private:
        char letter;
        int posX;
        int posY;
        int searchPoint[8][2];
        bool searchHistory;
};

//--------------------------------------------------------------------------------
// Display name and program information
void displayIdentifyingInformation()
{
    printf("\n");
    printf("Author: Dale Reed          \n");
    printf("Program: #5, Boggle        \n");
    printf("TA: Grace Hopper, Tues 5-6  \n");
    printf("Nov 5, 2017                \n");
    printf("\n");
}//end displayIdentifyingInformation()


//--------------------------------------------------------------------------------
// Display instructions
void displayInstructions()
{
    printf("Welcome to the game of Boggle, where you play against the clock   \n");
    printf("to see how many words you can find using adjacent letters on the  \n");
    printf("board.  Each letter can be used only once for a given word.       \n");
    printf("  \n");
 /*   printf("When prompted to provide input you may also:                      \n");
    printf("     Enter 'r' to reset the board to user-defined values.         \n");
    printf("     Enter 's' to solve the board and display all possible words. \n");
    printf("     Enter 't' to toggle the timer on/off.                        \n");
    printf("     Enter 'x' to exit the program.                               \n");
  */  printf("  \n");
}//end displayInstructions()


//---------------------------------------------------------------------------
// Read in dictionary
//    First dynamically allocate space for the dictionary.  Then read in words
// from file.  Note that the '&' is needed so that the new array address is
// passed back as a reference parameter.
void readInDictionary(
          char ** &dictionary,                      // dictionary words
          long int &numberOfWords)                  // number of words stored
{
    // Allocate space for large array of C-style strings
    dictionary = new char*[ MaxNumberOfWords];
    
    // For each array entry, allocate space for the word (C-string) to be stored there
    for (int i=0; i < MaxNumberOfWords; i++) {
        dictionary[i] = new char[ MaxWordLength+1];
        // just to be safe, initialize C-strings to all null characters
        for (int j=0; j < MaxWordLength; j++) {
            dictionary[i][j] = '\0';
        }//end for (int j=0...
    }//end for (int i...
    
    // Now read in the words from the file
    ifstream inStream;                 // declare an input stream for my use
    numberOfWords = 0;                   // Row for the current word
    inStream.open( DictionaryFileName);
    assert( ! inStream.fail() );       // make sure file open was OK
    
    // Keep repeating while input from the file yields a word
    char theWord[ 81];    // declare input space to be clearly larger than largest word
    while( inStream >> theWord) {
        int wordLength = (int)strlen( theWord);
        if( wordLength >= MinWordLength && wordLength <= MaxWordLength) {
            strcpy( dictionary[ numberOfWords], theWord);
            // increment number of words
            numberOfWords++;
        }
    }//end while( inStream...
    
    cout << "The dictionary total number of words is: " << MaxNumberOfWords << endl;
    cout << "Number of words of the right length is:  " << numberOfWords << endl;
    
    // close the file
    inStream.close();
}//end readInDictionary()


//--------------------------------------------------------------------------------------
// Use binary search to look up the search word in the dictionary array, returning index
// if found, -1 otherwise
int binarySearch( const char searchWord[ MaxWordLength+1], // word to be looked up
                  char **dictionary)               // the dictionary of words
{
    int low, mid, high;     // array indices for binary search
    int searchResult = -1;  // Stores index of word if search succeeded, else -1
    
    // Binary search for word
    low = 0;
    high = MaxNumberOfWords - 1;
    while ( low <= high)  {
        mid = (low + high) / 2;
        // searchResult negative value means word is to the left, positive value means
        // word is to the right, value of 0 means word was found
        searchResult = strcmp( searchWord, dictionary[ mid]);
        if ( searchResult == 0)  {
            // Word IS in dictionary, so return the index where the word was found
            return mid;
        }
        else if (searchResult < 0)  {
            high = mid - 1; // word should be located prior to mid location
        }
        else  {
            low = mid + 1; // word should be located after mid location
        }
    }
    
    // Word was not found
    return -1;
}//end binarySearch()


//--------------------------------------------------------------------------------------
// Check to see if the comination of word exist
bool combinationExist( const char searchWord[ MaxWordLength+1], // word to be looked up
                  char **dictionary)               // the dictionary of words
{
    int low, mid, high;     // array indices for binary search
    int searchResult = -1;  // Stores index of word if search succeeded, else -1
    
    
    // Binary search for word
    low = 0;
    high = MaxNumberOfWords - 1;
    while ( low <= high)  {
        mid = (low + high) / 2;
        
        char targetWord[ MaxWordLength+1];
        strncpy(targetWord, dictionary[ mid], strlen(searchWord));
        targetWord[strlen(searchWord)] = '\0';
        // searchResult negative value means word is to the left, positive value means
        // word is to the right, value of 0 means word was found
        searchResult = strcmp( searchWord, targetWord);
        if ( searchResult == 0)  {
            // Word IS in dictionary, so return the index where the word was found
            return true;
        }
        else if (searchResult < 0)  {
            high = mid - 1; // word should be located prior to mid location
        }
        else  {
            low = mid + 1; // word should be located after mid location
        }
    }
    
    // Word was not found
    return false;
}//end combinationExist()


//---------------------------------------------------------------------------
// Get random character
//    Find random character using a table of letter frequency counts.
// Iterate through the array and find the first position where the random number is
// less than the value stored.  The resulting index position corresponds to the
// letter to be generated (0='a', 1='b', etc.)
char getRandomCharacter()
{
    // The following table of values came from the frequency distribution of letters in the dictionary
    float letterPercentTotals[ NumberOfLetters] = {
                0.07680,  //  a
                0.09485,  //  b
                0.13527,  //  c
                0.16824,  //  d
                0.28129,  //  e
                0.29299,  //  f
                0.32033,  //  g
                0.34499,  //  h
                0.43625,  //  i
                0.43783,  //  j
                0.44627,  //  k
                0.49865,  //  l
                0.52743,  //  m
                0.59567,  //  n
                0.66222,  //  o
                0.69246,  //  p
                0.69246,  //  q
                0.76380,  //  r
                0.86042,  //  s
                0.92666,  //  t
                0.95963,  //  u
                0.96892,  //  v
                0.97616,  //  w
                0.97892,  //  x
                0.99510,  //  y
                1.00000}; //  z
    
    // generate a random number between 0..1
    // Multiply by 1.0 otherwise integer division truncates remainders
    float randomNumber = 1.0 * rand() / RAND_MAX;
    
    // Find the first position where our random number is less than the
    // value stored.  The index corresponds to the letter to be returned,
    // where 'a' is 0, 'b' is 1, and so on.
    for( int i=0; i<NumberOfLetters; i++) {
        if( randomNumber < letterPercentTotals[ i]) {
            // we found the spot.  Return the corresponding letter
            return (char) 'a' + i;
        }
    }
    
    // Sanity check
    cout << "No alphabetic character generated.  This should not have happened. Exiting program.\n";
    exit( -1);
    return ' ';   // should never get this
}//end getRandomCharacter

//--------------------------------------------------------------------------------
// Initialize board
void initializeBoard(boggleBoard arrayObject[6][6]){
    for (int x = 1; x <= 6; x++){
        int y = 1;
        arrayObject[x][y].setLetter('?');
    }
    for (int y = 1; y <= 6; y++){
        int x = 1;
        arrayObject[x][y].setLetter('?');
    }
    // Initialize Random character
    for (int x = 1; x <= 4; x++){
        for (int y=1; y <= 4; y++){
            arrayObject[x][y].setLetter(getRandomCharacter());
        }
        cout << endl;
    }
}

//--------------------------------------------------------------------------------
// Display board
void displayBoard(boggleBoard arrayObject[6][6]){
    for (int x = 1; x <= 4; x++){
        for (int y=1; y <= 4; y++){
            cout << arrayObject[x][y].getLetter() << " ";
        }
        cout << endl;
    }
    for (int x = 0; x <= 5; x++){
        for (int y=0; y <= 5; y++){
            cout << arrayObject[x][y].getLetter() << " ";
        }
        cout << endl;
    }
}

//--------------------------------------------------------------------------------
// Append letter to char*
char* appendCharToCharStar(char* charStar, char lastChar)
{
    char* newCharStar = new char[strlen(charStar) + 1 + 1];
    
    strcpy(newCharStar, charStar);
    newCharStar[strlen(charStar)] = lastChar;
    newCharStar[strlen(charStar) + 1] = '\0';
    return newCharStar;
}

//--------------------------------------------------------------------------------
// reset Search History
void resetSearchHistory(boggleBoard arrayObject[6][6]){
    for (int x = 0; x < 6; x++)
    {
        for (int y = 0; y < 6; y++)
        {
            arrayObject[x][y].setSearchHistory(false);
        }
    }
}

//--------------------------------------------------------------------------------
// Search board for word
void searchSpot(char **dictionary, boggleBoard arrayObject[6][6],int &numOfWordFound, char **wordFound, int spotX, int spotY, char* buildWord){
    // Check and set search History
    if (arrayObject[spotX][spotY].getSearchHistory()){
        return;
    }
    else
    {
        arrayObject[spotX][spotY].setSearchHistory(true);
    }
    // recursive call
    if (arrayObject[spotX][spotY].getLetter() == '?'){
        return;
    }
    else
    {
        int searchPointContainer[8][2];
        arrayObject[spotX][spotY].getSearchPoint(searchPointContainer);
        
        // Append the current letter into the build set
        buildWord = appendCharToCharStar(buildWord, arrayObject[spotX][spotY].getLetter());
        
        if (combinationExist(buildWord, dictionary))
        {
            if (binarySearch(buildWord, dictionary))
            {
                
                wordFound[numOfWordFound] = buildWord;
                numOfWordFound += 1;
                
            }
        }
        else
        {
            return;
        }
        searchSpot(dictionary, arrayObject, numOfWordFound, wordFound, spotX, spotY, buildWord);
    }
}

//--------------------------------------------------------------------------------
// Search board for word
void searchBoard(char **dictionary, boggleBoard arrayObject[6][6], char **wordFound){
    int numOfWordFound = 0;
    for (int x = 1; x <= 4; x++){
        for (int y = 1; y <= 4; y++){
            resetSearchHistory(arrayObject);
            char* buildWord;
            searchSpot(dictionary, arrayObject, numOfWordFound, wordFound, x, y, buildWord);
        }
        cout << endl;
    }
    
}

//---------------------------------------------------------------------------
int main()
{
	
    // declare variables
    char **dictionary;                // 2d array of dictionary words, dynamically allocated
    long int numberOfWords;           // how many words actually found in dictionary
    char userInput[ MaxUserInputLength];
    boggleBoard arrayObject[6][6]; // array of Object
    char **wordFound; // to store the word found
    wordFound = new char*[ MaxWordsFoundOnBoard];
    
    // Declare a variable to hold a time, and get the current time
    time_t startTime = time( NULL);

    displayIdentifyingInformation();
    displayInstructions();
    
    // read in dictionary.  numberOfWords returns the actual number of words found
    readInDictionary( dictionary, numberOfWords);
    
    // Demonstrate how to get random characters
    cout << "\nSome random characters are: ";
    for( int i=0; i<10; i++) {
       cout << getRandomCharacter() << " ";
    }
    cout << endl << endl;
    
    initializeBoard(arrayObject);
    displayBoard(arrayObject);
    
    ///////////////////////////////////////////////
    searchBoard(dictionary, arrayObject, wordFound);
    return 1;
    
    ///////////////////////////////////////////////
    
    cout << endl << endl;
    
    
    // Loop to while there is still time left
    cout << "Do word lookups for 10 seconds." << endl;
    int elapsedSeconds = 0;
    while( elapsedSeconds < TotalSecondsToPlay) {
        // Prompt for and get user input
        cout << endl;
        cout << "Enter a word: ";
        cin >> userInput;

        if( binarySearch( userInput, dictionary) != -1) {
           cout << userInput << " is in the dictionary." << endl;
        }
        else {
            cout << userInput << " is NOT in the dictionary." << endl;
        }
        
        // Calculate how many seconds have elapsed since we started the timer.
        elapsedSeconds = difftime( time( NULL), startTime);
        cout << "There are " << TotalSecondsToPlay - elapsedSeconds << " seconds left." << endl;
    }
    cout << "I let you finish your last move. Time is up!" << endl;
    
    
    return 0;   
}//end main()
