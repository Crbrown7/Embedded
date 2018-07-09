#include <iostream>
#include <cmath>
#include <math.h>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[]) {


int degreeMax = 0;
int degreeMin = 0;
int ADCmax = 0;
int ADCmin = 0;
int numBits = 0;
int degrees = 0;
double e = 0;
double digitalVal = 0;



std::cout << "Enter number of bits: " << '\n';
std::cin >> numBits;

std::cout << "Enter Max degrees: " << '\n';
std::cin >> degreeMax;

std::cout << "Enter Min Degrees" << '\n';
std::cin >> degreeMin;

std::cout << "Enter ADC Max: " << '\n';
std::cin >> ADCmax;

std::cout << "Enter ADC Min: " << '\n';
std::cin >> ADCmin;

std::cout << "Enter degrees: " << '\n';
std::cin >> degrees;

e = degrees * (1.0/10);


double a = (e - ADCmin) * pow(2, numBits);
double b =  ADCmax - ADCmin;

digitalVal = a/b;

cout<< endl;

int upper = ceil(digitalVal);
int lower = floor(digitalVal);

std::cout << lower  << " or " << upper << '\n';
std::cout << hex << lower << " or " << upper << '\n';

std::cout << "Res of ADC: " << (ADCmax - ADCmin)/pow(2,numBits) << '\n';

std::cout << "Res of DAS: " << (degreeMax - degreeMin)/ pow(2,numBits) << '\n';





  return 0;
}
