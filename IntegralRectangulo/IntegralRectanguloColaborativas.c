/* Solucion con comunicaciones colaborativa para la integración númeria basada en trapecios*/
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
    double globalArea=0.0;
    int i = 0;
    double a = 0.0, b = 0.0;
    double anchuraSubInterbalo = 0.0, areaSubintervalo = 0.0 , areaAcumulada=0.0, alturaConsiderada=0.0;
    double alturaConsideradaAnterior = 0.0, alturaConsideradaPosterior=0.0;
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
                
            }
            
            MPI_Bcast(&numeroIntervalos, 1, MPI_LONG,0,MPI_COMM_WORLD);
            MPI_Bcast(&dominio, 2, MPI_DOUBLE,0,MPI_COMM_WORLD);
            MPI_Bcast(&anchuraSubInterbalo, 1, MPI_DOUBLE,0,MPI_COMM_WORLD);
            
//Tenemos en cuenta que dominio contiene el limite inferior del dominio en la posicion 0 y el limite superior del dominio en la posicion 1
            dominioReparto = (dominio[1]-dominio[0])/worldSize;
            dominio[0]=dominio[0]+(worldRank*dominioReparto);
            dominio[1]=dominio[0]+dominioReparto;
            numeroIntervalos = numeroIntervalos / worldSize;
            
            printf("[%d]/[%d] --> a: %.16lf, b: %.16lf, subIntervalos: %d, anchuraSubInterbalo: %.16lf\n",worldRank,worldSize,dominio[0],dominio[1],numeroIntervalos,anchuraSubInterbalo);
            
            //Ejecutamos el problema
            
            for(i = 0; i < numeroIntervalos;i++)                
            {
                alturaConsiderada = fdX(dominio[0]+i*anchuraSubInterbalo);
                areaSubintervalo = alturaConsiderada*anchuraSubInterbalo;
                areaAcumulada = areaAcumulada + areaSubintervalo;       
            }
            printf("f(%.16lf)=%.16lf\tArearIntervalo:%.12lf\tAreaAcumulada:%.12lf\t\tTask DONE!!\n",a+i*anchuraSubInterbalo,alturaConsiderada,areaSubintervalo, areaAcumulada);
            
            
            
            MPI_Reduce(&areaAcumulada,&globalArea,1,MPI_DOUBLE, MPI_SUM ,0 ,MPI_COMM_WORLD);
 
            //Presentacion Resultados
            if(worldRank == 0)
            {
                 printf("Integracion Numerica Aproximada por trapecios. [A:%lf,B:%lf] Resultado:%lf\n\n",a,b,globalArea);
            }
        
        }
    }
    MPI_Finalize();
    return 0;
}
