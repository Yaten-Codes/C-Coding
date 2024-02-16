/*
 * File: assign5.cpp
 * --------------
*/

#include <iostream>
#include "util.h"
#include <cctype>

using namespace std;

// Prototypes
string capitalize(string s);
string removeCharacters(string str, string remove);
string addCommas(string s);

// Each of the three functions below must return (NOT PRINT) 
// a string.

// Problem 1
string capitalize(string s)
{
    s[0] = toupper(s[0]) ;  
    for (int i= 1; i < s.length(); i++)
        s[i] = tolower(s[i]);

    return s;
}

// Problem 2
string removeCharacters(string str, string remove)
{

for (int j = 0; j < remove.length(); j++) {
    int pos = 0;
    while (( pos = str.find( remove[j], pos )) != string::npos )
    { 
        //if found erase from string once
        str.erase(pos,1 );
    } 
    }    
    return str;

} 

// Problem 3
string addCommas(string s)
{
int pos = 0; //position checker
string newstr = "";
int strlength = s.length();
while ( strlength > 0) {  //runs until length is 0
if ((((s.length() - pos) % 3) == 1) && strlength > 3) {  //if length is greater than 3; length-pos for checking
newstr = newstr + s.substr(pos,1) + ",";  // add
}
else{   //once/ or if less than 3 digits
newstr = newstr+s.substr(pos,1); //

}
pos++;
--strlength;  }
return newstr;

}

int main()
{
    cout << capitalize("viva las Vegas!") << endl;
    cout << capitalize("viVA la") << endl;
    cout << removeCharacters("counterrevolutionaries", "aeiou") << endl;  
    cout << removeCharacters("counterrevolutionaries**", "aeiou*") << endl;
    cout << removeCharacters("*hello* world", "l*") << endl;


while ( true ) {
string digits = getLine (" Enter a number : ") ;
if ( digits == "")
break ;
cout << addCommas ( digits ) << endl ;
}

return 0;

}
