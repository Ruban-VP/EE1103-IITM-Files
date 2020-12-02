/* ***   Numerical solution of Landau-Lifshitz-Gilbert equation   ***

This program code solves the Landau-Lifshitz-Gilbert differential equation based on the Runge Kutta 5th order and Heun's method using the 
values of alpha and step size given by the user. Also, based on the choice the user enters, it displays:

Choice 1: "Iteration_number" "Error_in_ Mx" "Error_in_My" "Error_in_Mz" (Error at that iteration) 

Choice 2: "Step_size" "Root_mean_squared_error_of_Theta_in_radians" (Root mean square error of theta after all iterations are done)

Choice 3: "Alpha" "Switching_time (Time when Mz becomes positive or Theta becomes less than 90 degrees)"

Choice 4: "Time" "Mx" "My" "Mz" (All the magnetisation vector components are w.r.t the values of theta and phi obtained from RK5 method)

Created by: V. Ruban Vishnu Pandian EE19B138
Created on: 17th October, 2020

Command input example: ./a.out <Stepsize> <Alpha> <Choice> (Used after the linux command "gcc EE19B138_4.c -lm") (or)
                       ./<Filename> <Stepsize> <Alpha> <Choice> (<Filename> is the name used in linux command "gcc EE19B138_4.c -lm -o <Filename>")
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h> //Including the libraries needed for the code

float thetder(float gam, float alph, float H, float thet)
{
	float temp = gam*alph*H*sin(thet)/((alph*alph)+1);      //Function to return the derivative of theta w.r.t time
	return temp;
}

int main(int argc, char **argv)  //Opening of the main function and getting the command line inputs
{
	if(argc!=4)   //If the number of commands is not 4 (which we don't want), this message is printed and the code exits
    {
        printf("\n\nThe number of arguments must be exactly 4. Please re-enter a valid command.\n\n");
        exit(0);
    }

    int choice = atoi(argv[3]);  //Getting the choice
    float ch = atof(argv[3]);
    
    float gamma = -1.76*1e11, Hz = 1000000/(4*M_PI), h = atof(argv[1]), alpha = atof(argv[2]);   
    //Gamma and Hz are given above. Also, stepsize and alpha are stored in variables "h" and "alpha" respectively
    float theta = 179*M_PI/180, theta2 = theta = 179*M_PI/180, phi = M_PI/180;
	//The initial values of theta, theta2 and phi are 179 and 1 degrees respectively. 
    //(theta corresponds to the RK5 method, theta2 corresponds to the Heun's method)
    
    float k1, k2, k3, k4, k5, k6, p1, p2, t_switch;                                 //Variables used for the RK5 and Heun's method
	float phider = ((-1)*gamma*Hz)/((alpha*alpha) + 1);                             //Derivative function of phi (Constant in this case)
    float mx1, my1, mz1, mx2, my2, mz2, errorx, errory, errorz, error;
    //mx1, my1, mz1 correspond to the magnetisation vector components of RK5 method
    //mx2, my2, mz2 correspond to the magnetisation vector components of Heun's method
    //error(i) corresponds to the error between m(i)1 and m(i)2  (i belongs to {x, y, z})
    //error correpsonds to error between theta and theta2
    unsigned long int i=0; 
    int temp = 0;

    if(alpha>1 || alpha<0 || choice-ch != 0 || choice<1 || choice>4) 
    {
        printf("\n\nCommand usage: %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3]);
        printf("Seems like either alpha is not wihin the specified range [0, 1] or choice is not belonging to the set {1, 2, 3, 4}.\n");
        printf("Please re-enter a valid command.\n\n");
        exit(0);
    }
    //If alpha is not within [0, 1] or if choice is not belonging to {1, 2, 3, 4} (which we don't want), this above IF condition is 
    //executed displaying an error message and the code exits

    while(theta>=(M_PI/180) && theta2>=(M_PI/180))  //If all parameters are fine, then this while loop will be executed
    {
        mx1 = sin(theta)*cos(phi);
        my1 = sin(theta)*sin(phi);
        mz1 = cos(theta);
        
        mx2 = sin(theta2)*cos(phi);
        my2 = sin(theta2)*sin(phi); 
        mz2 = cos(theta2);                   //Formuale for the magnetisation vector components
 
        errorx = sqrt((mx1-mx2)*(mx1-mx2));
        errory = sqrt((my1-my2)*(my1-my2));
        errorz = sqrt((mz1-mz2)*(mz1-mz2));

        error = error + ((theta-theta2)*(theta-theta2));   //Formulae for the error variables

        if(choice == 1)
        {
            printf("%lu %le %le %le\n", i, errorx, errory, errorz); //If choice is 1, then the errors in Mx, My, Mz are printed w.r.t the iteration number
        }
        else if(choice == 4)
        {
            printf("%le %le %le %le\n", h*i, mx1, my1, mz1);     //If choice is 4, then the magnetisation vector components, calculated using 
                                                                 //RK5 method are printed w.r.t time
        }

        
        if(theta<(M_PI/2) && temp==0)
        {
            t_switch = i*h;                   //Switching time is calculated if "RK5 theta" < 90 degrees
            temp++;
        }
        
        
        k1 = thetder(gamma, alpha, Hz, theta);
        k2 = thetder(gamma, alpha, Hz, theta+(k1*h/4));
        k3 = thetder(gamma, alpha, Hz, theta+((k1 + k2)*h/8));
        k4 = thetder(gamma, alpha, Hz, theta+((k3 - (k2/2))*h));
        k5 = thetder(gamma, alpha, Hz, theta+(((3*k1) + (9*k4))*h/16));
        k6 = thetder(gamma, alpha, Hz, theta+((((-3)*k1) + (2*k2) + (12*k3) + ((-12)*k4) + (8*k5))*h/7));
        //All variables used in increment of RK5 theta (variable "theta") are calculated

        p1 = thetder(gamma, alpha, Hz, theta2);
        p2 = thetder(gamma, alpha, Hz, theta2 + (p1*h));
        //All variables used in increment of Heun's theta (variable "theta2") are calculated

        theta = theta + (((7*k1) + (32*k3) + (12*k4) + (32*k5) + (7*k6))*h/90);   //RK5 theta is incremented
        theta2 = theta2 + ((p1+p2)*h/2);                                          //Heun's theta is incremented
        phi = phi + (phider*h);                            
        //The time derivative of phi is constant. So any method we use boils down to the Euler's method. So that method is directly used
        i++;  //Iteration number is incremented

        if(phi>(2*M_PI))
        {
            phi = phi - (2*M_PI);       
        }
        else if(phi<0)
        {
            phi = phi + (2*M_PI);
        }                           //To preserve phi within its principal domain [0, 2*pi] radians, these IF and ELSE conditions are used 
    }
    
    error = sqrt(error/i);    //Root mean square error of theta is calculated after the while loop ends
    if(choice == 2)
    {
        printf("%le %le\n", h, error);   //If choice is 2, Stepsize and RMS error of theta are printed
    }
    else if(choice == 3)
    {
        printf("%le %le\n", alpha, t_switch); //If choice is 3, alpha and switching time are printed
    }
    
    return 0; //Return statement of the main function
}

