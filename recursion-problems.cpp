/*
 * File: recursion-problems.cpp
 * --------------
 * Name: [Alexis Carreto]
 *
 * Assignment 3 - Recursion Problems
 *
 * Write all your code in this file!
 */

#include <iostream>
#include "tigr.h"
#include "util.h"
#include <vector>

using namespace std;

// globals
const TPixel black = tigrRGB(0, 0, 0);
const TPixel white = tigrRGB(255, 255, 255);
Tigr *window;

// function prototypes
string removeVowels(string s);
void drawRulerWrapper(double x, double y, double w, double h);
void drawRuler(double x, double y, double w, double h);

// testing functions
void testRemoveVowels()
{
    cout << "Armadillo-->" << removeVowels("Armadillo") << endl;
    cout << "AEIOU-->" << removeVowels("AEIOU") << endl;
    cout << "aei ou-->" << removeVowels("aei ou") << endl;
    cout << "ntr wqsdfgh-->" << removeVowels("ntr wqsdfgh") << endl;
}

void testRuler()
{
    drawRulerWrapper(0, 0, 800, 400);
}

int main()
{
    string yn;
    yn = getLine("test disemvowel? ");
    if (yn == "y")
        testRemoveVowels();

    yn = getLine("test ruler? ");
    if (yn == "y")
        testRuler();
}

bool isVowel(char c)
{
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
    {

        return true;
    }

    return false;
}

string removeVowels(string s)
{
    if (s.empty())
    {
        return s;
    }
    if (isVowel(s[0]))
    { // if first s is vowel
        s.erase(s.begin());
        return removeVowels(s);
        return s;
    }
    else
    {
        return s[0] + removeVowels(s.substr(1)); // returns 1st s and continues to the rest
    }
    // TODO: Problem A
}

// Wrapper function; do not modify
void drawRulerWrapper(double x, double y, double w, double h)
{
    window = tigrWindow(w + 10, h + 10, "Ruler", TIGR_AUTO);
    tigrClear(window, white);
    tigrUpdate(window);
    drawRuler(x, y, w, h);
    while (!tigrClosed(window))
    {
        tigrUpdate(window);
    }
    tigrFree(window);
}

void drawRuler(double x, double y, double w, double h)
{
    tigrLine(window, 400, 0, 400, 400, black); //mid line
    tigrLine(window,(w - 10 ), h, (w - 10) , y + 350, black);
    tigrLine(window,(w - 20), h, (w - 20 ), y + 300, black);
    tigrLine(window,(w - 30 ), h, (w - 30) , y + 350, black);
    tigrLine(window,(w - 40) , h, (w - 40) , y + 200, black);
    tigrLine(window, 0, 400, 800, 400, black); // bottom line
    tigrLine(window, 0, 0, 0, 400, white);  // make white line to 
    //remove line made at those coords by recursion at w=0

    if (w != 0) 
    drawRuler( x, y, w-40, h);   
    //put w -(last w) to start from there if need to change width

    // TODO: Problem B

    // Use the global variable `window` in your calls to TIGR
    // drawing functions.
}
