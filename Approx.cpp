#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;


int main(int argc, char const *argv[]) {

int numBits = 0;
double Vmax = 0;
double Vmin = 0;
double eVal = 0;
int degrees = 0;
double average = 0;

int a[8];


cout << "Enter number of bits: " << endl;
cin >> numBits;
cout << "Enter Vmax: " << endl;
cin >> Vmax;
cout << "Enter Vmin: " << endl;
cin >> Vmin;
cout << "Enter degrees: " << endl;
cin >> degrees;

eVal = degrees*(1.0/10);

cout << endl;

cout << "Vmax" << "    " << "Vmin" << "   ";
cout << "Avg" << "     " << " e" << endl;
cout << "----------------------------" << endl;


int size = 7;

for (int i = 0; i < 8; i++) {
average = (Vmax + Vmin)/2;

if (average > eVal) {
  cout << setprecision(3)
  << left << setw(size) << Vmax << setw(size)  << Vmin
    << setw(size) << average << ">  " << eVal << endl;
  Vmax = average;

  a[i] = 0;
}
else{
  cout << setprecision(3)
  << left << setw(size) << Vmax << setw(size)  << Vmin
  << setw(size) << average << "<  " << eVal << endl;
  Vmin = average;

  a[i] = 1;
}

}
cout << endl;

for (int j = 0; j < numBits; j++) {
  cout << a[j] << " ";
}


cout << endl;
cout << endl;
  return 0;
}
