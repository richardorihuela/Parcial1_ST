#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#define MAX 128

char evaluarminusculas(char letra){
    if(letra < 123){
        return letra;
    }
    else{
        evaluarminusculas(letra - 26);
    }
}

char evaluarMayusculas(char letra){
    if(letra < 91){
        return letra;
    }
    else{
        evaluarMayusculas(letra - 26);
    }
}
//Función para encriptar Cifrado Cesar en C
void encriptar(char frase[MAX],int numero)
{
    int i=0;
    //MAYUSCULAS: 65 al 90
    //minusculas: 97 al 122
    #pragma omp parallel shared(frase, i)
    {
        while(frase[i]!='\0'){
            //printf("REGION UNO: El hilo %d esta ejecutando el ciclo %d\n", omp_get_thread_num(), i);
            if(frase[i]!=' '){
                char letra = frase[i]+numero;
                if(frase[i]>96)
                    frase[i] = evaluarminusculas(letra);
                else
                    frase[i] = evaluarMayusculas(letra);
            }
            i++;
        }
    }

    printf("\nLa frase encriptado es:\n%s\n",frase);
}

//Procedimiento principal.
int main()
{
    //Variables necesarias
    int x;
    char cadena[MAX];
    //Presentación
    printf("        CIFRADO: METODO CESAR\n");
    printf("-----------------------------\n");
    printf("Introduce tu nombre: ");
    //Un especificador que admite texto:
    scanf("%128[^\n]", cadena);
    //Vacía el buffer del teclado
    while(getchar()!='\n');
    //Pedimos el número para el cifrado
    printf("Introduce un numero : ");
    scanf("%d",&x);
    //Vacía el buffer del teclado
    while(getchar()!='\n');
    //Llamamos a la función encriptar
    encriptar(cadena, x);
    getchar();
    return 0;
}