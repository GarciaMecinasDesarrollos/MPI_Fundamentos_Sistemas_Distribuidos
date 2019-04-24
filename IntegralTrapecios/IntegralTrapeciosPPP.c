
/* Solucion con comunicaciones PPP para la integración númeria basada en trapecios*/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ln4 1.38629436112

//Funcion de evaluacion
double fdX(double x)
{
    return 1/x;
    //return x*x*x;
}


int main(int argc, char *argv[])
{
    long numeroIntervalos = 0;
    int i = 0;
    double a = 0.0, b = 0.0;
    double anchuraSubInterbalo = 0.0, areaSubintervalo = 0.0 , areaAcumulada=0.0, alturaConsiderada=0.0;
    double dominioReparto = 0.0;
    int worldRank;
    int worldSize;
    
    double dominio[2];
    dominio[0]=0.0;
    dominio[1]=0.0;
    //dominio[2]=0.0;
    
    int  len, rc; //variables para controlar MPI
    char hostname[129];

    // initialize MPI  
    MPI_Init(NULL,NULL);
    
    // Find out rank, size
    MPI_Comm_size(MPI_COMM_WORLD,&worldSize); 
    MPI_Comm_rank(MPI_COMM_WORLD,&worldRank);
    MPI_Get_processor_name(hostname, &len);


    if( argc != 4 )
    {    
        fprintf(stderr,"Esperamos como parametros: [Limite inferior] [Limite superior] [Nº Intervalos de calculo]\nEj. IntRectangulo 1.0 4.0 10000\n\n");
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
                numeroIntervalos = atoi(argv[argc-1]);
                a = atof(argv[argc -3]);
                b = atof(argv[argc -2]);
                printf("Problema : Integracion Numerica Aproximada por Rectangulos. [A:%lf,B:%lf] Intervalos:%ld\n",a,b,numeroIntervalos);
                dominio[0]=a;
                dominio[1]=b;
                
                //El calculamos los puntos por cada proceso
                //numeroIntervalos = numeroIntervalos/worldSize;
                anchuraSubInterbalo = (b-a)/(double)numeroIntervalos;
                //Comienza el reparto de informacion para el mundo. Flujos de reparto
                for(i=1; i<worldSize;i++)
                {
                    MPI_Send(
                    /* data         = */ &numeroIntervalos, 
                    /* count        = */ 1, 
                    /* datatype     = */ MPI_LONG, 
                    /* destination  = */ i, 
                    /* tag          = */ 0, 
                    /* communicator = */ MPI_COMM_WORLD);
                    
                    MPI_Send(
                    /* data         = */ &dominio, 
                    /* count        = */ 2, 
                    /* datatype     = */ MPI_DOUBLE, 
                    /* destination  = */ i, 
                    /* tag          = */ 0, 
                    /* communicator = */ MPI_COMM_WORLD);
                    
                    MPI_Send(
                    /* data         = */ &anchuraSubInterbalo, 
                    /* count        = */ 1, 
                    /* datatype     = */ MPI_DOUBLE, 
                    /* destination  = */ i, 
                    /* tag          = */ 0, 
                    /* communicator = */ MPI_COMM_WORLD);
                }
            }
            else //El mundo recibe informacion
            {
                MPI_Recv(
              /* data         = */ &numeroIntervalos, 
              /* count        = */ 1, 
              /* datatype     = */ MPI_LONG, 
              /* source       = */ 0, 
              /* tag          = */ 0, 
              /* communicator = */ MPI_COMM_WORLD, 
              /* status       = */ MPI_STATUS_IGNORE);
                MPI_Recv(
              /* data         = */ &dominio, 
              /* count        = */ 2, 
              /* datatype     = */ MPI_DOUBLE, 
              /* source       = */ 0, 
              /* tag          = */ 0, 
              /* communicator = */ MPI_COMM_WORLD, 
              /* status       = */ MPI_STATUS_IGNORE);
                MPI_Recv(
              /* data         = */ &anchuraSubInterbalo, 
              /* count        = */ 1, 
              /* datatype     = */ MPI_DOUBLE, 
              /* source       = */ 0, 
              /* tag          = */ 0, 
              /* communicator = */ MPI_COMM_WORLD, 
              /* status       = */ MPI_STATUS_IGNORE);
            }
            
            dominioReparto = (dominio[1]-dominio[0])/worldSize;
            dominio[0]=dominio[0]+(worldRank*dominioReparto);
            dominio[1]=dominio[0]+dominioReparto;
            numeroIntervalos = numeroIntervalos / worldSize;
            
            printf("[%d]/[%d] --> a: %.16lf, b: %.16lf, subIntervalos: %d, anchuraSubInterbalo: %.16lf\n",worldRank,worldSize,dominio[0],dominio[1],numeroIntervalos,anchuraSubInterbalo);
            
            //Ejecutamos el problema
            
            for(i = 0; i < numeroIntervalos;i++)                
            {
                alturaConsideradaAnterior = fdX(dominio[0]+i*anchuraSubInterbalo);
                alturaConsideradaPosterior = fdX(dominio[0]+(i+1)*anchuraSubInterbalo);
            
                areaSubintervalo = (alturaConsideradaAnterior+alturaConsideradaPosterior)*(0.5)*anchuraSubInterbalo;
                areaAcumulada = areaAcumulada + areaSubintervalo;
            }
            printf("f(%.16lf)=%.16lf\tArearIntervalo:%.12lf\tAreaAcumulada:%.12lf\t\tTask DONE!!\n",a+i*anchuraSubInterbalo,alturaConsiderada,areaSubintervalo, areaAcumulada);
            
            if(worldRank == 0)
            {
                double areasParciales[worldSize];
                        memset(areasParciales,0.0,worldSize);
                for(i = 1; i < worldSize; i++)
                {
                    MPI_Recv(
              /* data         = */ &areasParciales[i], 
              /* count        = */ 1, 
              /* datatype     = */ MPI_LONG, 
              /* source       = */ i, 
              /* tag          = */ 0, 
              /* communicator = */ MPI_COMM_WORLD, 
              /* status       = */ MPI_STATUS_IGNORE);
                }
                areasParciales[0]=areaAcumulada;
                areaAcumulada=0;
                for(i=0;i<worldSize;i++)
                {
                     areaAcumulada = areaAcumulada + areasParciales[i];
                }
                
            }
            else
            {
                MPI_Send(
                    /* data         = */ &areaAcumulada, 
                    /* count        = */ 1, 
                    /* datatype     = */ MPI_LONG, 
                    /* destination  = */ 0, 
                    /* tag          = */ 0, 
                    /* communicator = */ MPI_COMM_WORLD);
                
            }
 
            //Presentacion Resultados
            if(worldRank == 0)
            {
                 printf("Integracion Numerica Aproximada por Rectangulos. [A:%lf,B:%lf] Resultado:%lf\n\n",a,b,areaAcumulada);
            }
        
        }
    }
    MPI_Finalize();
    return 0;
}
