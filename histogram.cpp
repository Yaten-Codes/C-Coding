
 #include <iostream>
 #include "util.h"
 #include <fstream>
 #include <vector>
 using namespace std;
 

void histogram() {
ifstream f;
while (!f.is_open()) {
  string filename = getLine("filename: ");
  f.open(filename);
    }
vector<int> scores;
int number;
while (f >> number) {       
   scores.push_back(number);
}
int nums[10] = { 0 };
for( int x: scores) {
    if (x >= 0 && x <= 9)
    {
        nums[0]++;
    }
    else if (x >= 10 && x <= 19)
    {
        nums[1]++;
    }
    else if (x >= 20 && x <= 29)
    {
        nums[2]++;
    }
    else if (x >= 30 && x <= 39)
    {
        nums[3]++;
    }
    else if (x >= 40 && x <= 49)
    {
        nums[4]++;
    }
    else if (x >= 50 && x <= 59)
    {
        nums[5]++;
    }
    else if (x >= 60 && x <= 69)
    {
        nums[6]++;
    }
    else if (x >= 70 && x <= 79)
    {
        nums[7]++;
    }
    else if (x >= 80 && x <= 89)
    {
        nums[8]++;
    }
    else
    {
        nums[9]++;
    }
}

    cout << "0-9: ";
    for (int i = 0; i < nums[0]; i++) { cout << "X"; }
    cout << endl;

    cout << "10-19: ";
    for (int i = 0; i < nums[1]; i++) { cout << "X"; }
    cout << endl;

    cout << "20-29: ";
    for (int i = 0; i < nums[2]; i++) { cout << "X"; }
    cout << endl;

    cout << "30-39: ";
    for (int i = 0; i < nums[3]; i++) { cout << "X"; }
    cout << endl;

    cout << "40-49: ";
    for (int i = 0; i < nums[4]; i++) { cout << "X"; }
    cout << endl;

    cout << "50-59: ";
    for (int i = 0; i < nums[5]; i++) { cout << "X"; }
    cout << endl;

    cout << "60-69: ";
    for (int i = 0; i < nums[6]; i++) { cout << "X"; }
    cout << endl;

    cout << "70-79: ";
    for (int i = 0; i < nums[7]; i++) { cout << "X"; }
    cout << endl;

    cout << "80-89: ";
    for (int i = 0; i < nums[8]; i++) { cout << "X"; }
    cout << endl;

    cout << "90-99: ";
    for (int i = 0; i < nums[9]; i++) { cout << "X"; }
    cout << endl;
    f.close();

}

   // TODO: Write a correct version of this function. My solution uses a couple of
   // helper functions that are called by this function. 
   //cout << v <<endl; //"histogram() not yet implemented" << endl;





