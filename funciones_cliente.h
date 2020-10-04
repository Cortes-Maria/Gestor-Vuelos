#include <stdio.h>
#include <string.h>
//#include "conecta_mySQL.h"

void consultar_reservacion(char idReserva[10]){
    MYSQL *conn = mysql_init(NULL);
	MYSQL_RES *res;
	MYSQL_ROW row;
    char query[50] = "call obtener_query_r(";
    char existeReserva[50] = "select existeReserva(";
    strcat(query, idReserva);
    strcat(existeReserva, idReserva);
    strcat(query, ")");
    strcat(existeReserva, ")");
    conn = conexion_mySQL();
    
    //Primero verificamos que la reserva exista
    if (mysql_query(conn, existeReserva)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if (strcmp(row[0], "0") == 0){
        printf("Error: El código que ingresó no existe\n");
        return;
    }
    mysql_free_result(res);

    if (mysql_query(conn, query)) {
        printf("Error: El código que ingresó no existe\n");
        return;
	}
    
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    int contador = 1;
    printf("\nCódigo de reserva #%s\n", idReserva);
    printf("Código de vuelo #%s\n", row[0]);
    while (row != NULL){
        printf("\nCliente #%d\n", contador);
        printf("Nombre: %s %s %s\n", row[1], row[2], row[3]);
        printf("Número de pasaporte: %s\n", row[4]);
        printf("Edad: %s\n", row[9]);
        printf("Asiento tipo %s: ", row[5]);
        printf("ubicado en la fila %s y columna %s\n", row[6], row[7]);
        printf("Precio del boleto: %s\n", row[8]);
        row = mysql_fetch_row(res);
        contador ++;
    }
    
    
    mysql_free_result(res);
    mysql_close(conn);
    //Ahora sacamos el monto total pagado
    strcpy(query, "select obtener_monto(");
    strcat(query, idReserva);
    strcat(query, ")");    
    conn = conexion_mySQL();
    if (mysql_query(conn, query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);

    printf("\nMonto total de la reservación: %s\n", row[0]);

	mysql_close(conn);
}

