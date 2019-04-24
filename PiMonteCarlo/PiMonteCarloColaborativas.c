/* Transformación del código PiMonteCarloPPP para el uso de las comunicaciones colaborativas*/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



int main(int argc, char *argv[])
{
    

    long numeroPuntos = 0;
    double x = 0.0, y =0.0, d = 0.0, divisor = 1/10000000.0, pi=0.0;// doublecp = 0.0, doublesp = 0.0;
    long cp = 0, sp = 0; /* CirclePoint SquarePoints */

    int  len; //variables para controlar MPI
    char hostname[129];
    long global_CP = 0;            
    int worldRank;
    int worldSize;
    
    int i = 0;
    // initialize MPI  
    MPI_Init(NULL,NULL);
    
    // Find out rank, size
    MPI_Comm_size(MPI_COMM_WORLD,&worldSize); 
    MPI_Comm_rank(MPI_COMM_WORLD,&worldRank);
    MPI_Get_processor_name(hostname, &len);
    long semillas[worldSize];
    //Semilla general del sistema
    srand(time(NULL));
    //Inicializacion para los vectores            
    for(i = 0; i < worldSize;i++)
        semillas[i]=(long)rand()%10000000; //Semillas particulares para los procesos            
    //Comprovamos parametros de la llamada especifica para el programa. Solo si sus argumentos son validos continuamos            
    if( argc != 2 )
    {    
        fprintf(stderr,"Solo esperamos el numero de puntos como argumento entero.\n");
        return -1;
    }    
    else
    {       
        if (worldSize < 2)//Si No estamos en condiciones de ejecutar, abortamos
        {
            fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        else//Si no continuamos ejecuntado
        {
            printf("%s:[%d/%d] -> Online, waiting task\n",hostname, worldRank, worldSize);

            if(worldRank == 0)
            {
                numeroPuntos = atoi(argv[argc-1]);
                printf("Problema : PI aproximado por %ld puntos\n",numeroPuntos);
                
                //El calculamos los puntos por cada proceso
                numeroPuntos = numeroPuntos/worldSize;
                
                //int MPI_Bcast( void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )
            }
            
            
            /*      Flujos de retorno con soluciones parciales      */       
            MPI_Bcast(&numeroPuntos, 1, MPI_LONG,0,MPI_COMM_WORLD);
            
            printf("Semilla[%ld].\t%s:[%d/%d] -> Recibido [%ld] dato.\n",semillas[worldRank],hostname, worldRank, worldSize,numeroPuntos);
            
            //PROBLEMA A RESOLVER
            
            srand(semillas[worldRank]); //Neceario para el programa, Las maquina son demasiado rapidas y toman el mismo valor de reloj
            while(sp < numeroPuntos)
            {     
                x=(rand()%10000000)*divisor; //7 digitos decimales
                y=(rand()%10000000)*divisor; //7 digitos decimales
                d = (x*x)+(y*y);
                if(d <= 1.0)
                { 
                    cp++; //incrementamos contador para puntos incidentes en el circulo
                }
                sp++; //incrementamos contador para puntos en el cuadrado   
            }

            //printf("x = %f; y = %f; d = %f; ",x,y,d); 
            printf("%s:[%d/%d] -> Puntos Calculados = %ld, incidencias en circulo: %ld\n",hostname, worldRank, worldSize,numeroPuntos,cp);
            
             
            /*      Flujos de retorno con soluciones parciales      */
            MPI_Reduce(&cp,&global_CP,1,MPI_LONG, MPI_SUM ,0 ,MPI_COMM_WORLD);
            
            if(worldRank == 0)
            {    
                //printf("Acumulado Global de CP: %ld\n",global_CP);
                pi = 4*(global_CP/((float)sp))/worldSize;
                printf("pi: %lf\n\n",pi); 
            }    
           // }
        //Finalizando
        printf("%s:[%d/%d] -> task Done!!\n",hostname, worldRank, worldSize);
        // done with MPI  
        MPI_Finalize();
        }           
    }
  
    return 0;
}
