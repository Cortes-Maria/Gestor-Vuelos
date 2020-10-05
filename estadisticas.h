#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <mysql.h>
#include "conecta_mySQL.h"
#define TOP_VENTAS "CALL TOP3_MayorVenta()"
#define TOP_CANT "CALL TOP3_MayorCantidad()"




void MayorVentas() {
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;

    int res_int;//Guarda el resultado de la query
    char q_mostrarTOP3[300];//Guarda el query para iniciar sesión
    strcpy(q_mostrarTOP3, TOP_VENTAS);//Agrega una parte de la query para la función

    if (mysql_query(conn, q_mostrarTOP3)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);

    printf("TOP 3 DE VUELOS DE MAYOR VENTA!\n");
    	
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s","Monto de venta: ");
		printf("%s", row[0]);
        printf("  ");
        printf("%s", "Código de vuelo: ");
        printf("%s", row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    mysql_close(conn);

}
void MayorCantidadPersonas() {
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;

    int res_int;//Guarda el resultado de la query
    char q_mostrarTOP3[300];//Guarda el query para iniciar sesión
    strcpy(q_mostrarTOP3, TOP_CANT);//Agrega una parte de la query para la función

    if (mysql_query(conn, q_mostrarTOP3)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);

    printf("TOP 3 DE VUELOS DE MAYO CANTIDAD DE PERSONAS!\n");
    	
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s","Cantidad de personas: ");
		printf("%s", row[0]);
        printf("  ");
        printf("%s", "Código de vuelo: ");
        printf("%s", row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    mysql_close(conn);
}

void menuEstadisticas() {
    int eleccion;
    do{
        printf("\n      Menú estadísticas\n");
        printf( " __________________________________________________________________________________________\n");
        printf( "|                                                                                          |\n");
        printf( "| 1.Top 3 de vuelos de mayor venta (de mayor a menor)                                      |\n");
        printf( "| 2.Top 3 de vuelos con mayor cantidad de personas (de mayor a menor, incluyendo infantes) |\n");
        printf( "| 0.Salir                                                                                  |\n" );
        printf( ".__________________________________________________________________________________________.\n");
        printf("\nIngrese su elección: ");
        scanf(" %d", &eleccion);
        printf("\n");
        
        switch (eleccion){
            case 1:
                MayorVentas();
                break;
            case 2:
                MayorCantidadPersonas();
                break;
            case 3:
                
                break;
            case 0:
                break;
        }
    }while(eleccion != 0);

}