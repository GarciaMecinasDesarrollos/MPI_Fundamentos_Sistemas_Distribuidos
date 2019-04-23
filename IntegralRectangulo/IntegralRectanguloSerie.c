/* Solucion Serie para la integracion numeria basada en rectangulos*/
//#include "mpi.h"
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
    

int numeroIntervalos = 0, i = 0;
double a = 0.0, b = 0.0;
double anchuraSubInterbalo = 0.0, areaSubintervalo = 0.0 , areaAcumulada=0.0, alturaConsiderada=0.0;

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
            alturaConsiderada = fdX(a+i*anchuraSubInterbalo);
            areaSubintervalo = alturaConsiderada*anchuraSubInterbalo;
            areaAcumulada = areaAcumulada + areaSubintervalo;                 
        }
        printf("AreaAcumulada:%.12lf\n",areaAcumulada);
        printf("Error Relativo %.16lf/%.16lf = %.16lf\n\n",areaAcumulada,ln4,areaAcumulada/ln4);       
    }
    
    return 0;
}
