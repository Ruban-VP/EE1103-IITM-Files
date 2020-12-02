/*     ***    Using Correlation to find phase difference   ***

This program code reads a given dataset, considers its first column as the discrete time variable, second and third columns as discrete time signals
of a given frequency(with the assumption that they are zero-padded outside the given range of time values), generates the discrete fourier transform
data of the first signal or the second signal or generates the correlation data (Depending on the user's choice). The generated data is later 
plotted using Gnuplot using the companion bash script and hence, the common frequency and phase difference are calculated.

Created by: V. Ruban Vishnu Pandian EE19B138
Created on: 22nd November, 2020

Command usage: ./a.out <Datafilepath> <Choice> (Used after the linux command "gcc correlation.c -lm") (or)
               ./<Filename> <Datafilepath> <Choice> (Used after the linux command "gcc correlation.c -lm -o <Filename>")
*/

#include<stdlib.h>
#include<stdio.h>
#include<math.h> //Including the libraries needed for the code

int main(int argc, char **argv) //Opening of the main function and getting the command line inputs
{
    if(argc!=3) //If the number of arguments is not 3(which we don't want), the error message present below is printed and the program terminates
    {
        printf("\nThe number of arguments must be exactly 3. Please re-enter a valid command.\n\n");
        exit(0);
    }

    float temp = atof(argv[2]);
    int choice = atoi(argv[2]);

    if(choice<1 || choice>3 || (temp-choice)!=0) //If the choice entered is not a valid choice, 
                                                 //the error message present below is printed and the program terminates
    {
        printf("\nCommand usage: %s %s %s\n", argv[0], argv[1], argv[2]);
        printf("Seems like the choice entered is not a positive integer between 1 and 3.\nPlease re-enter a valid choice.\n\n");
        exit(0);
    }

    int rows = 0;               // The variable storing the number of rows is made zero assuming the file doesn't contain
                                // any field names (Actually the file is first filtered using grep and awk commands present in the bash script)
    char str[50];
    FILE *fp;
    fp = fopen(argv[1], "r");   // File is opened in 'Read' mode
    while(!feof(fp))
    {
        fscanf(fp, "%s", str);
        if(fgetc(fp)=='\n')
        { 
            rows++;             // Total number of rows present in the file is calculated     
        }
    }
    fclose(fp);                 // File closing

    int cols=0, nr = 0;
    double element, *t, *x, *y;   
    t = (double*) calloc(rows, sizeof(double));   
    x = (double*) calloc(rows, sizeof(double));
    y = (double*) calloc(rows, sizeof(double));    // Dynamic memory allocated for the time values and signal values
    fp = fopen(argv[1],"r");             // File is again opened in 'Read' mode
    while(nr!=rows)
    {
        fscanf(fp, "%s", str);           // File content is scanned
        sscanf(str, "%lf", &element);    // String to float conversion

        if(cols==0)     
        {
            t[nr] = element;          // Time values are stored 
        }
        else if(cols==1)
        {
            x[nr] = element;          // Signal 1 values are stored
        }
        else if(cols==2)
        {
            y[nr] = element;          // Signal 2 values are stored
        }
        cols++;                       //Cols is incremented  
        
        if(fgetc(fp)=='\n')              
        {
            nr++;              // When a newline is encountered, 'nr' is incremented and 'cols' is re-initialised to zero
            cols = 0;
        }
    } 
    fclose(fp);                // Closing of file

    double Tsample = t[1]-t[0];                                // The sampling period is stored(Assuming the values are sampled at equal intervals)
    double xdftreal=0, xdftcomp=0, ydftreal=0, ydftcomp=0;     // Variables initialised to store the real and imaginary parts of the fourier transforms

    if(choice==1 || choice==2)           // Only if choice is 1 or 2 (Printing the DFT values), the FOR loop is executed 
    {
        for(int i=1; i<rows; i++)
        {
            double freq = 1/(i*Tsample);     //The DFT for corresponding frequency will be calculated
            
            for(int j=0; j<rows; j++)
            {
                if(choice==1)
                {
                    xdftreal += x[j]*cos(2*M_PI*j/i);
                    xdftcomp += x[j]*sin(2*M_PI*j/i);     //If choice=1, the DFT of first signal is computed
                }
                else if(choice==2)
                {
                    ydftreal += y[j]*cos(2*M_PI*j/i);
                    ydftcomp += y[j]*sin(2*M_PI*j/i);     //If choice=2, the DFT of second signal is computed
                }
            }
        
            if(choice==1)
            {
                double xdftmag = sqrt(pow(xdftreal,2)+pow(xdftcomp,2));
                printf("%le %le\n", freq, xdftmag);                      //The frequency and DFT magnitude (of signal 1) are printed
                xdftreal = 0;
                xdftcomp = 0; //The DFT variables are cleared for next frequency
            }
            else if(choice==2)
            {
                double ydftmag = sqrt(pow(ydftreal,2)+pow(ydftcomp,2));
                printf("%le %le\n", freq, ydftmag);                     //The frequency and DFT magnitude (of signal 2) are printed
                ydftreal = 0;
                ydftcomp = 0; //The DFT variables are cleared for next frequency
            }
        }
    }
    // The resource for Discrete time fourier transform (DFT) is given in this link: http://www.fit.vutbr.cz/~ihubeika/ISS/lect/diskretni_en.pdf
    // (Refer page no. 28: DFT for sampled discrete time signal) 

    else if(choice==3)        // If choice=3 (Finding the correlation data), this FOR loop is executed
    {
        double corr=0;                  //Variable initialised with zero which is used to store the correlation value
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<rows-i; j++)
            {
                corr += (x[j]*y[j+i]*Tsample);   //Correlation variable is updated
            }
            corr = corr/((rows-i)*Tsample);           /*Correlation value is divided by the number of data points used to generate it
                                                      (Number of data points decrease as we increase the delay parameter because of zero padding)*/
            printf("%d %le\n", i, corr);    //The delay parameter and the corresponding correlation value are printed
            corr=0;                         //Correlation variable cleared for next delay parameter
        }
    }

    free(t);
    free(x);
    free(y);                    //The dynamic arrays are freed
    return 0;                   //The return statement of the main function
}