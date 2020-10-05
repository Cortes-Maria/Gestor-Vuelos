#include <stdio.h>
#include <string.h>

struct datosReserva{
    char pas[10];
    char edad[4];
    char id_vuelo[10];
    char idAsiento[7];
} pasaportes_c[15];

void limpiar_struct(){
    /*Función que elimina los datos que quedaron almacenados en el 
    struct de pasaportes_c para que quede limpia para la próxima reservación
    */


   for (int i=0; strcmp(pasaportes_c[i].id_vuelo, "") != 0;i++){
       strcpy(pasaportes_c[i].edad, "");
       strcpy(pasaportes_c[i].id_vuelo, "");
       strcpy(pasaportes_c[i].idAsiento, "");
       strcpy(pasaportes_c[i].pas, "");
   }

}

void agregarClientes(char idReservacion[5], int personas){
    /* 
      Función encargada de agregar personas a la tabla ClienteXReserva
      E: el código de la reservación, la cantidad de personas en la reservación
      S: N/A
    */
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
    MYSQL_ROW row;

    char q_callInsert[50] = "call insertaClientesReserva(";
    char q_Insert[50];

    strcat(q_callInsert, idReservacion);
    strcat(q_callInsert, ",");
    conn = conexion_mySQL();
    for (int i=0; i<personas; i++){
        strcpy(q_Insert, q_callInsert);
        strcat(q_Insert, pasaportes_c[i].pas);
        strcat(q_Insert, ",");
        strcat(q_Insert, pasaportes_c[i].idAsiento);
        strcat(q_Insert, ")");
        if (mysql_query(conn, q_Insert)) {
	    	fprintf(stderr, "%s\n", mysql_error(conn));
		    exit(1);
        }
    }
    mysql_close(conn);
    limpiar_struct();
    generar_PDF(idReservacion);
    printf("\n\nSe realizó la reservación con éxito\n");
    printf("El codigo de su reservación es: %s\n\n", idReservacion);

}


void agregarReservacion(int personas){
    /*
      Función que agrega una reservación a la tabla Reserva
      E: un entero con la cantidad de personas en la reserva
      S: N/A
    */
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
    MYSQL_ROW row;
    char q_crearR[50] = "select crearReservacion("; 
    
    char idReservacion[5];
    strcat(q_crearR, pasaportes_c[0].pas);
    strcat(q_crearR, ",");
    strcat(q_crearR, pasaportes_c[0].id_vuelo);
    strcat(q_crearR, ")");

    conn = conexion_mySQL();
    if (mysql_query(conn, q_crearR)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    strcpy(idReservacion, row[0]);

    mysql_free_result(res);
    mysql_close(conn);

    agregarClientes(idReservacion, personas);
}


char* verificar_asiento(char vuelo[10],char fila[3],char columna[3]){
    /*Verifica que el asiento ingreasado por el usuario exista y que esté libre
        E: el codigo de vuelo, la fila y columna del asiento
        S: el id del asiento
        R: N/A
    */
    
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
	MYSQL_ROW row;
    char q_asientos[50] = "select asiento_disponible(";
    char *resultado;

    strcat(q_asientos, vuelo);
    strcat(q_asientos, ",");
    strcat(q_asientos, fila),
    strcat(q_asientos, ",");
    strcat(q_asientos, columna);
    strcat(q_asientos, ")");

    conn = conexion_mySQL();
    if (mysql_query(conn, q_asientos)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    
    resultado = malloc(3);
    strcpy(resultado, row[0]);

    mysql_free_result(res);
    mysql_close(conn);

    return resultado;
}

char* obtenerID_asientoI(char vuelo[10]){
    /*
    Obtiene el ID del 'asiento' para infantes
    E: el codigo del vuelo
    S: el ID del asiento
    R: N/A
    */
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
    MYSQL_ROW row;

    char* resultado;
    int asiento_I;
    printf("1. Tipo B\n");
    printf("2. Tipo S\n");
    printf("3. Tipo E\n");
    scanf(" %d", &asiento_I);
    char query[50] = "select retornaIDAsiento(";
    strcat(query, vuelo);
    strcat(query, ",");

    switch (asiento_I){
        case 1:        
            strcat(query, "'BI'");
            break;
        case 2:
            strcat(query, "'SI'");
            break;
        case 3:
            strcat(query, "'EI'");
            break;
        default:
            printf("Favor ingresar un valor válido\n\n");
            return "0";

    }

    strcat(query, ")");
    conn = conexion_mySQL();

    if (mysql_query(conn, query)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    resultado = malloc(3);
    strcpy(resultado, row[0]);
    mysql_free_result(res);
    mysql_close(conn);
    return resultado;
}


int verifica_pasaportes(char pasaportes[100], char vueloID[10]){
    /*
    Función que verifica que los pasaportes ingresados existan en la base de datos
    E: una cadena de caracteres con los pasaportes
    S: un entero indicando 0 que si existen todos y 1 que al menos uno no
    R: N/A
    */
   
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
	MYSQL_ROW row;
    char q_existePas[30] = "select existeCliente(";
    char query[30];
    char *tok;
    int contador = 0;
    int asientos = 0;
    tok = strtok(pasaportes, ",");
    conn = conexion_mySQL();

    while (tok != NULL){
        
        strcpy(query, q_existePas);
        strcat(query, tok);
        strcat(query, ")");

        if (mysql_query(conn, query)) {
		    fprintf(stderr, "%s\n", mysql_error(conn));
		    exit(1);
	    }

        res = mysql_use_result(conn);
        row = mysql_fetch_row(res);


        if (strcmp(row[0], "0") == 0){
            printf("\nEl pasaporte %s no existe en la base de datos, favor ingresar nuevamente los pasaportes\n\n", tok);
            //limpiar_struct();
            return 0;
        }
        else if (strcmp(row[0], "1") == 0){
            strcpy(pasaportes_c[contador].edad, "A");
        }
        else {
            strcpy(pasaportes_c[contador].edad, "I");
        }
        strcpy(pasaportes_c[contador].pas, tok);
        strcpy(pasaportes_c[contador].id_vuelo, vueloID);
        contador++;
        mysql_free_result(res);
        tok = strtok(NULL, ",");
    }

    mysql_close(conn);
    return contador;
}

void reservar_vuelo(){
    /*
      Función encargada de reservar vuelos
    */
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
	MYSQL_ROW row;
    char pasaportes[100];
    int resultado = 0;
    char q_existeVuelo[30] = "select existeVuelo(";
    char idVuelo[10];
    int i = 0;
    char a_fila[3];
    char a_columna[3];
    char id_asiento[7];
    char id_asiento_I[7];
    printf("Ingrese el código de un vuelo existente: ");
    scanf(" %s", idVuelo);

    strcat(q_existeVuelo, idVuelo);
    strcat(q_existeVuelo, ")");

    conn = conexion_mySQL();
    //Primero verificamos que exista el vuelo
    if (mysql_query(conn, q_existeVuelo)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if (strcmp(row[0], "0") == 0){
        printf("Error: El código de vuelo que ingresó no existe\n");
        return;
    }

    mysql_free_result(res);
    mysql_close(conn);

    while (resultado == 0){
        printf("Ingrese correctamente los pasaportes de los clientes\n");
        printf("Favor ingresarlos separados por coma y sin espacios, para salir escriba '0'\n");
        scanf(" %s", pasaportes);

        if (strcmp(pasaportes, "0") == 0){
            return;
        }
        resultado = verifica_pasaportes(pasaportes, idVuelo);
    }
    int res2 = resultado;
    printf("\n\nEstos son los asientos de este vuelo\n\n");
    conn = conexion_mySQL();
    imprime_asientos(idVuelo, conn);


    i = 0 ;
    while (i<resultado){
        if (strcmp(pasaportes_c[i].edad, "A") == 0){
            printf("Asiento para el pasaporte %s\n", pasaportes_c[i].pas);
            printf("Ingrese la fila: ");
            scanf(" %s", a_fila);
            printf("Ingrese la columna: ");
            scanf(" %s", a_columna);
            strcpy(id_asiento, verificar_asiento(idVuelo, a_fila, a_columna));

            if (strcmp(id_asiento, "0") == 0){
                printf("El asiento seleccionado está ocupado o no existe\n\n");
                continue;
            }
            else{
                strcpy(pasaportes_c[i].idAsiento, id_asiento);
            }
        }
        else {
            printf("Ingrese el tipo de boleto para el infante con pasaporte %s\n", pasaportes_c[i].pas);
            strcpy(id_asiento_I, obtenerID_asientoI(idVuelo));
            if (strcmp(id_asiento_I, "0") == 0){
                continue;
            }
            else{
                strcpy(pasaportes_c[i].idAsiento, id_asiento);        
            }
        }
        i++;

    }
    agregarReservacion(resultado);
}

