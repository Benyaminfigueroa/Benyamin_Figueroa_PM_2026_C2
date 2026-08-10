#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ARCHIVO "vehiculos.dat"
#define MAX_VEHICULOS 100


/* ==============================
   ESTRUCTURA DEL VEHICULO
   ============================== */


typedef struct {
    int id;


    char marca[30];
    char modelo[30];


    /* Datos económicos */
    float precioCompra;


    /* Vida util */
    int vidaUtilAnios;
    float vidaUtilkm;


    /* Mantenimiento total durante la vida util */
    float mantenimientoTotal;


    /* Combustible */
    float consumoCiudad;    // Litros cada 100 km
    float consumoAutopista; // Litros cada 100 km


    /* Neumaticos */
    float costoNeumaticos;
    float vidaNeumaticosKm;


    /* Seguro */
    float seguroAnual;


    /* Kilometros recorridos por año */
    float kmCiudadAnuales;
    float kmAutopistaAnuales;


} Vehiculo;


/* Precio del combustible */
float precioCombustible = 1500.0;


/* ==============================
   FUNCIONES AUXILIARES
   ============================== */


void limpiarBuffer()
{
    int c;


    while ((c = getchar()) != '\n' && c != EOF);
}


void leerCadena(char texto[], int tamanio)
{
    fgets(texto, tamanio, stdin);


    texto[strcspn(texto, "\n")] = '\0';
}


/* ==============================
   BUSCAR VEHICULO
   ============================== */


int buscarVehiculo(int id, Vehiculo *vehiculo)
{
    FILE *archivo;

    archivo = fopen(ARCHIVO, "rb");


    if (archivo == NULL)
        return 0;


    while (fread(vehiculo, sizeof(Vehiculo), 1, archivo) == 1)
    {
        if (vehiculo->id == id)
        {
            fclose(archivo);
            return 1;
        }
    }

    fclose(archivo);

    return 0;
}


/* ==============================
   OBTENER SIGUIENTE ID
   ============================== */

int obtenerSiguienteID()
{
    FILE *archivo;
    Vehiculo v;
    int mayorID = 0;

    archivo = fopen(ARCHIVO, "rb");

    if (archivo == NULL)
        return 1;

    while (fread(&v, sizeof(Vehiculo), 1, archivo) == 1)
    {
        if (v.id > mayorID)
            mayorID = v.id;
    }

    fclose(archivo);

    return mayorID + 1;
}


/* ==============================
   CREAR VEHICULO
   ============================== */

void crearVehiculo()
{
    FILE *archivo;
    Vehiculo v;

    v.id = obtenerSiguienteID();

    printf("\n========== CREAR VEHICULO ==========\n");

    printf("Marca: ");
    limpiarBuffer();
    leerCadena(v.marca, 30);

    printf("Modelo: ");
    leerCadena(v.modelo, 30);

    printf("Precio de compra: $");
    scanf("%f", &v.precioCompra);

    printf("Vida util en anios: ");
    scanf("%d", &v.vidaUtilAnios);

    printf("Vida util en kilometros: ");
    scanf("%f", &v.vidaUtilkm);

    printf("Gasto total de mantenimiento durante la vida util: $");
    scanf("%f", &v.mantenimientoTotal);

    printf("Consumo en ciudad (litros cada 100 km): ");
    scanf("%f", &v.consumoCiudad);

    printf("Consumo en autopista (litros cada 100 km): ");
    scanf("%f", &v.consumoAutopista);

    printf("Costo de los neumaticos: $");
    scanf("%f", &v.costoNeumaticos);

    printf("Vida util de los neumaticos en km: ");
    scanf("%f", &v.vidaNeumaticosKm);

    printf("Seguro anual: $");
    scanf("%f", &v.seguroAnual);

    printf("Kilometros en ciudad por anio: ");
    scanf("%f", &v.kmCiudadAnuales);

    printf("Kilometros en autopista por anio: ");
    scanf("%f", &v.kmAutopistaAnuales);

    archivo = fopen(ARCHIVO, "ab");

    if (archivo == NULL)
    {
        printf("\nError al abrir el archivo.\n");
        return;
    }

    fwrite(&v, sizeof(Vehiculo), 1, archivo);

    fclose(archivo);

    printf("\nVehiculo creado correctamente.");
    printf("\nID asignado: %d\n", v.id);
}


/* ==============================
   LISTAR VEHICULOS
   ============================== */

void listarVehiculos()
{
    FILE *archivo;
    Vehiculo v;

    archivo = fopen(ARCHIVO, "rb");

    if (archivo == NULL)
    {
        printf("\nNo existen vehiculos guardados.\n");
        return;
    }

    printf("\n========== VEHICULOS ==========\n");

    while (fread(&v, sizeof(Vehiculo), 1, archivo) == 1)
    {
        printf("\nID: %d", v.id);
        printf("\nMarca: %s", v.marca);
        printf("\nModelo: %s", v.modelo);
        printf("\nPrecio: $%.2f", v.precioCompra);
        printf("\nVida util: %d anios / %.0f km",
               v.vidaUtilAnios, v.vidaUtilkm);

        printf("\n-------------------------------\n");
    }

    fclose(archivo);
}


/* ==============================
   ELIMINAR VEHICULO
   ============================== */

void eliminarVehiculo()
{
    FILE *archivo;
    FILE *temporal;

    Vehiculo v;
    int id;
    int encontrado = 0;

    printf("\n========== ELIMINAR VEHICULO ==========\n");

    printf("Ingrese el ID del vehiculo: ");
    scanf("%d", &id);

    archivo = fopen(ARCHIVO, "rb");

    if (archivo == NULL)
    {
        printf("\nNo existe el archivo.\n");
        return;
    }

    temporal = fopen("temporal.dat", "wb");

    if (temporal == NULL)
    {
        fclose(archivo);
        printf("\nError al crear archivo temporal.\n");
        return;
    }

    while (fread(&v, sizeof(Vehiculo), 1, archivo) == 1)
    {
        if (v.id == id)
        {
            encontrado = 1;
        }
        else
        {
            fwrite(&v, sizeof(Vehiculo), 1, temporal);
        }
    }

    fclose(archivo);
    fclose(temporal);

    remove(ARCHIVO);
    rename("temporal.dat", ARCHIVO);

    if (encontrado)
        printf("\nVehiculo eliminado correctamente.\n");
    else
        printf("\nNo se encontro el vehiculo.\n");
}


/* ==============================
   MODIFICAR VEHICULO
   ============================== */

void modificarVehiculo()
{
    FILE *archivo;

    Vehiculo vehiculos[MAX_VEHICULOS];
    Vehiculo v;

    int cantidad = 0;
    int id;
    int encontrado = 0;

    printf("\n========== MODIFICAR VEHICULO ==========\n");

    printf("Ingrese el ID del vehiculo: ");
    scanf("%d", &id);

    archivo = fopen(ARCHIVO, "rb");

    if (archivo == NULL)
    {
        printf("\nNo existen vehiculos.\n");
        return;
    }

    while (cantidad < MAX_VEHICULOS &&
           fread(&vehiculos[cantidad],
                 sizeof(Vehiculo),
                 1,
                 archivo) == 1)
    {
        cantidad++;
    }

    fclose(archivo);

    for (int i = 0; i < cantidad; i++)
    {
        if (vehiculos[i].id == id)
        {
            encontrado = 1;

            printf("\nModificando vehiculo %s %s\n",
                   vehiculos[i].marca,
                   vehiculos[i].modelo);

            printf("\nNueva marca: ");
            limpiarBuffer();
            leerCadena(vehiculos[i].marca, 30);

            printf("Nuevo modelo: ");
            leerCadena(vehiculos[i].modelo, 30);

            printf("Nuevo precio de compra: $");
            scanf("%f", &vehiculos[i].precioCompra);

            printf("Nueva vida util en anios: ");
            scanf("%d", &vehiculos[i].vidaUtilAnios);

            printf("Nueva vida util en km: ");
            scanf("%f", &vehiculos[i].vidaUtilkm);

            printf("Nuevo mantenimiento total: $");
            scanf("%f", &vehiculos[i].mantenimientoTotal);

            printf("Nuevo consumo en ciudad (L/100km): ");
            scanf("%f", &vehiculos[i].consumoCiudad);

            printf("Nuevo consumo en autopista (L/100km): ");
            scanf("%f", &vehiculos[i].consumoAutopista);

            printf("Nuevo costo de neumaticos: $");
            scanf("%f", &vehiculos[i].costoNeumaticos);

            printf("Nueva vida de neumaticos en km: ");
            scanf("%f", &vehiculos[i].vidaNeumaticosKm);

            printf("Nuevo seguro anual: $");
            scanf("%f", &vehiculos[i].seguroAnual);

            printf("Nuevos km en ciudad por anio: ");
            scanf("%f", &vehiculos[i].kmCiudadAnuales);

            printf("Nuevos km en autopista por anio: ");
            scanf("%f", &vehiculos[i].kmAutopistaAnuales);

            break;
        }
    }

    if (!encontrado)
    {
        printf("\nNo se encontro el vehiculo.\n");
        return;
    }

    archivo = fopen(ARCHIVO, "wb");

    if (archivo == NULL)
    {
        printf("\nError al guardar los cambios.\n");
        return;
    }

    for (int i = 0; i < cantidad; i++)
    {
        fwrite(&vehiculos[i],
               sizeof(Vehiculo),
               1,
               archivo);
    }

    fclose(archivo);

    printf("\nVehiculo modificado correctamente.\n");
}


/* ==============================
   AMORTIZACION
   ============================== */

float amortizacionKm(Vehiculo v)
{
    if (v.vidaUtilkm <= 0)
        return 0;

    return v.precioCompra / v.vidaUtilkm;
}


/* ==============================
   MANTENIMIENTO POR KM
   ============================== */

float mantenimientoKm(Vehiculo v)
{
    if (v.vidaUtilkm <= 0)
        return 0;

    return v.mantenimientoTotal / v.vidaUtilkm;
}


/* ==============================
   COMBUSTIBLE POR KM
   ============================== */

float combustibleKm(Vehiculo v, int tipo)
{
    float consumo;

    if (tipo == 1)
        consumo = v.consumoCiudad;
    else
        consumo = v.consumoAutopista;

    /*
       Si consume X litros cada 100 km:

       litros por km = X / 100

       costo por km = litros por km * precio combustible
    */

    return (consumo / 100.0) * precioCombustible;
}


/* ==============================
   NEUMATICOS POR KM
   ============================== */

float neumaticosKm(Vehiculo v)
{
    if (v.vidaNeumaticosKm <= 0)
        return 0;

    return v.costoNeumaticos / v.vidaNeumaticosKm;
}


/* ==============================
   SEGURO POR KM
   ============================== */

float seguroKm(Vehiculo v)
{
    float kmAnuales;

    kmAnuales = v.kmCiudadAnuales +
                v.kmAutopistaAnuales;

    if (kmAnuales <= 0)
        return 0;

    return v.seguroAnual / kmAnuales;
}


/* ==============================
   COSTO REAL POR KM
   ============================== */

float costoRealKm(Vehiculo v, int tipo)
{
    float costo;

    costo = 0;

    /* Amortizacion */
    costo += amortizacionKm(v);

    /* Mantenimiento */
    costo += mantenimientoKm(v);

    /* Combustible */
    costo += combustibleKm(v, tipo);

    /* Neumaticos */
    costo += neumaticosKm(v);

    /* Seguro */
    costo += seguroKm(v);

    return costo;
}


/* ==============================
   MOSTRAR COSTOS
   ============================== */

void mostrarCostos()
{
    Vehiculo v;
    int id;

    printf("\n========== COSTOS DEL VEHICULO ==========\n");

    printf("Ingrese ID: ");
    scanf("%d", &id);

    if (!buscarVehiculo(id, &v))
    {
        printf("\nNo se encontro el vehiculo.\n");
        return;
    }

    printf("\nVehiculo: %s %s\n", v.marca, v.modelo);

    printf("\n--- COSTOS POR KM ---\n");

    printf("\nAmortizacion:     $%.2f/km",
           amortizacionKm(v));

    printf("\nMantenimiento:    $%.2f/km",
           mantenimientoKm(v));

    printf("\nNeumaticos:       $%.2f/km",
           neumaticosKm(v));

    printf("\nSeguro:           $%.2f/km",
           seguroKm(v));

    printf("\nCombustible ciudad:     $%.2f/km",
           combustibleKm(v, 1));

    printf("\nCombustible autopista:  $%.2f/km",
           combustibleKm(v, 2));

    printf("\n\n--- COSTO REAL ---\n");

    printf("\nCosto real ciudad:    $%.2f/km",
           costoRealKm(v, 1));

    printf("\nCosto real autopista: $%.2f/km\n",
           costoRealKm(v, 2));
}


/* ==============================
   CALCULAR VIAJE
   ============================== */

void calcularViaje()
{
    Vehiculo v;

    int id;

    float kmCiudad;
    float kmAutopista;

    float costoCombustible;
    float costoTotal;

    printf("\n========== CALCULAR VIAJE ==========\n");

    printf("Ingrese ID del vehiculo: ");
    scanf("%d", &id);

    if (!buscarVehiculo(id, &v))
    {
        printf("\nNo se encontro el vehiculo.\n");
        return;
    }

    printf("\nVehiculo: %s %s\n",
           v.marca,
           v.modelo);

    printf("\nKilometros en ciudad: ");
    scanf("%f", &kmCiudad);

    printf("Kilometros en autopista: ");
    scanf("%f", &kmAutopista);

    /* Costo solamente de combustible */

    costoCombustible =
        kmCiudad * combustibleKm(v, 1) +
        kmAutopista * combustibleKm(v, 2);

    /* Costo real del viaje */

    costoTotal =
        kmCiudad * costoRealKm(v, 1) +
        kmAutopista * costoRealKm(v, 2);

    printf("\n========== RESULTADO DEL VIAJE ==========\n");

    printf("\nKm ciudad: %.2f", kmCiudad);
    printf("\nKm autopista: %.2f", kmAutopista);
    printf("\nKm totales: %.2f",
           kmCiudad + kmAutopista);

    printf("\n\nCosto de combustible: $%.2f",
           costoCombustible);

    printf("\nCosto real total del viaje: $%.2f",
           costoTotal);

    printf("\nCosto real promedio por km: $%.2f\n",
           costoTotal / (kmCiudad + kmAutopista));
}


/* ==============================
   MODIFICAR PRECIO COMBUSTIBLE
   ============================== */

void modificarCombustible()
{
    printf("\n========== PRECIO DEL COMBUSTIBLE ==========\n");

    printf("Precio actual: $%.2f por litro\n",
           precioCombustible);

    printf("Nuevo precio por litro: $");
    scanf("%f", &precioCombustible);

    printf("\nPrecio actualizado correctamente.\n");
}


/* ==============================
   MENU
   ============================== */

void menu()
{
    int opcion;

    do
    {
        printf("\n\n");
        printf("============================================\n");
        printf("       CALCULADOR DE COSTOS DE VEHICULOS\n");
        printf("============================================\n");

        printf("\nPrecio combustible: $%.2f/litro\n",
               precioCombustible);

        printf("\n1. Crear vehiculo");
        printf("\n2. Mostrar vehiculos");
        printf("\n3. Eliminar vehiculo");
        printf("\n4. Modificar vehiculo");
        printf("\n5. Mostrar costos por km");
        printf("\n6. Calcular viaje");
        printf("\n7. Modificar precio de combustible");
        printf("\n0. Salir");

        printf("\n\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                crearVehiculo();
                break;

            case 2:
                listarVehiculos();
                break;

            case 3:
                eliminarVehiculo();
                break;

            case 4:
                modificarVehiculo();
                break;

            case 5:
                mostrarCostos();
                break;

            case 6:
                calcularViaje();
                break;

            case 7:
                modificarCombustible();
                break;

            case 0:
                printf("\nPrograma finalizado.\n");
                break;

            default:
                printf("\nOpcion incorrecta.\n");
        }

    } while (opcion != 0);
}


/* ==============================
   MAIN
   ============================== */

int main()
{
    menu();

    return 0;
}
