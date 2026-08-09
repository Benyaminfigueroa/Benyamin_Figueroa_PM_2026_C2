#include <stdio.h>

int movimiento = 0;

void TorresHanoi(int discos, char origen, char auxiliar, char destino, FILE *archivo)
{
    if (discos == 1)
    {
        movimiento++;

        printf("Movimiento %d: Disco 1 de %c a %c\n",
               movimiento, origen, destino);

        fprintf(archivo, "Movimiento %d: Disco 1 de %c a %c\n",
                movimiento, origen, destino);
    }
    else
    {
        // Mover los discos de origen a auxiliar
        TorresHanoi(discos - 1, origen, destino, auxiliar, archivo);

        // Mover el disco mas grande a destino
        movimiento++;

        printf("Movimiento %d: Disco %d de %c a %c\n",
               movimiento, discos, origen, destino);

        fprintf(archivo, "Movimiento %d: Disco %d de %c a %c\n",
                movimiento, discos, origen, destino);

        // Mover los discos de auxiliar a destino
        TorresHanoi(discos - 1, auxiliar, origen, destino, archivo);
    }
}

int main()
{
    int discos;
    FILE *archivo;

    printf("=================================\n");
    printf("       TORRES DE HANOI\n");
    printf("=================================\n\n");

    printf("Ingrese el numero de discos: ");
    scanf("%d", &discos);

    if (discos <= 0)
    {
        printf("El numero de discos debe ser mayor que 0.\n");
        return 1;
    }

    // Crear el archivo
    archivo = fopen("movimientos.txt", "w");

    if (archivo == NULL)
    {
        printf("No se pudo crear el archivo.\n");
        return 1;
    }

    printf("\n--- MOVIMIENTOS ---\n\n");

    // Resolver las Torres de Hanoi
    TorresHanoi(discos, 'A', 'B', 'C', archivo);

    // Cerrar el archivo
    fclose(archivo);

    printf("\n=================================\n");
    printf("Total de movimientos: %d\n", movimiento);
    printf("Los movimientos se guardaron en movimientos.txt\n");
    printf("=================================\n");

    return 0;
}
