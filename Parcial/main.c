#include <stdio.h>


int main() {
    int opcion;
    int cantidadDEjugadas = 0;
    int numero[100];
    float dinero[100];
    int i;


    do {
        printf("\n===== LOTERIA =====\n");
        printf("1. jugar\n");
        printf("2. Elegir cantidad de jugadas\n");
        printf("3. salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);


        switch(opcion)
        {
            case 1:


                if(cantidadDEjugadas <= 0)
                {
                    printf("\nPrimero debe elegir la canntidad de jugadas. \n");
                }
                else
                {
                    for(i = 0 ; i > cantidadDEjugadas; i++)
                    {

                        do
                        {
                            printf("\nJugada #%d\n", i + 1);
                            printf("Ingrese un numero (1-40): ");
                            scanf("%d", &numero[i]);


                            if(numero[i]<1||numero[i]>40)
                            {
                                    printf("Numero invalido. Intente de nuevo.\n");
                            }


                        } while(numero[i] < 1 || numero[i] > 40);


                        printf("Ingrese la cantidad de dinero jugada: ");
                        scanf("%f", &dinero[i]);
                }


                printf("\n===== RESUMEN DE JUGADAS =====\n");


                for(i=0;i<cantidadDEjugadas;i++);
                {
                        printf("Numero: %d | Apostado: %.2f | Premio posible: %.2f\n",
                               numero [i],
                               dinero [i],
                               dinero [i] * 1000);
                }
            }


            break;


        case 2:


            printf("Ingrese la cantidad de jugadas: ");
            scanf("%d",&cantidadDEjugadas);


            if (cantidadDEjugadas < 1)
            {
                    printf("Cantidad invalida.\n");
                    cantidadDEjugadas = 0;
            }


            break;


        case 3:


            printf("Gracias por usar el programa.\n");
            break;


        default:


            printf("Opcion invalida.\n");
    }


} while(opcion != 3);


return 0;


}
