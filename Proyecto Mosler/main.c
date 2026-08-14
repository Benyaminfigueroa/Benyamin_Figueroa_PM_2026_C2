#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOMBRE 100
#define MAX_DESC 300
#define MAX_ARCHIVO 260

typedef struct {
    int id;
    char nombre[MAX_NOMBRE];
    char descripcion[MAX_DESC];

    int F;  /* Función */
    int S;  /* Sustitución */
    int P;  /* Profundidad */
    int E;  /* Extensión */
    int A;  /* Agresión */
    int V;  /* Vulnerabilidad */

    int I;      /* Importancia */
    int D;      /* Daños */
    int C;      /* Criterio */
    int PR;     /* Resultado de agresión */
    int ER;     /* Evaluación del riesgo */

    char nivel[30];
} Riesgo;

typedef struct {
    Riesgo *datos;
    size_t cantidad;
    size_t capacidad;
} ListaRiesgos;

/* ---------- Utilidades ---------- */

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void leer_linea(const char *mensaje, char *destino, size_t tam) {
    printf("%s", mensaje);
    if (fgets(destino, (int)tam, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    destino[strcspn(destino, "\n")] = '\0';
}

static int leer_entero(const char *mensaje, int minimo, int maximo) {
    int valor;
    int ok;

    do {
        printf("%s", mensaje);
        ok = scanf("%d", &valor);
        limpiar_buffer();

        if (ok != 1 || valor < minimo || valor > maximo) {
            printf("Entrada no valida. Escriba un numero entre %d y %d.\n",
                   minimo, maximo);
        }
    } while (ok != 1 || valor < minimo || valor > maximo);

    return valor;
}

static void pausa(void) {
    char tmp[8];
    printf("\nPresione ENTER para continuar...");
    fgets(tmp, sizeof(tmp), stdin);
}

/* ---------- Memoria dinámica ---------- */

static void inicializar_lista(ListaRiesgos *lista) {
    lista->datos = NULL;
    lista->cantidad = 0;
    lista->capacidad = 0;
}

static int asegurar_capacidad(ListaRiesgos *lista) {
    if (lista->cantidad < lista->capacidad) {
        return 1;
    }

    size_t nueva_capacidad = (lista->capacidad == 0)
                           ? 4
                           : lista->capacidad * 2;

    Riesgo *nuevo = realloc(lista->datos,
                            nueva_capacidad * sizeof(Riesgo));

    if (nuevo == NULL) {
        printf("Error: no se pudo reservar memoria.\n");
        return 0;
    }

    lista->datos = nuevo;
    lista->capacidad = nueva_capacidad;
    return 1;
}

static void liberar_lista(ListaRiesgos *lista) {
    free(lista->datos);
    lista->datos = NULL;
    lista->cantidad = 0;
    lista->capacidad = 0;
}

/* ---------- Método Mosler ---------- */

static void clasificar_riesgo(Riesgo *r) {
    if (r->ER <= 250)
        strcpy(r->nivel, "MUY PEQUENO");
    else if (r->ER <= 500)
        strcpy(r->nivel, "PEQUENO");
    else if (r->ER <= 750)
        strcpy(r->nivel, "NORMAL");
    else if (r->ER <= 1000)
        strcpy(r->nivel, "GRANDE");
    else
        strcpy(r->nivel, "MUY GRANDE");
}

static void calcular_mosler(Riesgo *r) {
    r->I = r->F * r->S;
    r->D = r->P * r->E;
    r->C = r->I + r->D;
    r->PR = r->A * r->V;
    r->ER = r->C * r->PR;
    clasificar_riesgo(r);
}

/*
 * "IA" local:
 * No es una IA generativa ni un modelo entrenado. Es un modulo
 * de recomendacion automatica basado en reglas, incluido para
 * demostrar el uso de una funcion inteligente sin necesitar
 * una API externa ni una clave secreta.
 */
static void recomendacion_ia(const Riesgo *r) {
    printf("\n--- ANALISIS IA LOCAL ---\n");

    if (r->ER > 1000) {
        printf("Prioridad: CRITICA.\n");
        printf("Recomendacion: preparar un plan de emergencia para terremotos, "
               "simulacros, rutas de evacuacion, puntos de reunion y respaldo "
               "de operaciones.\n");
    } else if (r->ER > 750) {
        printf("Prioridad: ALTA.\n");
        printf("Recomendacion: reforzar las medidas de prevencion y realizar "
               "una revision de infraestructura y del plan de emergencia.\n");
    } else if (r->ER > 500) {
        printf("Prioridad: MEDIA.\n");
        printf("Recomendacion: mantener controles, capacitar al personal y "
               "revisar periodicamente la vulnerabilidad.\n");
    } else {
        printf("Prioridad: BAJA.\n");
        printf("Recomendacion: conservar las medidas actuales y monitorear "
               "cambios en el riesgo.\n");
    }

    if (r->V >= 4)
        printf("Observacion IA: la vulnerabilidad es alta; conviene priorizarla.\n");
    if (r->A >= 4)
        printf("Observacion IA: la agresion/exposicion es alta; revisar controles.\n");
}

/* ---------- Riesgos ---------- */

static void mostrar_riesgo(const Riesgo *r) {
    printf("\nID: %d\n", r->id);
    printf("Nombre: %s\n", r->nombre);
    printf("Descripcion: %s\n", r->descripcion);

    printf("\nParametros Mosler (1-5):\n");
    printf("  F Funcion        = %d\n", r->F);
    printf("  S Sustitucion    = %d\n", r->S);
    printf("  P Profundidad    = %d\n", r->P);
    printf("  E Extension      = %d\n", r->E);
    printf("  A Agresion       = %d\n", r->A);
    printf("  V Vulnerabilidad = %d\n", r->V);

    printf("\nCalculo:\n");
    printf("  I  = F*S   = %d\n", r->I);
    printf("  D  = P*E   = %d\n", r->D);
    printf("  C  = I+D   = %d\n", r->C);
    printf("  PR = A*V   = %d\n", r->PR);
    printf("  ER = C*PR  = %d\n", r->ER);
    printf("  NIVEL     = %s\n", r->nivel);
}

static int buscar_indice(const ListaRiesgos *lista, int id) {
    for (size_t i = 0; i < lista->cantidad; ++i) {
        if (lista->datos[i].id == id)
            return (int)i;
    }
    return -1;
}

static int siguiente_id(const ListaRiesgos *lista) {
    int mayor = 0;
    for (size_t i = 0; i < lista->cantidad; ++i) {
        if (lista->datos[i].id > mayor)
            mayor = lista->datos[i].id;
    }
    return mayor + 1;
}

static void crear_riesgo(ListaRiesgos *lista) {
    if (!asegurar_capacidad(lista))
        return;

    Riesgo r;
    memset(&r, 0, sizeof(r));
    r.id = siguiente_id(lista);

    printf("\n--- CREAR RIESGO ---\n");
    leer_linea("Nombre del riesgo: ", r.nombre, sizeof(r.nombre));
    leer_linea("Descripcion: ", r.descripcion, sizeof(r.descripcion));

    printf("\nIngrese los valores Mosler de 1 a 5.\n");
    printf("1 = bajo impacto/efecto y 5 = alto impacto/efecto.\n");

    r.F = leer_entero("F - Funcion: ", 1, 5);
    r.S = leer_entero("S - Sustitucion: ", 1, 5);
    r.P = leer_entero("P - Profundidad: ", 1, 5);
    r.E = leer_entero("E - Extension: ", 1, 5);
    r.A = leer_entero("A - Agresion: ", 1, 5);
    r.V = leer_entero("V - Vulnerabilidad: ", 1, 5);

    calcular_mosler(&r);
    lista->datos[lista->cantidad++] = r;

    printf("\nRiesgo creado correctamente.\n");
    mostrar_riesgo(&lista->datos[lista->cantidad - 1]);
    recomendacion_ia(&lista->datos[lista->cantidad - 1]);
}

static void modificar_riesgo(ListaRiesgos *lista) {
    if (lista->cantidad == 0) {
        printf("No hay riesgos cargados.\n");
        return;
    }

    int id = leer_entero("ID del riesgo a modificar: ", 1, 1000000);
    int idx = buscar_indice(lista, id);

    if (idx < 0) {
        printf("No existe un riesgo con ese ID.\n");
        return;
    }

    Riesgo *r = &lista->datos[idx];

    printf("\n--- MODIFICAR RIESGO %d ---\n", id);
    leer_linea("Nuevo nombre: ", r->nombre, sizeof(r->nombre));
    leer_linea("Nueva descripcion: ", r->descripcion, sizeof(r->descripcion));

    r->F = leer_entero("F - Funcion: ", 1, 5);
    r->S = leer_entero("S - Sustitucion: ", 1, 5);
    r->P = leer_entero("P - Profundidad: ", 1, 5);
    r->E = leer_entero("E - Extension: ", 1, 5);
    r->A = leer_entero("A - Agresion: ", 1, 5);
    r->V = leer_entero("V - Vulnerabilidad: ", 1, 5);

    calcular_mosler(r);
    printf("\nRiesgo modificado.\n");
    mostrar_riesgo(r);
}

static void borrar_riesgo(ListaRiesgos *lista) {
    if (lista->cantidad == 0) {
        printf("No hay riesgos cargados.\n");
        return;
    }

    int id = leer_entero("ID del riesgo a borrar: ", 1, 1000000);
    int idx = buscar_indice(lista, id);

    if (idx < 0) {
        printf("No existe un riesgo con ese ID.\n");
        return;
    }

    for (size_t i = (size_t)idx; i + 1 < lista->cantidad; ++i)
        lista->datos[i] = lista->datos[i + 1];

    lista->cantidad--;
    printf("Riesgo borrado correctamente.\n");
}

static void imprimir_riesgos(const ListaRiesgos *lista) {
    if (lista->cantidad == 0) {
        printf("\nNo hay riesgos cargados.\n");
        return;
    }

    printf("\n================ LISTADO DE RIESGOS ================\n");

    for (size_t i = 0; i < lista->cantidad; ++i) {
        const Riesgo *r = &lista->datos[i];

        printf("\n[%d] %-25s | ER=%-4d | %-11s\n",
               r->id, r->nombre, r->ER, r->nivel);
        printf("    F=%d S=%d P=%d E=%d A=%d V=%d\n",
               r->F, r->S, r->P, r->E, r->A, r->V);
    }

    printf("\n=====================================================\n");
}

/* ---------- Archivos ---------- */

static void crear_archivo_especifico(void) {
    char nombre[MAX_ARCHIVO];
    FILE *f;

    leer_linea("Nombre del archivo a crear (ej. terremotos.txt): ",
               nombre, sizeof(nombre));

    f = fopen(nombre, "w");
    if (f == NULL) {
        perror("No se pudo crear el archivo");
        return;
    }

    fprintf(f, "ARCHIVO DE RIESGOS - METODO MOSLER\n");
    fprintf(f, "Archivo creado por el programa.\n");
    fclose(f);

    printf("Archivo '%s' creado correctamente.\n", nombre);
}

static int guardar_txt(const ListaRiesgos *lista, const char *nombre) {
    FILE *f = fopen(nombre, "w");
    if (!f) return 0;

    fprintf(f, "RIESGOS - METODO MOSLER\n\n");

    for (size_t i = 0; i < lista->cantidad; ++i) {
        const Riesgo *r = &lista->datos[i];

        fprintf(f, "ID: %d\n", r->id);
        fprintf(f, "Nombre: %s\n", r->nombre);
        fprintf(f, "Descripcion: %s\n", r->descripcion);
        fprintf(f, "F=%d S=%d P=%d E=%d A=%d V=%d\n",
                r->F, r->S, r->P, r->E, r->A, r->V);
        fprintf(f, "I=%d D=%d C=%d PR=%d ER=%d\n",
                r->I, r->D, r->C, r->PR, r->ER);
        fprintf(f, "Nivel: %s\n\n", r->nivel);
    }

    fclose(f);
    return 1;
}

static int guardar_csv(const ListaRiesgos *lista, const char *nombre) {
    FILE *f = fopen(nombre, "w");
    if (!f) return 0;

    fprintf(f, "ID,Nombre,Descripcion,F,S,P,E,A,V,I,D,C,PR,ER,Nivel\n");

    for (size_t i = 0; i < lista->cantidad; ++i) {
        const Riesgo *r = &lista->datos[i];

        /* Para un CSV simple, se reemplazan comas en textos. */
        char nom[MAX_NOMBRE], desc[MAX_DESC];
        strncpy(nom, r->nombre, sizeof(nom));
        nom[sizeof(nom) - 1] = '\0';
        strncpy(desc, r->descripcion, sizeof(desc));
        desc[sizeof(desc) - 1] = '\0';

        for (char *p = nom; *p; ++p)
            if (*p == ',') *p = ';';
        for (char *p = desc; *p; ++p)
            if (*p == ',') *p = ';';

        fprintf(f, "%d,\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\"\n",
                r->id, nom, desc,
                r->F, r->S, r->P, r->E, r->A, r->V,
                r->I, r->D, r->C, r->PR, r->ER, r->nivel);
    }

    fclose(f);
    return 1;
}

static int guardar_json(const ListaRiesgos *lista, const char *nombre) {
    FILE *f = fopen(nombre, "w");
    if (!f) return 0;

    fprintf(f, "{\n  \"riesgos\": [\n");

    for (size_t i = 0; i < lista->cantidad; ++i) {
        const Riesgo *r = &lista->datos[i];

        fprintf(f,
                "    {\"id\":%d,\"nombre\":\"%s\",\"descripcion\":\"%s\","
                "\"F\":%d,\"S\":%d,\"P\":%d,\"E\":%d,\"A\":%d,\"V\":%d,"
                "\"I\":%d,\"D\":%d,\"C\":%d,\"PR\":%d,\"ER\":%d,"
                "\"nivel\":\"%s\"}%s\n",
                r->id, r->nombre, r->descripcion,
                r->F, r->S, r->P, r->E, r->A, r->V,
                r->I, r->D, r->C, r->PR, r->ER, r->nivel,
                (i + 1 < lista->cantidad) ? "," : "");
    }

    fprintf(f, "  ]\n}\n");
    fclose(f);
    return 1;
}

static int guardar_xml(const ListaRiesgos *lista, const char *nombre) {
    FILE *f = fopen(nombre, "w");
    if (!f) return 0;

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<riesgos>\n");

    for (size_t i = 0; i < lista->cantidad; ++i) {
        const Riesgo *r = &lista->datos[i];

        fprintf(f, "  <riesgo id=\"%d\">\n", r->id);
        fprintf(f, "    <nombre>%s</nombre>\n", r->nombre);
        fprintf(f, "    <descripcion>%s</descripcion>\n", r->descripcion);
        fprintf(f, "    <F>%d</F>\n", r->F);
        fprintf(f, "    <S>%d</S>\n", r->S);
        fprintf(f, "    <P>%d</P>\n", r->P);
        fprintf(f, "    <E>%d</E>\n", r->E);
        fprintf(f, "    <A>%d</A>\n", r->A);
        fprintf(f, "    <V>%d</V>\n", r->V);
        fprintf(f, "    <I>%d</I>\n", r->I);
        fprintf(f, "    <D>%d</D>\n", r->D);
        fprintf(f, "    <C>%d</C>\n", r->C);
        fprintf(f, "    <PR>%d</PR>\n", r->PR);
        fprintf(f, "    <ER>%d</ER>\n", r->ER);
        fprintf(f, "    <nivel>%s</nivel>\n", r->nivel);
        fprintf(f, "  </riesgo>\n");
    }

    fprintf(f, "</riesgos>\n");
    fclose(f);
    return 1;
}

static void guardar_riesgos(const ListaRiesgos *lista) {
    if (lista->cantidad == 0) {
        printf("No hay riesgos para guardar.\n");
        return;
    }

    char nombre[MAX_ARCHIVO];
    int formato;

    leer_linea("Nombre del archivo (ej. riesgos.json): ",
               nombre, sizeof(nombre));

    printf("\nFormato:\n");
    printf("1. TXT\n");
    printf("2. CSV (separado por coma)\n");
    printf("3. JSON\n");
    printf("4. XML\n");

    formato = leer_entero("Seleccione formato: ", 1, 4);

    int ok = 0;

    switch (formato) {
        case 1: ok = guardar_txt(lista, nombre); break;
        case 2: ok = guardar_csv(lista, nombre); break;
        case 3: ok = guardar_json(lista, nombre); break;
        case 4: ok = guardar_xml(lista, nombre); break;
    }

    if (ok)
        printf("Riesgos guardados en '%s'.\n", nombre);
    else
        perror("No se pudo guardar el archivo");
}

/*
 * Carga un archivo CSV producido por este mismo programa.
 * Usa realloc dinámicamente para ampliar la lista mientras lee.
 */
static int cargar_csv(ListaRiesgos *lista, const char *nombre) {
    FILE *f = fopen(nombre, "r");
    char linea[1200];

    if (!f) return 0;

    liberar_lista(lista);

    /* Encabezado */
    if (fgets(linea, sizeof(linea), f) == NULL) {
        fclose(f);
        return 1;
    }

    while (fgets(linea, sizeof(linea), f)) {
        Riesgo r;
        char *token;

        memset(&r, 0, sizeof(r));

        token = strtok(linea, ",");
        if (!token) continue;
        r.id = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(r.nombre, token, MAX_NOMBRE - 1);

        /* El nombre viene entre comillas */
        size_t len = strlen(r.nombre);
        if (len > 0 && r.nombre[0] == '"') {
            memmove(r.nombre, r.nombre + 1, len);
            len--;
            if (len > 0 && r.nombre[len - 1] == '"')
                r.nombre[len - 1] = '\0';
        }

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(r.descripcion, token, MAX_DESC - 1);

        len = strlen(r.descripcion);
        if (len > 0 && r.descripcion[0] == '"') {
            memmove(r.descripcion, r.descripcion + 1, len);
            len--;
            if (len > 0 && r.descripcion[len - 1] == '"')
                r.descripcion[len - 1] = '\0';
        }

        token = strtok(NULL, ","); if (!token) continue; r.F = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.S = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.P = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.E = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.A = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.V = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.I = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.D = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.C = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.PR = atoi(token);
        token = strtok(NULL, ","); if (!token) continue; r.ER = atoi(token);
        token = strtok(NULL, "\n"); if (!token) continue;

        strncpy(r.nivel, token, sizeof(r.nivel) - 1);
        len = strlen(r.nivel);
        if (len > 0 && r.nivel[0] == '"') {
            memmove(r.nivel, r.nivel + 1, len);
            len--;
            if (len > 0 && r.nivel[len - 1] == '"')
                r.nivel[len - 1] = '\0';
        }

        if (!asegurar_capacidad(lista)) {
            fclose(f);
            return 0;
        }

        lista->datos[lista->cantidad++] = r;
    }

    fclose(f);
    return 1;
}

static void seleccionar_archivo(ListaRiesgos *lista) {
    char nombre[MAX_ARCHIVO];

    printf("\n--- SELECCIONAR ARCHIVO ESPECIFICO ---\n");
    printf("El cargador espera un CSV creado por este programa.\n");
    leer_linea("Escriba el nombre/ruta del archivo CSV: ",
               nombre, sizeof(nombre));

    if (cargar_csv(lista, nombre)) {
        printf("Archivo '%s' cargado correctamente usando memoria dinamica.\n",
               nombre);
        printf("Cantidad de riesgos: %zu\n", lista->cantidad);
    } else {
        perror("No se pudo cargar el archivo");
    }
}

/* ---------- Ejemplo de terremoto ---------- */

static void cargar_ejemplo_terremoto(ListaRiesgos *lista) {
    if (!asegurar_capacidad(lista))
        return;

    Riesgo r;
    memset(&r, 0, sizeof(r));

    r.id = siguiente_id(lista);
    strcpy(r.nombre, "Terremoto en instalaciones");
    strcpy(r.descripcion,
           "Sismo que puede afectar la estructura, personal, equipos y continuidad operativa.");

    /*
     * Ejemplo educativo:
     * F=5: la empresa depende mucho de sus instalaciones.
     * S=4: sustituir la operacion seria dificil.
     * P=5: puede producir consecuencias profundas.
     * E=5: puede afectar a gran parte de la empresa.
     * A=4: exposicion/agresion significativa.
     * V=5: alta vulnerabilidad si no hay controles.
     */
    r.F = 5;
    r.S = 4;
    r.P = 5;
    r.E = 5;
    r.A = 4;
    r.V = 5;

    calcular_mosler(&r);
    lista->datos[lista->cantidad++] = r;

    printf("\nEjemplo de terremoto agregado.\n");
    mostrar_riesgo(&lista->datos[lista->cantidad - 1]);
    recomendacion_ia(&lista->datos[lista->cantidad - 1]);
}

/* ---------- Menu ---------- */

static void menu(void) {
    printf("\n");
    printf("============================================================\n");
    printf("       SISTEMA DE GESTION DE RIESGOS - METODO MOSLER\n");
    printf("                  CASO: TERREMOTOS\n");
    printf("============================================================\n");
    printf("1. Crear riesgo\n");
    printf("2. Modificar parametros del riesgo\n");
    printf("3. Calcular riesgos\n");
    printf("4. Imprimir los riesgos\n");
    printf("5. Borrar riesgo\n");
    printf("6. Seleccionar/cargar un archivo especifico (CSV)\n");
    printf("7. Crear un archivo especifico\n");
    printf("8. Guardar riesgos: TXT / CSV / JSON / XML\n");
    printf("9. Analisis IA local y recomendaciones\n");
    printf("10. Cargar ejemplo de terremoto\n");
    printf("0. Salir\n");
    printf("============================================================\n");
}

static void calcular_todos(ListaRiesgos *lista) {
    for (size_t i = 0; i < lista->cantidad; ++i)
        calcular_mosler(&lista->datos[i]);

    printf("Se recalcularon %zu riesgos.\n", lista->cantidad);
}

static void ejecutar_ia(const ListaRiesgos *lista) {
    if (lista->cantidad == 0) {
        printf("No hay riesgos cargados.\n");
        return;
    }

    int id = leer_entero("ID del riesgo para analizar con IA: ", 1, 1000000);
    int idx = buscar_indice(lista, id);

    if (idx < 0) {
        printf("No existe ese riesgo.\n");
        return;
    }

    recomendacion_ia(&lista->datos[idx]);
}

int main(void) {
    ListaRiesgos lista;
    int opcion;

    inicializar_lista(&lista);

    do {
        menu();
        opcion = leer_entero("Seleccione una opcion: ", 0, 10);

        switch (opcion) {
            case 1:
                crear_riesgo(&lista);
                break;

            case 2:
                modificar_riesgo(&lista);
                break;

            case 3:
                calcular_todos(&lista);
                break;

            case 4:
                imprimir_riesgos(&lista);
                break;

            case 5:
                borrar_riesgo(&lista);
                break;

            case 6:
                seleccionar_archivo(&lista);
                break;

            case 7:
                crear_archivo_especifico();
                break;

            case 8:
                guardar_riesgos(&lista);
                break;

            case 9:
                ejecutar_ia(&lista);
                break;

            case 10:
                cargar_ejemplo_terremoto(&lista);
                break;

            case 0:
                printf("Saliendo...\n");
                break;
        }

        if (opcion != 0)
            pausa();

    } while (opcion != 0);

    liberar_lista(&lista);
    return 0;
}
