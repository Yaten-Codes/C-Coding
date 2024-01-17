/**************************
*	COSC 220
***************************
*
*	Assignment 1, part 1: perfect numbers
*/

#include <iostream>

using namespace std;

// prototype for helper function
bool isPerfect(int n);

void perfectNumbers() {
    for(int n=1; n<10000; n++) 
    if(isPerfect(n))     //use helper func

    // Checks each number from 1 to 10000 and prints those that are perfect.

    // TODO: write a correct version of this function
    cout << n << " is a perfect numbers"<< endl;
}

bool isPerfect(int n) {   //def-perfect #s are equal to the sum of their proper divisor

    int sum = 0;
    for(int i=1; i<n; ++i){ 
        if(n%i ==0)      //if n/its sums of divisors = 0 add to sum
        sum += i;
    }
        if(sum==n){   
        return true;
        }
    else{
        return false;
        }
    // Returns true if n is perfect, false otherwise.
    // TODO: write a correct version of this function
}
