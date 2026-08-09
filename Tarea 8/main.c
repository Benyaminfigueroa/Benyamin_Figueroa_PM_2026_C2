#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define CARPETA "imagenes"
#define SALIDA "imagenes/blanco_negro"

#define MAX_ARCHIVOS 100
#define MAX_NOMBRE 260


char archivos[MAX_ARCHIVOS][MAX_NOMBRE];
int cantidad = 0;


/* =====================================================
   1. VERIFICAR LA EXTENSION
   ===================================================== */

int extensionValida(char *nombre)
{
    char *extension;

    extension = strrchr(nombre, '.');

    if (extension == NULL)
    {
        return 0;
    }

    if (_stricmp(extension, ".jpg") == 0 ||
        _stricmp(extension, ".jpeg") == 0 ||
        _stricmp(extension, ".png") == 0 ||
        _stricmp(extension, ".bmp") == 0)
    {
        return 1;
    }

    return 0;
}


/* =====================================================
   2. VERIFICAR EL CONTENIDO DEL ARCHIVO
   ===================================================== */

int verificarFirma(char *nombre, char *extension)
{
    FILE *archivo;
    unsigned char bytes[8];

    archivo = fopen(nombre, "rb");

    if (archivo == NULL)
    {
        return 0;
    }

    /*
       Leemos los primeros 8 bytes del archivo.
    */
    if (fread(bytes, 1, 8, archivo) < 8)
    {
        fclose(archivo);
        return 0;
    }

    fclose(archivo);


    /*
       JPG
       Firma: FF D8 FF
    */
    if (_stricmp(extension, ".jpg") == 0 ||
        _stricmp(extension, ".jpeg") == 0)
    {
        if (bytes[0] == 0xFF &&
            bytes[1] == 0xD8 &&
            bytes[2] == 0xFF)
        {
            return 1;
        }

        return 0;
    }


    /*
       PNG
       Firma: 89 50 4E 47 0D 0A 1A 0A
    */
    if (_stricmp(extension, ".png") == 0)
    {
        if (bytes[0] == 0x89 &&
            bytes[1] == 0x50 &&
            bytes[2] == 0x4E &&
            bytes[3] == 0x47 &&
            bytes[4] == 0x0D &&
            bytes[5] == 0x0A &&
            bytes[6] == 0x1A &&
            bytes[7] == 0x0A)
        {
            return 1;
        }

        return 0;
    }


    /*
       BMP
       Firma: 42 4D
    */
    if (_stricmp(extension, ".bmp") == 0)
    {
        if (bytes[0] == 0x42 &&
            bytes[1] == 0x4D)
        {
            return 1;
        }

        return 0;
    }


    return 0;
}


/* =====================================================
   3. LISTAR ARCHIVOS DE LA CARPETA
   ===================================================== */

void listarArchivos()
{
    WIN32_FIND_DATAA datos;
    HANDLE busqueda;

    char ruta[MAX_NOMBRE];

    sprintf(ruta, "%s\\*.*", CARPETA);

    busqueda = FindFirstFileA(ruta, &datos);

    if (busqueda == INVALID_HANDLE_VALUE)
    {
        printf("ERROR: No se encontro la carpeta '%s'.\n", CARPETA);
        return;
    }

    cantidad = 0;

    do
    {
        /*
           Comprobar que no sea una carpeta.
        */
        if (!(datos.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            /*
               Comprobar que tenga una extension
               de imagen permitida.
            */
            if (extensionValida(datos.cFileName))
            {
                strcpy(archivos[cantidad], datos.cFileName);

                cantidad++;

                if (cantidad >= MAX_ARCHIVOS)
                {
                    break;
                }
            }
        }

    } while (FindNextFileA(busqueda, &datos));

    FindClose(busqueda);
}


/* =====================================================
   4. VALIDAR LA IMAGEN
   ===================================================== */

int validarImagen(char *nombre)
{
    char ruta[MAX_NOMBRE];
    char *extension;

    int ancho;
    int alto;
    int canales;


    sprintf(ruta, "%s/%s", CARPETA, nombre);

    extension = strrchr(nombre, '.');


    printf("\n========================================\n");
    printf("VALIDANDO IMAGEN\n");
    printf("========================================\n");

    printf("Archivo: %s\n", nombre);


    /*
       Comprobar extension.
    */

    if (extension == NULL)
    {
        printf("ERROR: El archivo no tiene extension.\n");
        return 0;
    }


    printf("Extension: %s\n", extension);


    if (!extensionValida(nombre))
    {
        printf("ERROR: Extension no permitida.\n");
        return 0;
    }

    printf("Extension: VALIDA\n");


    /*
       Comprobar que el contenido coincida
       con la extension.
    */

    if (!verificarFirma(ruta, extension))
    {
        printf("ERROR: La extension no coincide");
        printf(" con el contenido del archivo.\n");

        return 0;
    }

    printf("Contenido: COINCIDE CON LA EXTENSION\n");


    /*
       Intentar leer la imagen.
    */

    if (!stbi_info(ruta, &ancho, &alto, &canales))
    {
        printf("ERROR: El archivo no es una imagen valida.\n");

        return 0;
    }


    printf("Imagen: VALIDA\n");
    printf("Ancho: %d pixeles\n", ancho);
    printf("Alto: %d pixeles\n", alto);
    printf("Canales: %d\n", canales);

    printf("========================================\n");

    return 1;
}


/* =====================================================
   5. CONVERTIR A BLANCO Y NEGRO
   ===================================================== */

void convertirBlancoNegro(char *nombre)
{
    char entrada[MAX_NOMBRE];
    char salida[MAX_NOMBRE];

    int ancho;
    int alto;
    int canales;

    int total;
    int i;

    unsigned char *imagen;
    unsigned char *gris;


    sprintf(entrada, "%s/%s", CARPETA, nombre);


    /*
       Cargar la imagen.

       Pedimos que stb_image nos entregue
       siempre 3 canales: rojo, verde y azul.
    */

    imagen = stbi_load(
        entrada,
        &ancho,
        &alto,
        &canales,
        3
    );


    if (imagen == NULL)
    {
        printf("\nERROR: No se pudo cargar la imagen.\n");
        return;
    }


    /*
       Reservar memoria para la nueva imagen.
    */

    total = ancho * alto;

    gris = (unsigned char *)malloc(total);


    if (gris == NULL)
    {
        printf("ERROR: No hay memoria suficiente.\n");

        stbi_image_free(imagen);

        return;
    }


    /*
       Convertir cada pixel a escala de grises.

       Formula:

       gris = 0.299 R + 0.587 G + 0.114 B
    */

    for (i = 0; i < total; i++)
    {
        int rojo;
        int verde;
        int azul;

        rojo = imagen[i * 3];
        verde = imagen[i * 3 + 1];
        azul = imagen[i * 3 + 2];


        gris[i] = (unsigned char)(
            0.299 * rojo +
            0.587 * verde +
            0.114 * azul
        );
    }


    /*
       Crear carpeta de salida.
    */

    CreateDirectoryA(SALIDA, NULL);


    /*
       Nombre del archivo de salida.
    */

    sprintf(
        salida,
        "%s/%s_blanco_negro.png",
        SALIDA,
        nombre
    );


    /*
       Guardar como PNG.
    */

    if (stbi_write_png(
            salida,
            ancho,
            alto,
            1,
            gris,
            ancho))
    {
        printf("\n========================================\n");
        printf("CONVERSION COMPLETADA\n");
        printf("========================================\n");

        printf("Imagen original: %s\n", entrada);

        printf("Imagen nueva: %s\n", salida);
    }
    else
    {
        printf("ERROR: No se pudo guardar la imagen.\n");
    }


    /*
       Liberar memoria.
    */

    free(gris);

    stbi_image_free(imagen);
}


/* =====================================================
   6. PROGRAMA PRINCIPAL
   ===================================================== */

int main()
{
    int opcion;


    printf("\n");
    printf("============================================\n");
    printf("       CONVERSOR DE IMAGENES\n");
    printf("             DR. STONE\n");
    printf("============================================\n");


    /*
       Buscar las imagenes.
    */

    listarArchivos();


    /*
       Comprobar si encontramos imagenes.
    */

    if (cantidad == 0)
    {
        printf("\nNo se encontraron imagenes.\n");

        printf("Coloca archivos JPG, PNG o BMP");
        printf(" dentro de la carpeta 'imagenes'.\n");

        system("pause");

        return 0;
    }


    /*
       Mostrar menu.
    */

    printf("\n");
    printf("IMAGENES DISPONIBLES\n");
    printf("--------------------------------------------\n");


    for (int i = 0; i < cantidad; i++)
    {
        printf("%d. %s\n", i + 1, archivos[i]);
    }


    printf("--------------------------------------------\n");

    printf("0. Salir\n");


    /*
       Pedir seleccion.
    */

    printf("\nSeleccione una imagen: ");

    scanf("%d", &opcion);


    /*
       Salir.
    */

    if (opcion == 0)
    {
        printf("\nPrograma finalizado.\n");

        return 0;
    }


    /*
       Comprobar seleccion.
    */

    if (opcion < 1 || opcion > cantidad)
    {
        printf("\nERROR: Opcion no valida.\n");

        system("pause");

        return 0;
    }


    /*
       Validar la imagen.
    */

    if (validarImagen(archivos[opcion - 1]))
    {
        /*
           Si es valida, convertirla.
        */

        convertirBlancoNegro(archivos[opcion - 1]);
    }
    else
    {
        printf("\nLa imagen no puede ser convertida.\n");
    }


    printf("\n");

    system("pause");

    return 0;
}
