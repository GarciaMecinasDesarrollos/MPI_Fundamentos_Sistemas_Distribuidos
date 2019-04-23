/* Implementación del algoritmo serie para estimar el valor de PI
con la aproximación propuesta por los algoritmos de MonteCarlo */

//#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[])
{
    

    long numeroPuntos = 0;
    double x = 0.0, y =0.0, d = 0.0, divisor = 1/10000000.0, pi=0.0;// doublecp = 0.0, doublesp = 0.0;
    long cp = 0, sp = 0; /* CirclePoint SquarePoints */

    srand(time(NULL));

    if( argc != 2 )
    {    
        fprintf(stderr,"Solo esperamos el numero de puntos como argumento entero.\n");
        return -1;
    }    
    else
    {
        numeroPuntos = atoi(argv[argc-1]);
        printf("PI aproximado por %ld puntos: ",numeroPuntos);
        
        while(sp < numeroPuntos)
        {    
        
            x=(rand()%10000000)*divisor; //7 digitos decimales
            y=(rand()%10000000)*divisor; //7 digitos decimales
            d = (x*x)+(y*y);
            if(d <= 1.0)
            { 
                cp++; 
            }
            sp++;     
        }
        pi = (double)(4 * cp) / sp; 
        //pi = 4*(cp/((float)sp));
        printf("%lf\n\n",pi);      
    }
    
    return 0;
}
