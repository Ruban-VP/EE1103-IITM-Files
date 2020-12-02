/* ***   Downsampling and Interpolation using Cubic spline with LU decomposition (Input, Output and Interpolated output printer) ***

This program code reads a file, gets the input and output values, downsamples it by a user-specified factor, interpolates over the downsampled points using
a cubic spline (assisted by LU decomposition) and expands it back to the original, non-interpolated datapoints. Finally it prints the input, output and
interpolated output values.

Created by: V. Ruban Vishnu Pandian EE19B138
Created on: 30th September, 2020

Command input example: ./a.out <Downsampling constant> <File path> (Used after the linux command gcc LUD_Cubspline_Inoutinterpol.c -lm) (or)
        ./<Filename> <Downsampling constant> <File path> (<Filename> is the name used in linux command "gcc LUD_Cubspline_Inoutinterpol.c -lm -o <Filename>")
*/

#include <stdlib.h>
#include <stdio.h>  
#include <string.h>
#include <math.h>    // Importing the libraries needed for the code

int main(int argc, char** argv)  // Opening of the main function and getting the command line arguments
{
    int g = atoi(argv[1]);
    double f = atof(argv[1]);
    if(argc!=3)
    {
        printf("Number of arguments must be exactly 3. Please re-enter a valid command\n\n");  // This error message is printed if command line arguments
                                                                                               // are not exactly 3 in number
    }
    else if(f==g && g>0)      // Checks if the downsampling constant entered is a positive integer
    {
        int cols = 0, rows = -1;
        char str[50];
    
        FILE *fp;
        fp = fopen(argv[2], "r");   // File is opened in 'Read' mode
        while(!feof(fp))
        {
            fscanf(fp, "%s", str);
            if(fgetc(fp)=='\n')
            {
                rows++;             // Whenever the file pointer encounters a newline, the variable 'rows' is incremented. Hence it stores the number of rows.
                                    // 'rows' is initialised with '-1' so when it completes the first line (which is usually field names), it becomes zero.
                                    // Hence, it stores the correct number of rows. Also, I assumed the last line ends with a newline character.
            }
        }
        fclose(fp);                 // File closing

        // ********** Data abstraction segment ***********

        if(rows>=g)
        {
            int nr = -1, p=0;
            double element, *x, *y, *intpol, *dsx, *dsy;     // Dynamic allocation of arrays to store input, original output, interpolated output,
            x = (double*) calloc(rows, sizeof(double));      // downsampled 'x' values and downsampled 'y' values
            y = (double*) calloc(rows, sizeof(double));
            intpol = (double*) calloc(rows, sizeof(double));

            fp = fopen(argv[2],"r");             // File is again opened in 'Read' mode
            while(nr!=rows)
            {
                fscanf(fp, "%s", str);           // File content is scanned
                sscanf(str, "%lf", &element);    // String to float conversion

                if(nr!=-1)                        // First row is ignored because it stores only the field names
                {
                    if(cols==0)     
                    {
                        x[nr] = element;         // Whenever 'cols' is zero, the input value of that row is scanned. So this condition is checked and later 
                                                 // only we store the file content in the input array (x)
                        if(nr%g == 0 || nr == rows-1)
                        {
                            p++;                 // A variable 'p' is updated which stores the total number of downsampled points
                        }
                    }
                    else
                    {
                        y[nr] = element;          // Output is stored in output array (y)
                        intpol[nr] = element;     // Interpolation array (intpol) is also initialised with the same output values for time being
                    }
                    cols++;
                }

                if(fgetc(fp)=='\n')              
                {
                    nr++;              // When a newline is encountered, 'nr' is incremented and 'cols' is re-initialised to zero
                    cols = 0;
                }
            } 
            fclose(fp);                // Closing of file

            dsx = (double* ) calloc(p, sizeof(double));  // Once we get 'p', we dynamically allocate memory for downsampled 'x' and downsampled 'y'
            dsy = (double* ) calloc(p, sizeof(double)); 

            int j=0;
            for(int i=0; i<rows; i++)
            {
                if(i%g == 0 || i == rows-1)
                {
                    dsx[j] = x[i];
                    dsy[j] = y[i];      // The downsampling arrays are updated using data from the original file
                    j++;
                }
            }

            
            // *********** Forming the matrix for LU decomposition based on Cubic spline *********

            double *k, *xdif, *ydif, *b, *out;
            k = (double* ) calloc(p, sizeof(double));             //Parameter set (or the K parameter vector)
            xdif = (double* ) calloc(p-1, sizeof(double));        
            ydif = (double* ) calloc(p-1, sizeof(double));
            b = (double* ) calloc(p-2, sizeof(double));
            out = (double* ) calloc(p-2, sizeof(double));         //The output parameter vector

            // Some arrays are dynamically allocated to store the different matrix elements. 
            // The Cubic spline math used in this code is based on the content written by Scholar Lois Anne Leal.
            // For further details, visit this link given below: 
            // "https://towardsdatascience.com/numerical-interpolation-natural-cubic-spline-52c1157b98ac"

            k[0] = 0;
            k[p-1] = 0;  //We have the liberty be chose the initial and final parameters and we have fixed them as zero
            for(int i=0; i<p-1; i++)
            {
                xdif[i] = dsx[i] - dsx[i+1];                  // Difference of consecutive 'x's
                ydif[i] = (dsy[i] - dsy[i+1])/xdif[i];        // Difference of consecutive 'y's divided by xdif
            }

            for(int i=0; i<p-2; i++)
            {
                b[i] = 2*(xdif[i] + xdif[i+1]);               // Twice the sum of two consecutive 'xdif's
            }

            for(int i=0; i<p-2; i++)
            {
                out[i] = 6*(ydif[i] - ydif[i+1]);             // Six times the difference of two consecutive 'ydif's
            }

            double par[p-2][p-2];                             // The augmented matrix which transforms K vector as (par)*K = out
            for(int i=0; i<p-2; i++)
            {
                for(int j=0; j<p-2; j++)
                {
                    par[i][j] = 0;                            // Initialised to zero matrix
                }
            }

            for(int i=0; i<p-2; i++)
            {
                if(i==p-3)
                {
                    par[i][i] = b[i];                          // Diagonal elements are 'b's
                }
                else
                {
                    par[i][i] = b[i];                          // Diagonal elements are 'b's
                    par[i][i+1] = xdif[i+1];                   // The entity of the matrix which is immediately right to or down the diagonal
                                                               // element indexed by 'i' is xdif[i] 
                    par[i+1][i] = xdif[i+1];
                }     
            }

            // ********** LU decomposition **********

            double upper[p-2][p-2], lower[p-2][p-2];         
            memset(lower, 0, sizeof(lower));
            memset(upper, 0, sizeof(upper));       // Lower and upper triangular matrices initialised to zero matrix

            // The LU decomposition code written below is based on 
            // a "GeeksforGeeks" article named "Doolittle algorithm" by Shubham Rana
            // For further details, visit the link given below:
            // "https://www.geeksforgeeks.org/doolittle-algorithm-lu-decomposition/"

            for(int i=0; i<p-2; i++)
            {
                for(int l=0; l<p-2; l++)
                {
                    double sum = 0;
                    for(int j=0; j<i; j++)
                    {
                        sum += (lower[i][j]*upper[j][l]);
                    }
                    upper[i][l] = par[i][l] - sum;          // Upper triangular matrix finding
                }

                for(int l=0; l<p-2; l++)
                {
                    if(i==l)
                    {
                        lower[i][i] = 1;
                    }
                    else
                    {
                        double sum = 0;
                        for(int j=0; j<i; j++)
                        {
                            sum += (lower[l][j]*upper[j][i]);
                        }
                        lower[l][i] = (par[l][i] - sum)/upper[i][i];    // Lower triangular matrix finding
                    }
                }
            }

            double z[p-2]; 
            // Once we have the augmented matrix par as par = LU, we can say L*U*k = out
            // This means U*k = inv(L)*out
            // Let inv(L)*out = z. Hene, L*z = out. We can solve for 'z' using forward substitution which is done below

            z[0] = out[0];
            for(int i=1; i<p-2; i++)
            {
                double sum = 0;
                for(int j=0; j<i; j++)
                {
                    sum += (lower[i][j]*z[j]);
                }
                z[i] = out[i] - sum;            //Implementing the forward substitution algorithm to find 'z'
            }

            //After finding 'z' vector, we have U*k = z, which we can find using back substitution
            k[p-2] = z[p-3]/upper[p-3][p-3];
            for(int i=p-4; i>=0; i--)
            {
                double sum = 0;
                for(int j=i+1; j<p-2; j++)
                {
                    sum += (upper[i][j]*k[j+1]);
                }
                k[i+1] = (z[i] - sum)/upper[i][i]; //Implementing the backward substitution algorithm to find 'k'
            }

            // ******** Finding the interpolated outputs (or 'y's) ********
            
            int ind=0;                   //An index variable to run through the downsampled values, i.e., through arrays dsx,dsy and k
            for(int i=1; i<rows; i++)
            {
                int a1 = (ind+1)*g;
                int a2 = ind*g;          //Indices to find the downsampled 'x's between which we are currently at 

                if(a1>rows-1)
                {
                    a1 = rows-1;         //If the number of rows(rows) is not a perfect multiple of the downsampling constant(g),
                                         //this if statement will be useful. 
                }

                //Some useful variables are declared, which helps us to write the interpolated output expression in a clean and precise manner
                //The variables are declared according to the formulae present in the Cubic spline article. The link is already put above

                double txdif1 = x[i] - x[a1];
                double txdif2 = x[i] - x[a2];            

                double term1 = (pow(txdif1,3) - txdif1*pow(xdif[ind], 2))/xdif[ind];
                double term2 = (pow(txdif2,3) - txdif2*pow(xdif[ind], 2))/xdif[ind];
                double term3 = (y[a2]*txdif1 - y[a1]*txdif2)/xdif[ind];


                if(i<a1)
                {
                    intpol[i] =  (k[ind]*term1/6) - (k[ind+1]*term2/6) + term3; // Interpolated output is found
                }
                else
                {
                    ind++;  // Once we reach the end of an interval(say between dsx[2] and dsx[3] and now we are at dsx[3]),
                            // we go to the next interval by incrementing ind
                }
            }

            for(int i=0; i<rows; i++)
            {
                printf("%le %lf %lf\n", x[i], y[i], intpol[i]);   // Printing the input, original output and interpolated output values
            }
            
            free(x);
            free(y);
            free(intpol);
            free(dsx);
            free(dsy);
            free(k);
            free(xdif);
            free(ydif);
            free(b);
            free(out);      // Freeing the dynamic arrays
        }
        else
        {
            printf("\nNumber of rows: %i, Downsampling constant: %i\n", rows, g); // If the downsampling constant entered is more than the number of rows,
                                                                                // this message is printed
            printf("Seems like the downsampling constant is higher than the number of datapoints.\nPlease re-enter a valid command.\n\n");
        }
        
    }
    else
    {
        printf("\nCommand usage: %s %s %s\n", argv[0], argv[1], argv[2]);        // If the downsampling constant is not a positive
                                                                               // this message is printed
        printf("Seems like the downsampling constant is not a positive, non-zero interger.\nPlease re-enter a valid command.\n\n");
    }
    return 0;            // Return statement of the main function
} 

