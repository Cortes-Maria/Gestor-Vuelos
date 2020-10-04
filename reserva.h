#include <stdio.h>
#include <string.h>


int verifica_pasaportes(char pasaportes[100]){
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
    int contador = 1;

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
            printf("El pasaporte %s no existe en la base de datos, favor ingresar todo de nuevo :'D\n", tok);
            return 1;
        }

        mysql_free_result(res);
        tok = strtok(NULL, ",");
        contador ++;
    }

    mysql_close(conn);
    return 0;
}

void reservar_vuelo(){
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
	MYSQL_ROW row;
    char pasaportes[100];

    char q_existeVuelo[30] = "select existeVuelo(";
    char idVuelo[10];

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
    printf("Ingrese correctamente los pasaportes de los clientes\n");
    printf("Favor ingresarlos separados por coma y sin espacios\n");
    scanf(" %s", pasaportes);
    int resultado = verifica_pasaportes(pasaportes);

}
