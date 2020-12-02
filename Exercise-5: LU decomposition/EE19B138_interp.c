/* ***   Downsampling and Interpolation ***

This program code reads a file, gets the input and output values, downsamples it by a user-specified factor, interpolates over the downsampled points using
a linear spline and expands it back to the original, non-interpolated datapoints. Finally, it calculates the mean squared error between the original output
and the interpolated output values. 

Created by: V. Ruban Vishnu Pandian EE19B138
Created on: 18th September, 2020

Command input example: ./a.out <Downsampling constant> <File path> (Used after the linux command gcc EE19B138_interp.c) (or)
                       ./<Filename> <Downsampling constant> <File path> (<Filename> is the name used in linux command "gcc EE19B138_interp.c -o <Filename>")
*/

#include <stdlib.h>
#include <stdio.h>  // Importing the libraries needed for the code

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

        if(rows>=g)
        {
            int introws;                // Stores the number of downsampled points (rows/g)
            if(rows%g == 0)
            {
                introws = rows/g;       
            }
            else
            {
                introws = (rows/g)+1;  // If 'rows' isn't a multiple of 'g', this is done to get an extra downsampling point
            }
    
            int nr = -1;
            double element, *x, *y, *intpol;                   // Dynamic allocation of arrays to store input, original output and interpolated output
            x = (double*) calloc(rows, sizeof(double));
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
    
            int i = 1;
            double error = 0;          // Error storing variable

            for(int j=0; j<introws-1; j++)
            {
                int a = j*g;           // 'j'th downsampled point
                int b = a+g;           // 'j+1'th downsampled point
                while(i<b)
                {
                    double slope = (y[b]-y[a])/(x[b]-x[a]);         // Formula for slope connecting the 'j'th and 'j+1'th downsampled points
                    intpol[i] = slope*(x[i] - x[a]) + y[a];         // Formula for finding interpolated value for all the points in between
                    error = error + (intpol[i]-y[i])*(intpol[i]-y[i]);  // Error is updated
                    i++;
                }
                i++;
            }

            while(i<rows-1)     // This while loop is run to interpolate points between 'introws*g'th point and 'rows-1'th point
            {
                double slope = (y[rows-1]-y[introws*g - g])/(x[rows-1]-x[introws*g - g]);
                intpol[i] = slope*(x[i] - x[introws*g - g]) + y[introws*g - g];
                error = error + (intpol[i]-y[i])*(intpol[i]-y[i]);                 // Same formulae as used above
                i++;
            }  

            printf("%i %lf\n", g, error/rows);      // Downsampling constant and error are printed

            free(x);
            free(y);
            free(intpol);    // Freeing the dynamic arrays
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