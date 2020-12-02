/*     ***    Numerical integration using trapezoid and romberg methods ***

This program code reads a given dataset, considers its first column as independent variable (x) and second column as dependent variable (y), depending on the 
order given it considers a specific set of points as interval (for eg, if order=5, then 2^(5-1)=16. So every 17 points(0 to 16, 16 to 32, etc.) will be 
considered as an integral for the romberg integration), finds the trapezoid and romberg integrals for all such intervals and prints those integral values.

Created by: V. Ruban Vishnu Pandian EE19B138
Created on: 14th November, 2020

Command usage: ./a.out <Datafilepath> <Order> (Used after the linux command "gcc integration.c -lm") (or)
               ./<Filename> <Datafilepath> <Order> (Used after the linux command "gcc integration.c -lm -o <Filename>")
*/

#include<stdlib.h>
#include<stdio.h>
#include<math.h>  //Including the libraries needed for the code

double trapezoid(double *arrin, double *arrout, int num) //Function to compute the trapezoid integral
{
	double temp = (arrin[num]-arrin[0])*(arrout[num]+arrout[0])/2;
	return temp;
}

void romberg(double *arrin, double *arrout, double *strer, int num, int ord)  //Function to compute the romberg integrals
{
	double h = arrin[num]-arrin[0];
	double *romm;
	romm = (double* ) calloc(ord*ord, sizeof(double));  //Space for the romberg matrix is allocated. (Since we can't allocate space
	                                                    //for a 2D array dynamically, the matrix is allocated space by treating it as a 1D array)

    /*The For loop present below computes all the first column elements of the romberg matrix.
	(The integral values due to methods like Trapezoids, Simpsons, Booles are the elements of first column)
    References: 1. https://en.wikipedia.org/wiki/Romberg%27s_method
                2. https://www.youtube.com/watch?reload=9&v=vzo47UoDwDA
    */
	for(int i=0; i<ord; i++)
	{
		int l2 = pow(2, ord-i-1);
		int l=l2;
		romm[i] = arrout[num]+arrout[0];
		while(l<num)
		{
			romm[i] = romm[i]+arrout[l]+arrout[l];
			l+=l2;
		}
		romm[i] = romm[i]*h/pow(2,i+1);       
	}                                          

    /*The for loop present below computes all the romberg extrapolations according to those provided in the reference links.
    The  diagonal elements of the romberg matrix corresponds to the romberg integrals of different orders. 
    */
	for(int j=1; j<ord; j++)
	{
		for(int i=j; i<ord; i++)
		{
			romm[(j*ord)+i] = ((pow(4,j)*romm[((j-1)*ord)+i])-romm[((j-1)*ord)+i-1])/(pow(4,j)-1);
		}
	}

    /*The romberg integrals of different orders are stored in a storer array named "Strer".
	Whenever we use a pointer or array as an argument, the method of calling is call by reference. So whatever array we passed from main 
	as an argument for "Strer" will also get automatically synced with the current status of "Strer". 
	*/
	int ind=0, k=0;
	while(k<(ord*ord))
	{
		strer[ind] = romm[k];
		ind++;
		k = k+ord+1;
	}                         
	free(romm);
}

int main(int argc, char **argv) //Opening of the main and getting the inputs as command line arguments
{
	if(argc!=3) //If the number of arguments is not 3, we get the error message present below
	{
		printf("\n\nThe number of arguments must be exactly 3. Please re-enter a valid command.\n\n");
		exit(0);
	}

    float order_f = atof(argv[2]);
	int order = atoi(argv[2]);
    if(order<=0 || (order_f-order)!=0)  //If the order is not a perfect positive integer, the error message present below will be printed
    {
    	printf("\n\nCommand usage: %s %s %s\n", argv[0], argv[1], argv[2]);
    	printf("Seems like the order is not a perfect positive integer, i.e., either it is negative or/and a fraction.\n");
    	printf("Please re-enter a valid command.\n\n");
    	exit(0);
    }

	int cols = 0, rows = -1;
    char str[50];
    FILE *fp;
    fp = fopen(argv[1], "r");   // File is opened in 'Read' mode
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

    if(pow(2,order-1)>(rows-1))  //If the stepsize, which is 2^(order-1) is more than the index of the last element, this error message will be printed
    {
    	printf("\n\nSeems like the order is too big for the dataset. Please re-enter a smaller order.\n\n");
    	exit(0);
    }

    int nr = -1;
    double element, *x, *y;        // Dynamic allocation of arrays to store independent variable(x) and dependent variable(y)
    x = (double*) calloc(rows, sizeof(double));
    y = (double*) calloc(rows, sizeof(double));
    fp = fopen(argv[1],"r");             // File is again opened in 'Read' mode
    while(nr!=rows)
    {
        fscanf(fp, "%s", str);           // File content is scanned
        sscanf(str, "%lf", &element);    // String to float conversion

        if(nr!=-1)                        // First row is ignored because it stores only the field names
        {
            if(cols==0)     
            {
                x[nr] = element;         // Whenever 'cols' is zero, the input value of that row is scanned. So this condition is checked and later 
                                         // only we store the file content in the independent variable array (x)
            }
            else if(cols==1)
            {
                y[nr] = element;          // Output is stored in dependent variable array (y)
            }
            cols++;                       //Cols is incremented so that other columns aren't considered 
        }
        if(fgetc(fp)=='\n')              
        {
            nr++;              // When a newline is encountered, 'nr' is incremented and 'cols' is re-initialised to zero
            cols = 0;
        }
    } 
    fclose(fp);                // Closing of file

    int p = pow(2, order-1);                              //Stepsize is stored in a variable named 'p'
    double *tempin, *tempout, *romb, *storer;                
    tempin = (double* ) calloc(p+1, sizeof(double));      //Used to store the independent variables of the current interval
    tempout = (double* ) calloc(p+1, sizeof(double));     //Used to store the dependent variables of the current interval
    romb = (double* ) calloc(order, sizeof(double));      //Used to store the total romberg integrals 
    storer = (double* ) calloc(order, sizeof(double));    //Used to store the current interval's romberg integral values
    int n = (rows-1)/p;                                   //Used to store the number of intervals
    double trap=0;                                        //Used to store the trapezoid integral
   
    for(int i=0; i<n; i++)                                //A For loop to loop over the intervals
    {
    	for(int j=0; j<=p; j++)
    	{
    		tempin[j] = x[i*p+j];
    		tempout[j] = y[i*p+j];                       //The current interval values are stored
    	}
        
        trap += trapezoid(tempin, tempout, p);           //The trapezoid integral is updated
        romberg(tempin, tempout, storer, p, order);      //The romberg integrals for current interval are obtained in the "Storer" array
        for(int u=0; u<order; u++)
        {
        	romb[u] += storer[u];                        //The romberg integrals are updated
        }

    }
 
    /*
    The trapezoid and romberg integrals are printed below
    */
    printf("\n\nThe number of data points taken for computing the integral: %d\n", (n*p+1));
    printf("The stepsize (or) interval length in this case is %d points.\n", p);
    printf("\nThe integral done using trapezoid method (1st order romberg) is:%le\n\n", trap);
    printf("These are the integral values due to all the orders from 2 to the order you've\nentered (%d in this case):\n", order);
    for(int i=0; i<order; i++)
    {
    	printf("%le ", romb[i]);
    }
    printf("\n\n");

    
    free(x);
    free(y);
    free(tempin);
    free(tempout);
    free(romb);
    free(storer);      //All the dynamic arrays are freed
    return 0;          //Return statement of the main
} 

