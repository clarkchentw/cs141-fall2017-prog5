#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <cctype>     // Allows using the tolower() function
#include <ctime>      // Timer functions
#include <cstring>    // For strlen
using namespace std;



int main()
{
    char one[] = "Clark1";
    char two[] = "Clark";
    cout << strcmp(one, two);

    
    return 0;
}