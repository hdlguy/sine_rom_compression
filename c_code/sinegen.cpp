// This program computes the parameters of a quarter cycle multiplying sine rom for a given set of parameters.
// Several tricks are used to make this work. The first is a shift of half a sample in order to produce 
// a quarter rom that can be combined four times to make a full cycle.
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>

int main()
{
    printf("computing sine and cosine tables\n");

    const double pi = M_PI;  //4.0*atan(1.0);
    const int Naddr = 12;            // width of rom address.
    const int Nfine = 4;             // width of fine part of address.
    const int Ncoarse = Naddr-Nfine; // width of coarse part of address.
    const int Dcoarse = 1<<Ncoarse;
    const int Dfine   = 1<<Nfine;
    const int Dtotal  = Dcoarse*Dfine;

    const int B = 10; // width of rom output data.
    const double A = pow(2.0, (B-1))-1.0; // Amplitude of the sine.

    std::vector<double> pure_sine(Dtotal);
    for(int i=0; i<Dtotal; i++) pure_sine[i] = A*sin((i+0.5)*pi/(2.0*Dtotal));

    std::vector<double> quant_sine(Dtotal);
    for(int i=0; i<Dtotal; i++) quant_sine[i] = round(A*sin((i+0.5)*pi/(2.0*Dtotal)));
    //for(int i=0; i<Dtotal; i++) std::cout << quant_sine[i] << "\n";

    std::vector<double> coarse_sine(Dcoarse);
    for(int i=0; i<Dcoarse; i++) coarse_sine[i] = round(A*sin((i+0.5)*pi/(2.0*Dcoarse)));
    for(int i=0; i<Dcoarse; i++) std::cout << coarse_sine[i] << "\n";

    std::cout << "Dcoarse = " << Dcoarse << ", Dfine = " << Dfine << ", Dtotal = " << Dtotal << "\n";

}

