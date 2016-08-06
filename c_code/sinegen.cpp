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

    const double pi = M_PI;          //4.0*atan(1.0);
    const int Naddr = 12;            // width of rom address.
    const int Nfine = 3;             // width of fine part of address.
    const int Ncoarse = Naddr-Nfine; // width of coarse part of address.
    const int Dcoarse = 1<<Ncoarse;
    const int Dfine   = 1<<Nfine;
    const int D       = Dcoarse*Dfine;
    const int B = 10;               // width of rom output data.
    const double A = pow(2.0, (B-1))-1.0; // Amplitude of the sine.
    const int Bsin = 16;            // number of bits in coarse sine rom.
    const int Bcos = 8;            // number of bits in coarse cosine rom.
    const double Asin = pow(2.0, (Bsin-1))-1.0; // Amplitude of the coarse sine.
    const double Acos  = pow(2.0, (Bcos-1))-1.0;  // Amplitude of the coarse cosine.

    std::vector<double> pure_sin(D);
    for(int i=0; i<D; i++) pure_sin[i] = A*sin((i+0.5)*pi/(2.0*D));

    std::vector<double> quant_sin(D);
    for(int i=0; i<D; i++) quant_sin[i] = A*sin((i+0.5)*pi/(2.0*D));
    //for(int i=0; i<D; i++) std::cout << quant_sin[i] << "\n";

    std::vector<double> coarse_sin(Dcoarse);
    for(int i=0; i<Dcoarse; i++) coarse_sin[i] = Asin*sin((i+0.5)*pi/(2.0*Dcoarse));

    std::vector<double> coarse_cos(Dcoarse);
    for(int i=0; i<Dcoarse; i++) coarse_cos[i] = round(Acos*cos((i+0.5)*pi/(2.0*Dcoarse)));

    std::vector<double> cos_prod(D);
    for(int i=0; i<Dcoarse; i++) {
        for(int j=0; j<Dfine; j++) {
            cos_prod[i*Dfine+j] = 0.79*coarse_cos[i]*j + 8*coarse_sin[i];
        }
    }


    FILE* gnuplotPipe = popen ("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "plot '-' \n");
    for (int i = 0; i < D; i++) fprintf(gnuplotPipe, "%lf\n", cos_prod[i]);
    fprintf(gnuplotPipe, "e");
    pclose(gnuplotPipe);


    std::cout << "Dcoarse = " << Dcoarse << ", Dfine = " << Dfine << ", D = " << D << "\n";

}

