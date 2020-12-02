/*
***   Factorial program  ***
This program code computes the factorial of a natural number.

Created by: V. Ruban Vishnu Pandian EE19B138
Created on: 11th August, 2020

Command input example: ./a.out <input_natural_number> (Used after the linux command gcc factorial.c) (or)
                       ./<Filename> <input_natural_number> (<Filename> is the name used in linux command "gcc factorial.c -o <Filename>")
Input example: 5
Output answer: 120(for input example 5)
*/


#include <stdio.h>      //Including the libraries needed for the code
#include <stdlib.h>


int main(int argc, char **argv){              // Getting the command line arguments and the opening of main function

    if(argc !=2){                             // Checks whether the number of arguments is precisely 2
        printf("Number of arguments read is not 2. We exactly need two elements. Please re-enter a valid command."); //Prints the message that number of arguments is not equal to 2(if it's not)
    }
    else{

        float p = atof(argv[1]);                 // Converts the second argument(the entered input number) from string to float variable
        int q = (int)p;                          // Assigns the integer part of the number obtained to a new variable
        
        if ((p-(float)q==0.0) && (p>0) && (p<=20)){         // Checks whether the number entered is a natural number(less than 20) or not

           unsigned long long fact = 1;    // Initializing a variable used to store the factorial value

           for(int i=1;i<=p;i++){          // Loop to calculate and store the value of the factorial iteratively until all natural numbers below the entered number are multiplied
               fact*=i;                    // Calculates and stores the value of factorial obtained in the 'i'th iteration
           }
        printf("The factorial of %i (%i!) is %llu.",q,q,fact); // Prints the factorial value after the loop terminates,i.e.,all numbers below the entered number are multiplied
        }
        
        else if(p>20){                    // Executed when the entered argument is a number which is more than 20
        
        printf("Factorial can be computed but cannot be displayed fully on the screen. Please re-enter a natural number less than 20."); 
        // Because of terminal screen's capabilities, the complete factorial value can be displayed, only for numbers less than(or equal to) 20. That is why the conditional statement present above is used. 
        }
        
        else{    // Executed when the argument passed is a number like 0, -5 or when it is a string like three, four
            printf("Factorial cannot be computed for the entered argument. Please re-enter a valid(natutral) number.");// If the entered argument is not a natural number, this message is printed
        }
    }
printf("\n"); // For making the terminal cursor to advance by one line(for aesthetic purposes only)
return 0; // The return statement of the main function
}
