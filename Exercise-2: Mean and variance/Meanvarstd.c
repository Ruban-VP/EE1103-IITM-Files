/*
***   Mean and variance program  ***
This program code computes and displays the means, variances and standard deviations of all fields of a given dataset.

Created by: V. Ruban Vishnu Pandian EE19B138
Created on: 22th August, 2020

Command input example: ./a.out <Dataset file path> (Used after the linux command gcc Meanvarstd.c -lm) (or)
                       ./<Filename> <Dataset file path> (<Filename> is the name used in linux command "gcc Meanvarstd.c -o <Filename> -lm")

Note:
(1.): Usage of '-lm' at the end of the gcc command is important as that only allows the proper compilation of the math.h library
(2.): The Dataset file path must be free of any spaces as the main function classifies the argv inputs based on blank spaces as well. So if a folder containing the dataset has spaces in its name,  either create a copy of the dataset in a folder where its path doesn't contain any spaces (or) rename the folder by replacing the spaces with underscores.   
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>  // Including the libraries needed for the program

int main(int argc, char **argv){  // Opening of main function and getting the command line arguments

 if(argc != 2){    // Checks whether the number of command line arguments is not 2.
  printf("\nNumber of commands must be precisely 2, i.e, first command is either ./a.out or ./filename (Depending on the gcc command) and the next argument is the dataset \nfile's location. Please re-enter a valid command.\n\n");  // If number of arguments isn't 2, this message is printed so that the user is asked to re-enter a valid command.
 }
 else{     // Executed if the number of command line arguments is 2, which we want
 
  FILE *fp;                                               // Declaration of file pointer
  char str[50];                                           // Initialisation of a string variable used to store the file contents read at a time
  unsigned int rows=0, nr=0, cols=0, cols2;               // Initialisation of four variables:
                                                          // rows: To store the final number of rows or records (Including the first row which usually contains the field names)
                                                          // cols2: To store the final number of fields or columns
                                                          // nr: To store the number of rows read so far
                                                          // cols: To store the number of columns read so far
  fp = fopen(argv[1], "r");                               // Opening of the datafile in 'Read' mode
  printf("\n");                             // Advancing by a newline for aesthetic purposes so that the output starts appearing after a line and not immediately near the commands we have entered
  
  if(fp == NULL){    // The command line input may not open. Some reasons are the argument passed may not be a valid file path, etc. In those cases, this condition becomes true
  
   printf("\nError opening file.\nCommand usage: %s %s.\nPlease re-enter a valid argument which is the file path.\n\n", argv[0], argv[1]);
   // A print statement is used if it's unable to open the file. This message displayed above asks the user to re-enter a valid file path.
   
  }
  else{   // If the file is successfully opened, which we want, then this else part is executed
   
   while(!feof(fp)){  // A while loop to store the number of rows and columns
    
    fscanf(fp, "%s", str); // A fscanf statement to get the file contents classified by blank spaces
    cols++;                // Since each element (on the same row) belong to a different field, we increase the variable 'cols' by 1 
    if(fgetc(fp)=='\n'){   // When the file pointer reaches the end of a row, the fgetc statement reads a newline. This if condition becomes true at that particular time
     rows++;               // Since this if condition becomes true when we get to a row's end, we increase the number of rows read by one
     cols2=cols;           // We copy the number of columns present in the current row to a variable 'cols2'
     cols=0;               // We once again initialise cols to zero so that it can again be used to store the number of columns present in the next row
    }
   }
   
   /* By the end of the first while loop, the total number of rows is stored in variable 'rows'. Note that this number includes the first row (row containing field names as well). 
   But it really doesn't matter as we use this variable only to cross-check with another variable's value so that we can check whether the file pointer has reached the end
   (Becomes clear as we reach the next while loop). The variable cols has become zero. But before that, its value has been stored in another variable named 'cols2'.
   Particularly, cols2 contains the number of columns present in the last row. Since this is a dataset, all rows must contain same number of columns. Hence, cols2
   contains the correct number of columns we need.
   */
    
   fclose(fp);  // Closing of file as the file pointer has reached the end-of-file.
    
   double element;                                   // Variable to store the file contents in decimal form
   double *sum, *sumsq;                              // Declaration of pointers of dynamic arrays which are used to store the cumulative sum and cumulative sum of squares for individual columns
   sum = (double*) calloc(cols2, sizeof(double));    // Allocating heap memory for pointer variable 'sum'
   sumsq = (double*) calloc(cols2, sizeof(double));  // Allocating heap memory for pointer variable 'sumsq'
 
   if(sum==NULL || sumsq==NULL){                     // If there's no enough heap memory and hence no memory allocated for sum or sumsq, this condition becomes true
   
    printf("Heap memory not allocated for dynamic storage of values. Please free it if it's consumed in other programs. \n");
    // The above message is printed which asks the user to clear the dynamically allocated memory from other programs so that it can be used here
    
   }
   else{                            // If both sum and sumsq are alloted the specified heap memory, which we want, this else part is executed
    fp = fopen(argv[1], "r");       // File is again opened in 'Read' mode
  
    while(1){                       // A while loop used to run through the file contents. This while loop never terminates as it's checking condition is always logical '1'.
                                    // But the termination code snippet is present inside it which we will see later
    
     if(nr != rows){                // 'nr' is the variable which stores the number of rows read at current time. This if condition checks whether 'nr' is not equal to total 
                                    // number of rows, i.e., the file pointer has not reached the end (The checking of (feof(fp) != NULL) has some problems. Hence this method is used).
                                    
      fscanf(fp, "%s", str);        // A fscanf statement to access the file contents classified by blank spaces
      sscanf(str, "%lf", &element);          // The accessed content is converted into float value for manipulation purposes
      
      if(nr==0 && cols<cols2){      // If 'nr' = 0, it means the file pointer is currently in first row. We need to ignore this row and hence this if condition is used. 
       
       sum[cols]=0;                 // Also, this precious time is used to initialise the sum and sumsq arrays to zero. Also, we must check whether cols < cols2 as
       sumsq[cols]=0;               // cols is currently storing the number of column values of first row which may be bigger than the actual number of columns (cols2).
                                    
      }
      else{                             // If 'nr' isn't zero, this else part is executed
       sum[cols]+=element;              // The sum value of 'cols+1'th column is updated for 'nr'th row
       sumsq[cols]+=(element*element);  // The sumsq value of 'cols+1'th column is updated for 'nr'th row
      }
       
      cols++;                       // 'nr' value remains same. So we are still in the current row. But 'cols' is increased by one so that we can update sum and sumsq values of next column
      
      if(fgetc(fp)=='\n'){          // If fgetc returns a newline, it means we reached a row end. At that time, this if condition is executed
       cols=0;                      // cols is reinitialised to zero so that it can be used for next row
       nr++;                        // nr is increased by one, signifying we have reached next row and also recording it by increasing 'nr'
      }
     }
     else{                          // This else condition executes when 'nr' is equal to 'rows', i.e., the file pointer has reached the end of file becuase of which total number of rows read has 
                                    // become equal to the total number of rows. So now we break out of the while loop. This is the termination condition used to break out of the loop
                                    
      break;                        // Break statement to come out of the loop
     }
     
    } 
     
    fclose(fp);                 // File is once again closed.
    
    /*
    By the end of the second while loop, we have stored the sum and sum of squares values of all columns in arrays. We will use them to display the means, variances and standard deviations 
    on the output screen. Also, the variable 'cols' has become zero. We will use it to loop over all colmuns.
    */
    
     printf("Following values are calculated using file input/output in C language:\n\n"); // Displays the message that the mean, variance and standard deviation values, which are going to be 
                                                                                           // printed by the upcoming while loop are calculated by using file i/o in C language
    
    while(cols<cols2){                                      // Checks whether cols is less than cols2, i.e., this while loop loops over all columns
     double mean=sum[cols]/(rows-1);                        // Formula for mean
     double variance=(sumsq[cols]/(rows-1))-(mean*mean);    // Formula for variance
     double sd=sqrt(variance);                              // Formula for standard deviation 
     // Note that in all the above formulae, we have used (rows-1) value because the value of 'rows' accounts for the first row as well which we don't want
     
     if(mean<0.001 || variance<0.001){                        // This if condition checks whether the mean or variance values are less than a threshold, which is 0.001 here. If that's true,
                                                              // then the mean, variance and standard deviation are displayed in scientific notation
                                                                   
      printf("The mean of field %i is %le\n", cols+1, mean);                      // Mean is printed
      printf("The variance of field %i is %le\n", cols+1, variance);              // Variance is printed
      printf("The standard deviation of field %i is %le\n", cols+1, sd);          // Standard deviation is printed
      // Note that we've used 'cols+1' because the variable 'cols' starts from zero which corresponds to the first column. Similarly the 'cols'th index corresponds to 'cols+1'th column. 
      // Hence cols+1 is used
     }
     else{                                                  // If both mean and variance are greater than 0.001, this else part is executed. In this case, the values are printed as normal floating
                                                            // point (decimal) numbers
     
      printf("The mean of field %i is %lf\n", cols+1, mean);                      // Mean is printed
      printf("The variance of field %i is %lf\n", cols+1, variance);              // Variance is printed
      printf("The standard deviation of field %i is %lf\n", cols+1, sd);          // Standard deviation is printed
     }
     printf("\n");                                     // For aesthetic purposes. This command leaves a line between two consecutive column values on the output screen
     cols++;                                           // cols is increased by one so that we can print the values for next column
     sleep(1);                                         // Before pinting next column values, the code execution is paused for a second. This makes the different column values to occur at time 
                                                       // intervals of one second
    } 
   }  
    
   free(sum);                                          // sum dynamic array is freed
   free(sumsq);                                        // sumsq dynamic array is freed
   printf("Press ENTER to continue:\n");               // Asks the user to enter the "ENTER" key so that he/she can review the values first. The upcoming code won't be executed immediately and waits 
                                                       // until the user has pressed the "ENTER" key
   getchar();                                          // Accepts the "ENTER" key 
   
   
   /*
   The following code snippet emphasises the use of linux commands 'grep' and 'awk'. The system() function of C is used to execute these linux commands.
   
   Linux command way of calculating and displaying mean, variance and standard deviation explained:
                                                                                            Using linux commands 'grep' and 'awk', the following code prints the values for column '3'. The grep command get rids of the first row by checking the presence of the '#' character. Usually datasets contain a first row which contains the field names. This row doesn't give us any values for manipulation. Rather it contains the field names. So to distinguish them,  a '#' character is usually included in it. Hence, the grep command removes the first row from manipulation.
                                                                                            
The 'awk' command does the computation for finding mean, variance and standard distribution. In this particular code, only the values for column '3' are calculated and displayed. Finally also a message is printed which asks the user to enter the "ENTER" key and hence, providing him time to check the values.
   */
   
   
    printf("The following values are calculated using the linux commands: grep and awk: \n\n"); // Displays the message that the values that are going to be printed are calculated using the linux 
                                                                                                // commands: 'grep' and  'awk'
                                                                                                
   char command[500] = " grep -v '#' ";  // A character array named 'command' is initialised with the 'grep' part of the linux command              
   strcat(command, argv[1]);             // The file path, which is stored in argv[1], is concatenated to the array 'command'
   strcat(command, "| awk ' BEGIN{sum = 0; sumsq = 0;} {sum += $3; sumsq += ($3)*($3);} END{printf \"Mean of field 3 is \" sum/NR; print \"\"; print \"Variance of field 3 is \" ((sumsq/NR)-((sum/NR)*(sum/NR))); printf \"Standard deviation of field 3 is \" sqrt((sumsq/NR)-((sum/NR)*(sum/NR))); print \"\"; print \"\"; printf \"Press 'ENTER' key to exit:\"} ' ");
                                         // Finally, the 'awk' part of the linux command is concatenated to the array 'command'
                                         
   system(command);                  // system() function is used to run the linux command
   getchar();                        // To accept the pressing of the "ENTER" key
   system("clear");                  // system() function runs the "clear" command, which clears the output screen and makes it free for next program execution 
  }
 } 
 return 0;                           // Return statement of the main() function
}
