//La Ecuacion
//A X = b
//Hallamos Descomponemos A
//A = (Diagonal Principal) + ((triangular inferior) + (triangular superior))
//A = D + (L + U)

//R = L + U
//Rescribimos la ecuacion
//DX + RX = b
//DX = b - RX
//X = (1/D) (b - R X)
//X(k +1) = (1/D) (b - R X(k))






#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

/*
M-->matriz aumentada de sistema lineal
n-->numero de ecuaciones
*/
/*
*modificar y luego saber si la matriz es daiagonalmente dominante
*si la matriz es diagonalmente dominante se asegura la convergencia del método
*/
int convergencia(float M[100][100],int n){
    double may;//variable para almacenar el mayor de la columna k
    int ind;//indice del mayor-->indice de may
    int band=1;
    double aux;
    double acum;
    
    for(int k=0;k<n;k++){//recorrer columnas de la matriz reducida
        may=abs(M[k][k]);//se inicializa may con el primer elemento de la columna
        ind=k;
        //recorrer la columna k para buscar el indice del mayor
        //(usamos un parallel for)
        #pragma omp parallel for shared(n, may)
            for(int l=k+1;l<n;l++){
                if(may<abs(M[l][k])){
                    #pragma omp critical (update_may)
                    {
                        may=abs(M[l][k]);
                        ind=l;
                    }
                }            
            }
        //cambiar filas
        if(k!=ind){//asegurar que no se trata de la misma fila
            for(int i=0;i<n+1;i++){
                aux=M[k][i];
                M[k][i]=M[ind][i];
                M[ind][i]=aux;
            }
        }

        //verificar convergencia (usamos un for y un critical)
        acum=0;
        #pragma omp parallel for shared(n, acum)
            for(int j=0;j<n;j++){
                if(k!=j){
                    #pragma omp critical (update_acum)
                    {
                        acum=acum+abs(M[k][j]);
                    }
                }
            }
        if(acum>abs(M[k][k])){//no se trata de una matriz diagonalmente dominante
            band=0;
            break;//termina el primer ciclo for
        }
    }
    
    return band;
}


/*
M-->matriz aumentada modificada en la funcion convergencia(matriz diagonalmente dominante)
V-->vector de la solución(inicializada con los punto iniciales de las variables)
n-->numero de ecuaciones
tol-->tolerancia para encontrar la solucion
*/ 
int jacobi(float M[100][100],double V[100],int n,float tol){
    double error=0;
    double acum=0;
    double VA[100];//vector solucion de la iteracion anterior
    int iter=0;//número de iteraciones
    do{
        iter++;
        
        //recorrer diagonal de matriz disminuida
        for(int k=0; k<n; k++){
            acum=M[k][n];
            //recorrer la fila k de la matriz disminuida (for)
            #pragma omp for
                for(int j=0; j<n; j++){
                    if(k!=j){
                        #pragma omp critical (update_acum)
                        {
                            acum=acum-VA[j]*M[k][j];
                        }
                    }
                
                }
            V[k]=acum/M[k][k];
        }
        
        //error
        #pragma omp parallel shared(n)
        {
            acum=0;
            #pragma omp for
                for(int i=0;i<n;i++){
                    error=V[i]-VA[i];
                    #pragma omp critical (update_acum)
                    {
                        acum=acum+pow(error,2);
                    }
                }
            error=sqrt(acum);
            
            //preparando VA para la proxima iteracion
            #pragma omp for
                for(int i=0;i<n;i++){
                    VA[i]=V[i];
                }
        }
    }while(error>tol);
    return iter;
}

int main () {
    float M[100][100];
    int n;
    int band;
    double V[100];
    float tol;
    printf("ingrese el numero de ecuaciones:\n");
    scanf("%d", &n);
    printf("ingrese elementos de la matriz aumentada:\n");
    for(int i=0;i<n;i++){
        printf("fila %d\n", i+1);
        for(int j=0;j<n+1;j++){
            printf("\tcolumna %d :", j+1);
            scanf("%f", &M[i][j]);
        }
    }
    
    printf("matriz aumentada:\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n+1;j++){
            printf("%.2f\t", M[i][j]);
        }printf("\n");
    }

    band=convergencia(M,n);
    if(band == 1){
        
        printf("se garantiza la convergencia\n");
        printf("matriz diagonalmente dominante:\n");
        for(int i=0;i<n;i++){
            for(int j=0;j<n+1;j++){
                printf("%.2f\t", M[i][j]);
            }printf("\n");
        }
        if(band == 1){
            printf("ingrese tolerancia: ");
            scanf("%f", &tol);
            
            printf("-->iteraciones: %d\n", jacobi(M,V,n,tol));
            printf("la solucion es:\n");
            for(int i=0;i<n;i++){
                printf("-->X%d = %.5f\n", i+1, V[i]);
            }
        }    
        
        
        
    }
    else
        printf("no se garantiza convergencia-->la matriz no es diagonalmente dominante\n");
    return 0;
}