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

    const double pi = M_PI;
    const int Naddr = 12;           // width of rom address.
    const int Ncoarse = 10; 
    const int Nfine = Naddr-Ncoarse;
    const int Dcoarse = 1<<Ncoarse;
    const int Dfine   = 1<<Nfine;
    const int D       = Dcoarse*Dfine;
    const int B = 10;               // width of rom output data.
    //const double A = pow(2.0, (B-1))-1.0; // Amplitude of the sine.
    const int Bsin = 16;            // number of bits in coarse sine rom.
    const int Bcos = 8;             // number of bits in coarse cosine rom.
    //const double Asin = pow(2.0, (Bsin-1))-1.0;


    // Make a some reference signals.
    std::vector<double> pure_sin(D+1);  for(int i=0; i<D+1; i++) pure_sin[i]  = sin((i+0.5)*pi/(2.0*D));

    // unquantized coarse tables
    std::vector<double> coarse_sin(Dcoarse+1); for(int i=0; i<Dcoarse+1; i++) coarse_sin[i] = pure_sin[i*Dfine];
    std::vector<double> coarse_cos(Dcoarse);   for(int i=0; i<Dcoarse; i++)   coarse_cos[i] = (coarse_sin[i+1] - coarse_sin[i])/Dfine;

    //int bits_for_cos = ceil(log2(coarse_cos[0]));
    //double cos_scale = pow(2.0, (Bcos-bits_for_cos));
    //for(int i=0; i<Dcoarse; i++) coarse_cos[i] *= cos_scale;
    //std::cout << "bits_for_cos = " << bits_for_cos << "\n";


    std::vector<double> mul_sine(D);
    for(int i=0; i<Dcoarse; i++) {
        for(int j=0; j<Dfine; j++) {
            mul_sine[i*Dfine+j] = j*coarse_cos[i] + coarse_sin[i];
        }
    }

    std::vector<double> mul_sine_norm(D); for(int i=0; i<D; i++) mul_sine_norm[i] = mul_sine[i];
    std::vector<double> error(D);         for(int i=0; i<D; i++) error[i] = mul_sine_norm[i] - pure_sin[i];
    double max_error = 0.0; for(int i=0; i<D; i++) if (fabs(error[i])>fabs(max_error)) max_error = error[i];
    std::cout << "max_error = " << max_error << "\n";


    FILE* gnuplotPipe = popen ("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "plot '-' ps 0.4 lt 6\n");
    //for (int i = 0; i < D/1; i++) fprintf(gnuplotPipe, "%lf\n", mul_sine_norm[i]);
    for (int i = 0; i < D/1; i++) fprintf(gnuplotPipe, "%lf\n", error[i]);
    //for (int i=0; i<Dcoarse+1; i++) fprintf(gnuplotPipe, "%lf\n", coarse_sin[i]);
    fprintf(gnuplotPipe, "e");
    pclose(gnuplotPipe);


    std::cout << "Dcoarse = " << Dcoarse << ", Dfine = " << Dfine << ", D = " << D << "\n";


}

