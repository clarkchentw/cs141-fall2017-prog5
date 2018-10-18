/*------------------------------------------------------
 
Overall Score out of 100: 81

Grading comments in the code below are marked with //-!-

 Program execution (55 points total)
 A. 5: Using classes to implement the program                  | 
 B. 5: Allow playing. No need to check if word on the board    |
       However, the word should be checked if it is in the     |
       dictionary                                              |
 C. 3: When ‘r’ is enter, prompt user to enter 16 characters   |
       to reset the board. And the board is reset correctly    | 
 D.20: Check user input if it is on the board                  | -2 not handeling the repeated word
 E. 5: Display error message if the word is not on the board   |  
       or it is not in the dictionary                          |                                     
 F. 5: Have a boolean array, same size as words in dictionary  |  -5             
       Display the words found so far in alphabetical order    | 
       Shortest first     
 G. 10: Display all possible words when user enters ‘s’        |  -10
 H.  2: Allow toggle timer on and off when user enters ‘t’     |  -2   
 
 Program Style (45 points total):
 A.  5: Followed program naming conventions     |
 B. 10: Meaningful identifier names             |
 C. 10: Comments                                |
 D.  5: Functional Decomposition                |
 E. 10: Appropriate data and control structures |
 F.  5: Code Layout                             |
*/
/* --------------------------------------------------------
 * Author:  Clark Chen
 * Program: #5, Boggle
 * Lab:     Tues 9am
 * TA:      Moumita Samanta
 * Date:    Nov 12, 2017
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
const int TotalSecondsToPlay = 60;   // Total number of seconds to play per board

//--------------------------------------------------------------------------------
// Game Class
class bogglePiece
{
    public: 
        static int numOfWordFound; // static variable for num of word found;
        bogglePiece()
        {
            letter = '!';
            posX = -1;
            posY = -1;
            setSearchPoint();
            nextSearchIndexX = 0;
            nextSearchIndexY = 0;
            usedHistory = false;
        };
        bogglePiece(char sinputLetter, int inputPosX, int inputPosY){
            letter = inputLetter;
            posX = inputPosX;
            posY = inputPosY;
            setSearchPoint();
            nextSearchIndexX = 0;
            nextSearchIndexY = 0;
            usedHistory = false;
        }
        
        char getLetter(){
            return letter;
        }
        
        void setX(int x)
        {
            posX = x;
            setSearchPoint();
        }
        
        void setY(int y)
        {
            posY = y;
            setSearchPoint();
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
        
        void setUsedHistory(bool inputUsedHistory){
            usedHistory = inputUsedHistory;
        }
        
        void resetNextSearchIndex()
        {
            nextSearchIndexX = 0;
            nextSearchIndexY = 0;
        }
        
        int nextX(bool viewOnly=false)
        {
            int index = nextSearchIndexX;
            
            if (!viewOnly){
                nextSearchIndexX++;
            }
            return searchPoint[index][0];
        }
        
        int nextY(bool viewOnly=false)
        {
            int index = nextSearchIndexY;
            if (!viewOnly){
                nextSearchIndexY++;
            }
            return searchPoint[index][1];
        }
        
        bool getUsedHistory(){
            return usedHistory;
        }
        
        int getCurrentSearchIndex(){
            if (nextSearchIndexX == nextSearchIndexY){
                return nextSearchIndexX;
            }
            else
            {
                return 999;
            }
        }
        
        void resetPiece(){
            letter = '!';
            posX = -1;
            posY = -1;
            setSearchPoint();
            nextSearchIndexX = 0;
            nextSearchIndexY = 0;
            usedHistory = false;
        }
        
    private:
        char letter;
        int posX;
        int posY;
        int searchPoint[8][2];
        int nextSearchIndexX;
        int nextSearchIndexY;
        bool usedHistory;
}; // end class bogglePiece


//--------------------------------------------------------------------------------
// Contsainer for the wordFound
class wordFoundContainer
{
    public:
        static int numOfWordFound;
        wordFoundContainer(){
            isSpaceUsed = false;
        }
        wordFoundContainer(char* newWord){
            setWord(newWord);
            isSpaceUsed = true;
        }
        void setWord(char* newWord)
        {
            strcpy(word, newWord);
			numOfWordFound++;
            
        }
        
        char* getWord()
        {
            return word;
        }
        
        void resetWord(){
            word[0] = '\0';
            isSpaceUsed = false;
        }
        static void resetNumOfWordFound()
        {
            numOfWordFound = 0;
        }
        
    private:
        char* word = new char[MaxWordLength];
        bool isSpaceUsed;
}; // end class wordFoundContainer
int wordFoundContainer::numOfWordFound = 0; // initialize numOfWordFound in wordFoundContainer Class


//--------------------------------------------------------------------------------
// Display name and program information
void displayIdentifyingInformation()
{
	cout << endl;
	cout << "Author:  Clark Chen" << endl;
	cout << "Program: #5, Boggle" << endl;
    cout << "Lab:     Tues 9am" << endl;
    cout << "TA:      Moumita Samanta" << endl;
    cout << "Date:    Nov 12, 2017" << endl << endl;
}//end displayIdentifyingInformation()


//--------------------------------------------------------------------------------
// Display instructions
void displayInstructions()
{
    printf("Welcome to the game of Boggle, where you play against the clock   \n");
    printf("to see how many words you can find using adjacent letters on the  \n");
    printf("board.  Each letter can be used only once for a given word.       \n");
    printf("  \n");
    printf("When prompted to provide input you may also:                      \n");
    printf("     Enter 'r' to reset the board to user-defined values.         \n");
    printf("     Enter 's' to solve the board and display all possible words. \n");
    printf("     Enter 't' to toggle the timer on/off.                        \n");
    printf("     Enter 'x' to exit the program.                               \n");
    printf("  \n");
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
// Use binary search to look up the search word in the dictionary array
// if found, return true otherwise false
bool binarySearch( const char searchWord[ MaxWordLength+1], // word to be looked up
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
            // Word IS in dictionary, return true
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
        
        // Check if the combination is possible by shorten the targetWord to the
        // same size as the searchWord
        char targetWord[ MaxWordLength+1];
        strncpy(targetWord, dictionary[ mid], strlen(searchWord));
        targetWord[strlen(searchWord)] = '\0';
        
        // searchResult negative value means word is to the left, positive value means
        // word is to the right, value of 0 means word was found
        searchResult = strcmp( searchWord, targetWord);
        if ( searchResult == 0)  {
            // Word IS possible in dictionary, so return true
            return true;
        }
        else if (searchResult < 0)  {
            high = mid - 1; // word should be located prior to mid location
        }
        else  {
            low = mid + 1; // word should be located after mid location
        }
    }
    
    // Word was not possible
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
// input true in customBoard to enable custom board initialization
void initializeBoard(bogglePiece bogglePieceObjectArray[6][6], wordFoundContainer wordFound[MaxWordsFoundOnBoard], 
						bool customBoard=false, 
						const char userInput[ MaxWordLength+1]=new char[ MaxWordLength+1])
{
    
    for(int i = 0; i < MaxWordsFoundOnBoard; i++){
        wordFound[i].resetWord();
    }
    
    wordFoundContainer::resetNumOfWordFound();
    
    // Reset the array
    for (int y = 0; y <= 5; y++){
        for (int x=0; x <= 5; x++){
            bogglePieceObjectArray[x][y].resetPiece();
            bogglePieceObjectArray[x][y].setLetter('?');
            bogglePieceObjectArray[x][y].setX(x);
            bogglePieceObjectArray[x][y].setY(y);
            
        }
    }
    
    
    if (!customBoard){
    	// Initialize Random character
	    for (int y = 1; y <= 4; y++){
	        for (int x=1; x <= 4; x++){
	            bogglePieceObjectArray[x][y].setLetter(getRandomCharacter());
	        }
	    }
	}else{
		// Initialize using userInput
		int userInputIndex = 0;
	    for (int y = 1; y <= 4; y++){
	        for (int x=1; x <= 4; x++){
	            bogglePieceObjectArray[x][y].setLetter(userInput[userInputIndex++]);
	        }
	    }
	}
	
}// end initializeBoard()


//--------------------------------------------------------------------------------
// Display board
void displayBoard(bogglePiece bogglePieceObjectArray[6][6]){
    for (int y = 1; y < 5; y++)
    {
        for (int x = 1; x < 5; x++)
        {
            cout << bogglePieceObjectArray[x][y].getLetter() << " ";
        }
        cout << endl;
    }
}// end displayBoard(


//--------------------------------------------------------------------------------
// Display all piece's current usage status (debug)
void displayPieceUsage(bogglePiece bogglePieceObjectArray[6][6]){
    for (int y = 1; y < 5; y++)
    {
        for (int x = 1; x < 5; x++)
        {
            if (bogglePieceObjectArray[x][y].getUsedHistory() == true)
            {
                cout << "T" << " ";
            }
            else if (bogglePieceObjectArray[x][y].getUsedHistory() == false)
            {
                cout << "F" << " ";
            }
            else
            {
                cout << "?" << " ";
            }
            
        }
        cout << endl;
    }
    cout << endl;
}// end displayPieceUsage


//--------------------------------------------------------------------------------
// Display current selected point (debug)
void displaySelectedPoint(bogglePiece bogglePieceObjectArray[6][6], int posX, int posY){
    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 6; x++)
        {
            if (x == posX && y == posY){
                cout << "* ";
            }
            else
            {
                cout << bogglePieceObjectArray[x][y].getLetter() << " ";
            }
            
        }
        cout << endl;
    }
}// end displaySelectedPoint()


//--------------------------------------------------------------------------------
// Append letter to char*
char* appendCharToCharStar(char* charStar, char lastChar)
{
    int charStarLength = strlen(charStar);
    char* newCharStar = new char[MaxWordLength+1];
    strcpy(newCharStar, charStar);
    newCharStar[strlen(charStar)] = lastChar;
    newCharStar[strlen(charStar) + 1] = '\0';
    return newCharStar;
} // end appendCharToCharStar()


//--------------------------------------------------------------------------------
// Delete the last char in char*
void deleteLastCharStarLetter(char*& charStar)
{
    int charStarLength = strlen(charStar);
    charStar[charStarLength-1] = '\0';
}// end deleteLastCharStarLetter()


//--------------------------------------------------------------------------------
// Reset Search History in bogglePiece
void resetUsedHistory(bogglePiece bogglePieceObjectArray[6][6]){
    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 6; x++)
        {
            bogglePieceObjectArray[x][y].setUsedHistory(false);
            bogglePieceObjectArray[x][y].resetNextSearchIndex();
        }
    }
}// end resetUsedHistory()


//--------------------------------------------------------------------------------
// Search board begin from a particular spot, with recursive
void searchSpot(char **dictionary, bogglePiece bogglePieceObjectArray[6][6], wordFoundContainer wordFound[MaxWordsFoundOnBoard], int spotX, int spotY, char*& buildWord, int*& (buildWordPosHistoryX), int*& (buildWordPosHistoryY), int &buildWordPosHistoryIndex){
    
    // Check used history
    if (bogglePieceObjectArray[spotX][spotY].getUsedHistory()){
        return;
    }
    
    // Check if no more nearby letter to test
    if (bogglePieceObjectArray[spotX][spotY].getCurrentSearchIndex() >= 8){
        return;
    }
    
    // recursive call
    if (!isalpha(bogglePieceObjectArray[spotX][spotY].getLetter())){
        return;
    }
    else
    {
        // Prepare search point to retrive 
        int searchPointContainer[8][2];
        bogglePieceObjectArray[spotX][spotY].getSearchPoint(searchPointContainer);
        // Append the current letter into the build set
        
        buildWord = appendCharToCharStar(buildWord, bogglePieceObjectArray[spotX][spotY].getLetter());
        buildWordPosHistoryX[buildWordPosHistoryIndex+1] = spotX;
        buildWordPosHistoryY[buildWordPosHistoryIndex+1] = spotY;
        buildWordPosHistoryIndex = buildWordPosHistoryIndex + 1;
        
        if (combinationExist(buildWord, dictionary))
        {
            // Set the word usage to true, if the word can be used
            bogglePieceObjectArray[spotX][spotY].setUsedHistory(true);
            
            if (binarySearch(buildWord, dictionary))
            {
                wordFound[wordFoundContainer::numOfWordFound].setWord(buildWord);
            }
        }
        else
        {
            deleteLastCharStarLetter(buildWord);
            // reset search history for last word
            bogglePieceObjectArray[buildWordPosHistoryX[buildWordPosHistoryIndex]][buildWordPosHistoryY[buildWordPosHistoryIndex]].setUsedHistory(false);
            buildWordPosHistoryIndex = buildWordPosHistoryIndex - 1;
            
            return;
        }
        
        while (bogglePieceObjectArray[spotX][spotY].getCurrentSearchIndex() < 8){
            searchSpot(dictionary, bogglePieceObjectArray, wordFound, bogglePieceObjectArray[spotX][spotY].nextX(), bogglePieceObjectArray[spotX][spotY].nextY(), buildWord, buildWordPosHistoryX, buildWordPosHistoryY, buildWordPosHistoryIndex);
        
		}
        // remove last character if not found
        deleteLastCharStarLetter(buildWord);
        // reset search history for last word
        bogglePieceObjectArray[buildWordPosHistoryX[buildWordPosHistoryIndex]][buildWordPosHistoryY[buildWordPosHistoryIndex]].setUsedHistory(false);
        buildWordPosHistoryIndex = buildWordPosHistoryIndex - 1;
        
        return;
    }
}// end searchSpot()


//--------------------------------------------------------------------------------
// Search the whole board, correspond with searchSpot()
void searchBoard(char **dictionary, bogglePiece bogglePieceObjectArray[6][6], wordFoundContainer wordFound[MaxWordsFoundOnBoard]){

 
    
    int buildWordPosHistoryIndex = -1;
    char* buildWord = new char[MaxWordLength + 1];
    int* buildWordPosHistoryX = new int[MaxWordLength + 1];
    int* buildWordPosHistoryY = new int[MaxWordLength + 1];
    // Search begin from 16 different spot
    for (int y = 1; y <= 4; y++){
        for (int x = 1; x <= 4; x++){
            
            resetUsedHistory(bogglePieceObjectArray);
            // Set the target search letter spot's usage to true
            bogglePieceObjectArray[x][y].setUsedHistory(false);
            searchSpot(dictionary, bogglePieceObjectArray, wordFound, x, y, buildWord, buildWordPosHistoryX, buildWordPosHistoryY, buildWordPosHistoryIndex);
            
            buildWord[0] = '\0';
            buildWordPosHistoryIndex = -1;
            resetUsedHistory(bogglePieceObjectArray);
        }
    }
    // Remove buildWord Prevent later memory allocate conflict
    
} //searchBoard()


//--------------------------------------------------------------------------------
// Search if the word is on the board
bool ifWordIsOnTheBoard(char* searchWord, wordFoundContainer wordFound[MaxWordsFoundOnBoard]){
	
    for (int i = 0; i < wordFoundContainer::numOfWordFound; i++){
    	if (strcmp(wordFound[i].getWord(), searchWord) == 0){
        	return true;
		}
    }
    return false;
}// end ifWordIsOnTheBoard()


//--------------------------------------------------------------------------------
// Display all the possible words
void displayAnswer(wordFoundContainer wordFound[MaxWordsFoundOnBoard], int min, int max){
	
	// Remove duplicate word
	for (int i = 0; i < wordFoundContainer::numOfWordFound; i++){
	    for (int j = 0; j < wordFoundContainer::numOfWordFound; j++){
	    	if (i != j && strcmp(wordFound[i].getWord(), wordFound[j].getWord()) == 0){
				wordFound[j].resetWord();
			}
	    }
	}
	
	cout << "Words between " << min << " and " << max << " are: " << endl;
	// show final result
	for (int i = 0; i < wordFoundContainer::numOfWordFound; i++){
		if (strlen(wordFound[i].getWord()) > 0 && strlen(wordFound[i].getWord()) > min && strlen(wordFound[i].getWord()) < max){
	    	cout << wordFound[i].getWord() << " ";
		}
	}
	cout << endl;
}// end displayAnswer


//---------------------------------------------------------------------------
int main()
{
    // declare variables
    char **dictionary;                // 2d array of dictionary words, dynamically allocated
    long int numberOfWords;           // how many words actually found in dictionary
    char userInput[ MaxUserInputLength];
    bogglePiece bogglePieceObjectArray[6][6]; // array of Object
    wordFoundContainer wordFound[MaxWordsFoundOnBoard]; // array of wordFoundContainer type for the wordFound
    int totalPointReceived = 0;
    int pointReceivedForThisRound;
    
    // Declare a variable to hold a time, and get the current time
    time_t startTime = time( NULL);

    displayIdentifyingInformation();
    displayInstructions();
    
    // read in dictionary.  numberOfWords returns the actual number of words found
    readInDictionary( dictionary, numberOfWords);
    
    initializeBoard(bogglePieceObjectArray, wordFound);
    searchBoard(dictionary, bogglePieceObjectArray, wordFound);
    cout << endl; 
    
    int elapsedSeconds = 0;
    
    while( elapsedSeconds < TotalSecondsToPlay) {
    	cout << "  " << TotalSecondsToPlay - elapsedSeconds << "seconds remaining" << endl;
    	displayBoard(bogglePieceObjectArray); 
    	cout << "   Score: " << totalPointReceived << endl;
        // Prompt for and get user input
        cout << "Enter a word: ";
        cin.getline(userInput, MaxUserInputLength-1);

        // UserInput Control
        if (strlen(userInput) == 1){
        	// Exit
	        if (userInput[0] == 'x'){
	        	return 0;
	        
	        // Custom Board
			}else if (userInput[0] == 'r'){
				cout << "Enter 16 characters to be used to set the board: ";
	            cin.getline(userInput, MaxUserInputLength-1);
	        	
		        initializeBoard(bogglePieceObjectArray, wordFound, true, userInput);
		        searchBoard(dictionary, bogglePieceObjectArray, wordFound);
		        cout << endl;
		        continue;
			}
			
			// Display possible word
			else if (userInput[0] == 's'){
				cout << "Enter min and max word lengths to display: ";
				cin.getline(userInput, MaxUserInputLength-1);
				displayAnswer(wordFound, userInput[0]-'0', userInput[2]-'0');
				cout << endl;
		        return 0;
			}
		}
        
        // Check for word in dictionary
        if( binarySearch( userInput, dictionary) == false) {
            cout << userInput << " was not found in the dictionary." << endl;
        }
        
        // Check for word in wordFound list
		else if (ifWordIsOnTheBoard(userInput, wordFound)){
			if (strlen(userInput) < 3){
				pointReceivedForThisRound = 0;
			}else if (strlen(userInput) == 3){					
				pointReceivedForThisRound = 1;
			}else if (strlen(userInput) == 4){					
				pointReceivedForThisRound = 2;
			}else if (strlen(userInput) == 5){					
				pointReceivedForThisRound = 4;
			}else if (strlen(userInput) >= 6){
				pointReceivedForThisRound = strlen(userInput);
			}
			cout << "   Worth "<< pointReceivedForThisRound << " points." << endl;
			totalPointReceived += pointReceivedForThisRound;
		}
		// Output for if not found in wordFound list
		else{
			cout << " cannot be formed on this board." << endl;
		}
        cout << endl;
        
        // Calculate how many seconds have elapsed since we started the timer.
        elapsedSeconds = difftime( time( NULL), startTime);
    }
    cout << "I let you finish your last move. Time is up!" << endl;
    
    
    return 0;   
}//end main()
