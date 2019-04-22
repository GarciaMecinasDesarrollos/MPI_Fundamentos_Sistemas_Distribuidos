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

//if(!(numeroPuntos <= 1 && numeroPuntos >= 0))

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
            //if(x <= 1.0 && y <= 1.0)
            
            sp++;
            
            //printf("x = %f; y = %f; d = %f; ",x,y,d);
            //printf("PI=%f\n",(4*(cp/(float)sp)));
            
        }
        pi = (double)(4 * cp) / sp; 
        //pi = 4*(cp/((float)sp));
        printf("%lf\n\n",pi);
            
    }
    
    
    
        //Captemos argumentos esperamos solamente el numero de puntos
        /*for(i = 0; i < argc; i++)
        {
            printf("%s\n",argv[i]);
        }*/    
    
        //int  numtasks, rank, len, rc;
        //char hostname[MPI_MAX_PROCESSOR_NAME];

        // initialize MPI  
        //MPI_Init(&argc,&argv);

        // get number of tasks
        //MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

        // get my rank  
        //MPI_Comm_rank(MPI_COMM_WORLD,&rank);

        // this one is obvious  
        //MPI_Get_processor_name(hostname, &len);
        //printf ("Number of tasks= %d My rank= %d Running on %s\n", numtasks,rank,hostname);


        // do some work with message passing


        // done with MPI  
        //MPI_Finalize();

    
    return 0;
}
