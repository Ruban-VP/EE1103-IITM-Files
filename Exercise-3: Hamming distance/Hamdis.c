
/* ***   Hamming distances program  ***
This program code generates a randomised bit array of given length, calculates the hamming distances by taking 32 bits at a time and comparing it with another given 32-bit number
and displays the positions at which the minimum hamming distance occurs.

Created by: V. Ruban Vishnu Pandian EE19B138
Created on: 4th September, 2020

Command input example: ./a.out <Array length> <32-bit comparison number> (Used after the linux command gcc Hamdis.c) (or)
                       ./<Filename> <Array length> <32-bit comparison number> (<Filename> is the name used in linux command "gcc Hamdis.c -o <Filename>")
                       
Note: Try keeping the array length less than 10000000 for faster computation.
      Also note that the comparison number is a 32 bit number. So its value should strictly belong to this range: 0 to 4294967295

*/

#include <stdio.h>
#include <stdlib.h>
#include<time.h>      // Including the libraries needed for the code       

int hdis(unsigned int xorval)   // Function to find the sum of bits in the bit reperesentation a decimal number (Used to find the hamming distance)
{
 int rem=0, sum=0;
 
 for(int i=0; i<32; i++)  // Since we deal only with 32 bit numbers in this program, the number of iterations is known and it is 32. So a for loop is used
 {
  rem = xorval%2;        // Remainder when divided by two (The bit we need)
  xorval/=2;             // The number is made equal to the quotient
  sum+=rem;              // The sum variable is updated after every iteration by adding the remainder, i.e., adding the bits of the bit representation
 }
 
 return sum;             // Return statement of the function
}

void printbits(unsigned int temp)   // Function to print the bit representation of a decimal number
{
 int i=0, rem, sum=0; 
 char arr[32];
 for(int i=0; i<32; i++) // Again, we know that we are dealing with only 32 bit numbers. So the number of iterations is 32.
 {
  rem = temp%2;          // Remainder bit
  arr[i]=rem;            // Bits are stored in an array 
  temp/=2;               // Number updated to quotient
 }
 
 for(int j=31; j>=0; j--)
 {
   printf("%d", arr[j]);  // Array is printed in reverse as we want the least significant bit to be printed at last which is stored first in the array
 }
}
  
int main(int argc, char** argv)  // Opening of main function and command line arguments are taken as argv values
{
 if(argc!=3)  // Checks if argc!=3 which we dont want. 
 {
  printf("\nNumber of commands must be exactly 3. Please enter a valid command.\n\n");  // If argc is not 3, this error message is printed
 }
 else       // If argc==3, which we want, this else condition is executed
 { 
  float a = atof(argv[1]) - atoi(argv[1]);
  float b = atof(argv[2]) - atoi(argv[2]);
  unsigned long long int N = atoi(argv[1]);
  unsigned int M = atoi(argv[2]);
  
  if(a==0 && b==0 && N>0 && M>0 && (N%32==0))       // Checks whether the bit array length and the comparison number are positive integers with the array length being a multiple of 32
  {
   int flag = 1;
   if(N>1024)
   {
    printf("\nThe bit array length seems to be quite big to display on the screen. Do you still want to display it and the associated hamming distances? (y/n):\n");
    // If the user enters a very large array length, this message is printed which asks him whether to print the array and hamming distances for reference or not.
    char ch = getchar();
    if(ch == 'y' || ch == 'Y')
    {
     flag = 1;  // A flag variable is made 1 if the user wants the array to be printed
    }
    else
    {
     flag = 0;  // Else, the flag is made zero
    }
   }   
     
   unsigned int *randarr, *hamdis;
   randarr = (unsigned int*) calloc(N/32, sizeof(unsigned int));  // Dynamic allocation for the random bit array
   hamdis = (unsigned int*) calloc(N-31, sizeof(unsigned int));   // Dynamic allocation for hamming distances array
 
   srand(time(0));                  // Seeding the random number generator algorithm
   for(unsigned long long int i=0; i<N/32; i++)
   {
    randarr[i] = rand() % 4294967296;   // Random bit array is initialised randomly with some values which are 32-bit numbers (0 to 4294967295)
   } 
  
   if(flag == 1)
   {
    printf("\nThe randomised bit array is: ");  
   } 
   unsigned long long int k=0; 
   while(flag==1 && k<N/32)     // While loop to print the bit representations of the random 32-bit numbers (provided flag==1, i.e., the user has agreed for the printing of the array)
   {
    printbits(randarr[k]);
    k++;
   }
   printf("\n\n");
   printf("The decimal number to which the bit strings are compared is: %i ", M);  // The comparison number is printed in decimal form
   printf("\n(");
   printbits(M);                                                                   // As well as in bit representation
   printf(")\n\n");
 
   if(flag == 1)
   {
    printf("The hamming distances are: ");
   } 
   int minhdis=32, sum=0;
   for(unsigned long long int i=0; i<N/32; i++)       // Calculates the hamming distances and also finds the least value out of them (prints them as well if flag==1)
   {
    if(i!=(N/32-1))                // If we haven't yet reached the last 32-bit number in the random array, this if condition is executed 
    {
     for(int j=0; j<32; j++)       // Since we can't loop through the intermediate bits like 1-32, 4-35 directly, we use this for loop and shift operations
     {
      sum = (randarr[i]<<j)+(randarr[i+1]>>(32-j));  // Let's assume we want the 32-bits with indices 4-35. We can't access them directly. So the first number (0-31 bits) is shifted left by j=4 bits
                                                     // and the second number (32-63 bits) is shifted right by 32-j = 28 bits. So when we add these two, we get the original 4-35 bits. This is 
                                                     // possible because C language, by default, fills the vacant bits created during shift operation with zeroes.
     
      hamdis[32*i+j] = hdis(sum ^ M);    // sum ^ M is the bitwise XOR. So its bit representation contains the info about where the bits of sum and M weren't equal. We pass this sum ^ M value to
                                         // to the hdis function to get the hamming distance value
                                         
      if(hamdis[32*i+j]<minhdis)            
      {
       minhdis = hamdis[32*i+j];         // If "minhdis" (variable used to store the minimum hamming distance) value is more than the current iteration's hamming distance value, i.e., hamdis[32*i+j],
                                         // it  is updated as minhdis = hamdis[32*i+j]
      }
      if(flag==1)
      {
       printf("%d ", hamdis[32*i+j]);   // Hamming distances are printed if flag==1
      }
     }
    } 
    else                                // Executes if i == N/32-1, i.e., when we reached the last 32-bit number
    {
     hamdis[32*i] = hdis(randarr[i] ^ M);       // Hamming distance is calculated and stored at the last position of the hamdis array 
     if(flag == 1)
     {
      printf("%d ", hamdis[32*i]);     // Again, the hamming distance is printed if flag==1    
     } 
     if(hamdis[32*i]<minhdis)
     {
       minhdis = hamdis[32*i];       // If the last hamdis value is lesser than the minhdis value, minhdis is updated as minhdis = hamdis[32*i];
     }
    }
   }

   printf("\n\nThe minimum hamming distance is: %d\n\n", minhdis);
   printf("The positions at which the minimum hamming distance occurs are: ");
   for(unsigned long long int i=0; i<N-31; i++)        // Loops through the hamming distances array
   {
    if(minhdis == hamdis[i])          // Checks if the current iteration's hamming ditance is the minimum value, i.e., minhdis
    {
     printf("%llu ", i+1);            // If it is the minimum value, then the position of it is printed
    } 
   } 
 
   free(randarr);           // Dynamic random bit array (randarr) memory is freed 
   free(hamdis);            // Dynamic Hamming distances array (hamdis) memory is freed
   printf("\n\n");
  }
 
  else  // Executed if the array length or the comparison number is not entered properly ( A non-number character,  a negative number, a floating point number are not valid candidates for the array
        // length and the comparison number. Additionally, numbers which aren't multiples of 32 aren't valid candidates for the array length in this program).
  {
   printf("\nCommand usage: %s %s %s\n", argv[0], argv[1], argv[2]);
   printf("Please enter a valid command. Note that Number of bits in the random array must be a multiple of 32 for this code to work. \n"); // Asks the user to re-enter a valid command
   printf("Also try keeping it small , i.e., less than 10000000 for faster output.\n");
   printf("Note that the comparison number is a 32 bit number.So its value should strictly belong to this range: 0 to 4294967295\n\n");
  } 
 }

 return 0; // Return statement of the main function
}

