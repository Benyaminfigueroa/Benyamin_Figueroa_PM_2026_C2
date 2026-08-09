#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================================================
   ESTRUCTURA ALUMNO
   ========================================================= */

typedef struct
{
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[50];

} Alumno;


/* =========================================================
   DATOS PARA CREAR LOS ARCHIVOS
   ========================================================= */

Alumno datosIniciales[10] =
{
    {"Juan", "Lopez", 8.5, "Programacion"},
    {"Ana", "Martinez", 9.2, "Matematicas"},
    {"Carlos", "Hernandez", 7.8, "Fisica"},
    {"Sofia", "Garcia", 9.5, "Programacion"},
    {"Pedro", "Ramirez", 8.0, "Ingles"},
    {"Laura", "Torres", 9.0, "Matematicas"},
    {"Diego", "Flores", 7.5, "Fisica"},
    {"Maria", "Castro", 8.8, "Programacion"},
    {"Luis", "Morales", 8.3, "Ingles"},
    {"Elena", "Vargas", 9.7, "Matematicas"}
};


/* =========================================================
   FUNCION PARA CREAR ARCHIVO XML
   ========================================================= */

void crearXML()
{
    FILE *archivo;
    int i;

    archivo = fopen("alumnos.xml", "w");

    if (archivo == NULL)
    {
        printf("Error al crear alumnos.xml\n");
        return;
    }

    fprintf(archivo, "<Alumnos>\n");

    for (i = 0; i < 10; i++)
    {
        fprintf(archivo, "    <Alumno>\n");

        fprintf(archivo, "        <Nombre>%s</Nombre>\n",
                datosIniciales[i].nombre);

        fprintf(archivo, "        <Apellido>%s</Apellido>\n",
                datosIniciales[i].apellido);

        fprintf(archivo, "        <Promedio>%.2f</Promedio>\n",
                datosIniciales[i].promedio);

        fprintf(archivo, "        <Materia>%s</Materia>\n",
                datosIniciales[i].materia);

        fprintf(archivo, "    </Alumno>\n");
    }

    fprintf(archivo, "</Alumnos>\n");

    fclose(archivo);

    printf("\nArchivo alumnos.xml creado correctamente.\n");
}


/* =========================================================
   FUNCION PARA CREAR ARCHIVO JSON
   ========================================================= */

void crearJSON()
{
    FILE *archivo;
    int i;

    archivo = fopen("alumnos.json", "w");

    if (archivo == NULL)
    {
        printf("Error al crear alumnos.json\n");
        return;
    }

    fprintf(archivo, "[\n");

    for (i = 0; i < 10; i++)
    {
        fprintf(archivo, "    {\n");

        fprintf(archivo, "        \"Nombre\": \"%s\",\n",
                datosIniciales[i].nombre);

        fprintf(archivo, "        \"Apellido\": \"%s\",\n",
                datosIniciales[i].apellido);

        fprintf(archivo, "        \"Promedio\": %.2f,\n",
                datosIniciales[i].promedio);

        fprintf(archivo, "        \"Materia\": \"%s\"\n",
                datosIniciales[i].materia);

        if (i < 9)
            fprintf(archivo, "    },\n");
        else
            fprintf(archivo, "    }\n");
    }

    fprintf(archivo, "]\n");

    fclose(archivo);

    printf("\nArchivo alumnos.json creado correctamente.\n");
}


/* =========================================================
   FUNCION PARA CREAR ARCHIVO CSV
   ========================================================= */

void crearCSV()
{
    FILE *archivo;
    int i;

    archivo = fopen("alumnos.csv", "w");

    if (archivo == NULL)
    {
        printf("Error al crear alumnos.csv\n");
        return;
    }

    /* Encabezado */
    fprintf(archivo, "Nombre,Apellido,Promedio,Materia\n");

    for (i = 0; i < 10; i++)
    {
        fprintf(archivo, "%s,%s,%.2f,%s\n",
                datosIniciales[i].nombre,
                datosIniciales[i].apellido,
                datosIniciales[i].promedio,
                datosIniciales[i].materia);
    }

    fclose(archivo);

    printf("\nArchivo alumnos.csv creado correctamente.\n");
}


/* =========================================================
   CREAR LOS 3 ARCHIVOS
   ========================================================= */

void crearArchivos()
{
    crearXML();
    crearJSON();
    crearCSV();

    printf("\nLos tres archivos fueron creados.\n");
}


/* =========================================================
   MOSTRAR ALUMNOS
   ========================================================= */

void mostrarAlumnos(Alumno *alumnos, int cantidad)
{
    int i;

    printf("\n========================================\n");
    printf("           LISTA DE ALUMNOS\n");
    printf("========================================\n");

    for (i = 0; i < cantidad; i++)
    {
        printf("\nAlumno %d\n", i + 1);
        printf("Nombre: %s\n", alumnos[i].nombre);
        printf("Apellido: %s\n", alumnos[i].apellido);
        printf("Promedio: %.2f\n", alumnos[i].promedio);
        printf("Materia: %s\n", alumnos[i].materia);
    }

    printf("\n========================================\n");
}


/* =========================================================
   OBTENER DATO DE XML
   ========================================================= */

void obtenerDatoXML(char *linea,
                    char *inicio,
                    char *fin,
                    char *resultado)
{
    char *p1;
    char *p2;
    int longitud;

    p1 = strstr(linea, inicio);

    if (p1 != NULL)
    {
        p1 = p1 + strlen(inicio);

        p2 = strstr(p1, fin);

        if (p2 != NULL)
        {
            longitud = p2 - p1;

            strncpy(resultado, p1, longitud);

            resultado[longitud] = '\0';
        }
    }
}


/* =========================================================
   LEER XML
   ========================================================= */

void leerXML()
{
    FILE *archivo;

    char linea[200];

    int cantidad = 0;
    int i = 0;

    Alumno *alumnos;

    printf("\n========== LECTURA XML ==========\n");

    archivo = fopen("alumnos.xml", "r");

    if (archivo == NULL)
    {
        printf("No existe alumnos.xml\n");
        printf("Primero selecciona la opcion 1.\n");
        return;
    }

    /* -----------------------------------------
       PRIMER PASO:
       CONTAR LOS ELEMENTOS
       ----------------------------------------- */

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        if (strstr(linea, "<Alumno>") != NULL)
        {
            cantidad++;
        }
    }

    printf("Cantidad de elementos encontrados: %d\n",
           cantidad);

    if (cantidad <= 0)
    {
        printf("No hay alumnos.\n");
        fclose(archivo);
        return;
    }

    /* -----------------------------------------
       SEGUNDO PASO:
       RESERVAR MEMORIA DINAMICA
       ----------------------------------------- */

    alumnos = (Alumno *)malloc(cantidad * sizeof(Alumno));

    if (alumnos == NULL)
    {
        printf("No se pudo reservar memoria.\n");
        fclose(archivo);
        return;
    }

    printf("Memoria reservada correctamente.\n");

    /* Regresar al principio del archivo */
    rewind(archivo);

    /* -----------------------------------------
       TERCER PASO:
       CARGAR LOS DATOS EN MEMORIA
       ----------------------------------------- */

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        if (strstr(linea, "<Nombre>") != NULL)
        {
            obtenerDatoXML(linea,
                           "<Nombre>",
                           "</Nombre>",
                           alumnos[i].nombre);
        }

        else if (strstr(linea, "<Apellido>") != NULL)
        {
            obtenerDatoXML(linea,
                           "<Apellido>",
                           "</Apellido>",
                           alumnos[i].apellido);
        }

        else if (strstr(linea, "<Promedio>") != NULL)
        {
            char temporal[20];

            obtenerDatoXML(linea,
                           "<Promedio>",
                           "</Promedio>",
                           temporal);

            alumnos[i].promedio = atof(temporal);
        }

        else if (strstr(linea, "<Materia>") != NULL)
        {
            obtenerDatoXML(linea,
                           "<Materia>",
                           "</Materia>",
                           alumnos[i].materia);

            i++;
        }
    }

    fclose(archivo);

    /* -----------------------------------------
       CUARTO PASO:
       IMPRIMIR
       ----------------------------------------- */

    mostrarAlumnos(alumnos, cantidad);

    /* -----------------------------------------
       QUINTO PASO:
       LIBERAR MEMORIA
       ----------------------------------------- */

    free(alumnos);

    printf("\nMemoria liberada correctamente.\n");
}


/* =========================================================
   LEER JSON
   ========================================================= */

void leerJSON()
{
    FILE *archivo;

    char linea[200];

    int cantidad = 0;
    int i = 0;

    Alumno *alumnos;

    printf("\n========== LECTURA JSON ==========\n");

    archivo = fopen("alumnos.json", "r");

    if (archivo == NULL)
    {
        printf("No existe alumnos.json\n");
        printf("Primero selecciona la opcion 1.\n");
        return;
    }

    /* -----------------------------------------
       PRIMER PASO:
       CONTAR LOS ELEMENTOS
       ----------------------------------------- */

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        if (strstr(linea, "\"Nombre\"") != NULL)
        {
            cantidad++;
        }
    }

    printf("Cantidad de elementos encontrados: %d\n",
           cantidad);

    if (cantidad <= 0)
    {
        printf("No hay alumnos.\n");
        fclose(archivo);
        return;
    }

    /* -----------------------------------------
       SEGUNDO PASO:
       RESERVAR MEMORIA
       ----------------------------------------- */

    alumnos = (Alumno *)malloc(cantidad * sizeof(Alumno));

    if (alumnos == NULL)
    {
        printf("No se pudo reservar memoria.\n");
        fclose(archivo);
        return;
    }

    printf("Memoria reservada correctamente.\n");

    rewind(archivo);

    /* -----------------------------------------
       TERCER PASO:
       LEER LOS DATOS
       ----------------------------------------- */

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        if (strstr(linea, "\"Nombre\"") != NULL)
        {
            sscanf(linea,
                   " \"Nombre\": \"%49[^\"]\"",
                   alumnos[i].nombre);
        }

        else if (strstr(linea, "\"Apellido\"") != NULL)
        {
            sscanf(linea,
                   " \"Apellido\": \"%49[^\"]\"",
                   alumnos[i].apellido);
        }

        else if (strstr(linea, "\"Promedio\"") != NULL)
        {
            sscanf(linea,
                   " \"Promedio\": %f",
                   &alumnos[i].promedio);
        }

        else if (strstr(linea, "\"Materia\"") != NULL)
        {
            sscanf(linea,
                   " \"Materia\": \"%49[^\"]\"",
                   alumnos[i].materia);

            i++;
        }
    }

    fclose(archivo);

    /* -----------------------------------------
       CUARTO PASO:
       IMPRIMIR
       ----------------------------------------- */

    mostrarAlumnos(alumnos, cantidad);

    /* -----------------------------------------
       QUINTO PASO:
       LIBERAR MEMORIA
       ----------------------------------------- */

    free(alumnos);

    printf("\nMemoria liberada correctamente.\n");
}


/* =========================================================
   LEER CSV
   ========================================================= */

void leerCSV()
{
    FILE *archivo;

    char linea[200];

    int cantidad = 0;
    int i = 0;

    Alumno *alumnos;

    printf("\n========== LECTURA CSV ==========\n");

    archivo = fopen("alumnos.csv", "r");

    if (archivo == NULL)
    {
        printf("No existe alumnos.csv\n");
        printf("Primero selecciona la opcion 1.\n");
        return;
    }

    /* -----------------------------------------
       SALTAR ENCABEZADO
       ----------------------------------------- */

    fgets(linea, sizeof(linea), archivo);

    /* -----------------------------------------
       PRIMER PASO:
       CONTAR ELEMENTOS
       ----------------------------------------- */

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        if (strlen(linea) > 1)
        {
            cantidad++;
        }
    }

    printf("Cantidad de elementos encontrados: %d\n",
           cantidad);

    if (cantidad <= 0)
    {
        printf("No hay alumnos.\n");
        fclose(archivo);
        return;
    }

    /* -----------------------------------------
       SEGUNDO PASO:
       RESERVAR MEMORIA
       ----------------------------------------- */

    alumnos = (Alumno *)malloc(cantidad * sizeof(Alumno));

    if (alumnos == NULL)
    {
        printf("No se pudo reservar memoria.\n");
        fclose(archivo);
        return;
    }

    printf("Memoria reservada correctamente.\n");

    rewind(archivo);

    /* Saltar nuevamente el encabezado */
    fgets(linea, sizeof(linea), archivo);

    /* -----------------------------------------
       TERCER PASO:
       LEER LOS DATOS
       ----------------------------------------- */

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        sscanf(linea,
               "%49[^,],%49[^,],%f,%49[^\n]",
               alumnos[i].nombre,
               alumnos[i].apellido,
               &alumnos[i].promedio,
               alumnos[i].materia);

        i++;
    }

    fclose(archivo);

    /* -----------------------------------------
       CUARTO PASO:
       IMPRIMIR
       ----------------------------------------- */

    mostrarAlumnos(alumnos, cantidad);

    /* -----------------------------------------
       QUINTO PASO:
       LIBERAR MEMORIA
       ----------------------------------------- */

    free(alumnos);

    printf("\nMemoria liberada correctamente.\n");
}


/* =========================================================
   MENU PRINCIPAL
   ========================================================= */

int main()
{
    int opcion;

    do
    {
        printf("\n\n");
        printf("========================================\n");
        printf("       SISTEMA DE ARCHIVOS ALUMNOS\n");
        printf("========================================\n");

        printf("1. Crear XML, JSON y CSV\n");
        printf("2. Leer archivo XML\n");
        printf("3. Leer archivo JSON\n");
        printf("4. Leer archivo CSV\n");
        printf("5. Leer los tres archivos\n");
        printf("6. Salir\n");

        printf("\nSelecciona una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                crearArchivos();
                break;

            case 2:
                leerXML();
                break;

            case 3:
                leerJSON();
                break;

            case 4:
                leerCSV();
                break;

            case 5:
                leerXML();
                leerJSON();
                leerCSV();
                break;

            case 6:
                printf("\nPrograma terminado.\n");
                break;

            default:
                printf("\nOpcion no valida.\n");
        }

    } while (opcion != 6);

    return 0;
}
