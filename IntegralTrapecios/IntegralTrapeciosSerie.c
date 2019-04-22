/* */
//#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ln4 1.38629436112

//[[f(a+((i+1)*h))-f(a+(i*h))]*h]/2

double fdX(double x)
{
    return 1/x;
    //return x*x*x;
}


int main(int argc, char *argv[])
{
    

int numeroIntervalos = 0, i = 0;
double a = 0.0, b = 0.0;
double anchuraSubInterbalo = 0.0, areaSubintervalo = 0.0 , areaAcumulada=0.0, alturaConsideradaAnterior=0.0, alturaConsideradaPosterior = 0.0;





//if(!(numeroPuntos <= 1 && numeroPuntos >= 0))

    if( argc != 4 )
    {    
        fprintf(stderr,"Esperamos como parametros: [Limite inferior] [Limite superior] [Nº Intervalos de calculo]\nEj. IntRectangulo 1.0 4.0 10000\n\n");
        return -1;
    }    
    else
    {
        numeroIntervalos = atoi(argv[argc-1]);
        a = atof(argv[argc -3]);
        b = atof(argv[argc -2]);
        anchuraSubInterbalo = (b-a)/(double)numeroIntervalos;
        printf("a: %.16lf, b: %.16lf, subIntervalos: %d, anchuraSubInterbalo: %.16lf\n",a,b,numeroIntervalos,anchuraSubInterbalo);
        
        for(i = 0; i < numeroIntervalos;i++)
        {
            alturaConsideradaAnterior = fdX(a+i*anchuraSubInterbalo);
            alturaConsideradaPosterior = fdX(a+(i+1)*anchuraSubInterbalo);
            //areaSubintervalo = alturaConsideradaAnterior*anchuraSubInterbalo + ((alturaConsideradaPosterior-alturaConsideradaAnterior)*anchuraSubInterbalo)/2;
            areaSubintervalo = (alturaConsideradaAnterior+alturaConsideradaPosterior)*(0.5)*anchuraSubInterbalo;
            areaAcumulada = areaAcumulada + areaSubintervalo;
            //printf("f(%.16lf)=%.16lf\tArearIntervalo:%.12lf\tAreaAcumulada:%.12lf\n",a+i*anchuraSubInterbalo,alturaConsideradaAnterior,areaSubintervalo, areaAcumulada);
            
        }
        printf("AreaAcumulada:%.12lf\n",areaAcumulada);
        printf("Error Relativo %.16lf/%.16lf = %.16lf\n\n",areaAcumulada,ln4,areaAcumulada/ln4);
        
        
            
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
