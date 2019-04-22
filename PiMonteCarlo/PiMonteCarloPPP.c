/* Transformación del problema PiMonteCarloSerie.c para la ejecución
bajo el entorno MPI y la aceleración del cómputo */
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

    int  len, rc; //variables para controlar MPI
    char hostname[129];
                
    int worldRank;
    int worldSize;
    int i = 0;
    // initialize MPI  
    MPI_Init(NULL,NULL);
    
    // Find out rank, size
    MPI_Comm_size(MPI_COMM_WORLD,&worldSize); 
    MPI_Comm_rank(MPI_COMM_WORLD,&worldRank);
    MPI_Get_processor_name(hostname, &len);

    //Semilla general del sistema
    srand(time(NULL));
                
                //Vectores neesarios para recoger valores e inicializar correctamente los numeros aleatorios
                long parcialCircleHits[worldSize];
                long semillas[worldSize];
                //Inicializacion para los vectores
                memset(parcialCircleHits,0,worldSize);
                for(i = 0; i < worldSize;i++)
                    semillas[i]=(long)rand()%10000000; //Semillas particulares para los procesos

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
                //Comienza el reparto de informacion para el mundo. Flujos de reparto
                for(i=1; i<worldSize;i++)
                {
                    MPI_Send(
                    /* data         = */ &numeroPuntos, 
                    /* count        = */ 1, 
                    /* datatype     = */ MPI_LONG, 
                    /* destination  = */ i, 
                    /* tag          = */ 0, 
                    /* communicator = */ MPI_COMM_WORLD);
                }
                
            }
            else //El mundo recibe informacion
            {
                MPI_Recv(
              /* data         = */ &numeroPuntos, 
              /* count        = */ 1, 
              /* datatype     = */ MPI_LONG, 
              /* source       = */ 0, 
              /* tag          = */ 0, 
              /* communicator = */ MPI_COMM_WORLD, 
              /* status       = */ MPI_STATUS_IGNORE);
            
                printf("Semilla[%ld].\t%s:[%d/%d] -> Recivido [%ld] dato.\n",semillas[worldRank],hostname, worldRank, worldSize,numeroPuntos);
            }
            
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
             
            //Flujos de retorno con soluciones parciales
            
            if(worldRank == 0)
            {
                for(i = 1; i < worldSize; i++)
                {
                    MPI_Recv(
              /* data         = */ &parcialCircleHits[i], 
              /* count        = */ 1, 
              /* datatype     = */ MPI_LONG, 
              /* source       = */ i, 
              /* tag          = */ 0, 
              /* communicator = */ MPI_COMM_WORLD, 
              /* status       = */ MPI_STATUS_IGNORE);
                }
                parcialCircleHits[0]=cp;
                
            }
            else
            {
                MPI_Send(
                    /* data         = */ &cp, 
                    /* count        = */ 1, 
                    /* datatype     = */ MPI_LONG, 
                    /* destination  = */ 0, 
                    /* tag          = */ 0, 
                    /* communicator = */ MPI_COMM_WORLD);
                
            }
            
            if(worldRank == 0)
            {
                printf("Mostrando Resultados Parciales: ");
                cp = 0;
                for(i=0;i<worldSize;i++)
                {
                    printf("\t%ld",parcialCircleHits[i]);
                    cp = cp + parcialCircleHits[i]; 
                }
                //construyendo Solucion
                //pi = (double)(4 * cp) / sp; 
                pi = 4*(cp/((float)sp))/worldSize;
                printf("pi: %lf\n\n",pi); 
                
            }
        //Finalizando
        printf("%s:[%d/%d] -> task Done!!\n",hostname, worldRank, worldSize);
        // done with MPI  
        MPI_Finalize();
        }           
    }
  
    return 0;
}
