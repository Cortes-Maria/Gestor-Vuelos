#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <ctype.h>
#include "conecta_mySQL.h"
#include "manejo_datos.h"
#include "funciones_cliente.h"
#define INSERT_CLIENTE "insert into Cliente(pkCliente, Nombre, Apellido1, Apellido2, Sexo, FechaNacimiento, Edad) values ("


void carga_usuarios(){
    /*
      Función encargada de realizar la carga de usuarios
      E: N/A
      S: N/A
      R: N/A
    */
    char nombre_archivo[50];
    printf("Ingrese la dirección del archivo: ");
    scanf(" %[^\n]", nombre_archivo);
    FILE *archivo;//Puntero a el archivo .txt
    archivo = fopen(nombre_archivo, "r");//se abre el archivo

    //Variables para sacar el nombre del archivo reporte
    char *tok;
    char *nombre;
    char *extension;
    //variables char por incompatibilidad de punteros
    char nombre_reporte[50];
    char extension_char[10];

    nombre = strtok(nombre_archivo, ".");
    extension = strtok(NULL, "");
    
    strcpy(nombre_reporte, nombre);//copiamos a variables estáticas
    strcpy(extension_char, extension);//para poder usar strcat

    strcat(nombre_reporte, "_reporte.");
    strcat(nombre_reporte, extension_char);//nombre_reporte tiene el nombre del archivo reporte


	char linea[80];//Guarda cada línea del .txt
    int contador = 1;//Lleva la cuenta de las lineas del archivo
    int i;
    MYSQL *conn = mysql_init(NULL);//se inicializa un puntero para conectarse a mySQL
    conn = conexion_mySQL();

    //Chars para pasar a los inserts
    char query[200];//Tiene el query que se va a enviar a mySQL

	if (archivo == NULL)
    {
        printf("\nError de apertura del archivo. \n\n");
    }
    else
    {
        while (fgets(linea, 80, archivo) != NULL){//Se obtiene una línea entera del archivo

            if (guardar_datos(linea) == 1){
                generar_reporte(contador, 1, nombre_reporte);
                contador++;
                continue;
            }

            //se completa el query con los datos
            strcpy(query, INSERT_CLIENTE);
            i = 0;
            while (i < 7){
                if (i == 6){
                    strcat(query, "'A')");
                }else{
                    strcat(query, clientes[i].dato);
                    strcat(query, ", ");
                }
                i++;
            }

            if (mysql_query(conn, query)) {//Ejecuta el query de insert

                strcpy(tok, mysql_error(conn));
                tok = strtok(tok, " ");

                if (strcmp(tok, "Duplicate") == 0){//Verifica si el error fue por pasaporte repetido
                    generar_reporte(contador, 2, nombre_reporte);
                }else{
                    generar_reporte(contador, 1, nombre_reporte);
                }
            }
            else{
                generar_reporte(contador, 0, nombre_reporte);//Determinar si es Adulto o Infante
                strcpy(query, "call editarEdad(");
                strcat(query, clientes[0].dato);
                strcat(query, ")");
                if (mysql_query(conn, query)) {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                }
            }
            contador ++;
        }
    }
    mysql_close(conn);//se cierra la conexion con mySQL
    fclose(archivo);//se cierra el archivo
}

char *personasXvuelo(char idVuelo[10]){
    /*
      Función que determina la cantidad de personas que hay en un vuelo
      E: recibe un char con el código del vuelo
      S: retorna la cantidad de personas en un char
      R: N/A
    */ 
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *resultado; 
    char query[50] = "select personasXvuelo(";
    strcat(query, idVuelo);
    strcat(query, ")");

    conn = conexion_mySQL();
    if (mysql_query(conn, query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    resultado = malloc(6);
    strcpy(resultado, row[0]);
    mysql_free_result(res);
    mysql_close(conn);
    printf("Hay %s personas en total en este vuelo\n", resultado);
    return resultado;
    
}

void estado_vuelo(){
    /*
      Función encargada de mostrar en pantalla el estado de un vuelo
    */
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
	MYSQL_ROW row;
    char solicitar_query[150] = "call info_general(";    
    char cantidadPersonas[4];
    char idVuelo[5];
    printf("Indique el código de vuelo: ");
    scanf(" %s", idVuelo);
    strcat(solicitar_query, idVuelo);
    strcat(solicitar_query, ")");
    conn = conexion_mySQL();    

    //obtenemos la query que necesitamos
    if (mysql_query(conn, solicitar_query)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);

    //Imprimimos los datos generales del vuelo
    printf("\n\n\nCódigo de vuelo: %s\n", idVuelo);
    printf("Salió desde %s en la fecha y hora %s\n", row[0], row[1]);
    printf("Llegó a %s en la fecha y hora %s\n", row[2], row[3]);
    printf("Avión de la marca %s, modelo %s\n", row[4], row[5]);
    mysql_free_result(res);
    
    //Pasamos a imprimir los asientos
    printf("\n\nDetalle gráfico de los asientos\n\n");
    imprime_asientos(idVuelo, conn);
    
    //Sacamos ahora la cantidad de asientos por tipo
    strcpy(solicitar_query, "call tipo_asientos(");
    strcat(solicitar_query, idVuelo);
    strcat(solicitar_query, ")");
    conn = conexion_mySQL();
    //obtenemos la query de tipos de asiento
    if (mysql_query(conn, solicitar_query)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res = mysql_use_result(conn);
   
    //Empezamos a imprimirlos
    printf("\nDe lo anterior podemos resumir\n");

    while ((row = mysql_fetch_row(res)) != NULL){
        printf("Tipo de asiento: %s; Cantidad: %s\n", row[0], row[1]);
    }
    mysql_free_result(res);
    mysql_close(conn);
    
    strcpy(cantidadPersonas, personasXvuelo(idVuelo));

    if (strcmp(cantidadPersonas, "0") == 0){
        printf("No hay ninguna reservación asociada con este vuelo");
        return;
    }

    //Ahora sacamos la cantidad de infantes y adultos
    strcpy(solicitar_query, "call asiento_edades(");
    strcat(solicitar_query, idVuelo);
    strcat(solicitar_query, ")");
    conn = conexion_mySQL();
    //obtenemos la query de tipos de asiento
    if (mysql_query(conn, solicitar_query)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    printf("\nEn este vuelo hay %s adultos ", row[1]);
    row = mysql_fetch_row(res);
    printf("y %s infantes\n", row[1]);
    
    mysql_free_result(res);
    mysql_close(conn);
    
    //Ahora imprimimos el detalle de las reservaciones
    //Para esto sacamos todas las reservas que esté en este vuelo
    strcpy(solicitar_query, "select r.pkReserva from Reserva r inner join Vuelo v on r.fkVuelo = ");
    strcat(solicitar_query, idVuelo);
    conn = conexion_mySQL();
    if (mysql_query(conn, solicitar_query)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

    res = mysql_use_result(conn);

    printf("\nSe tienen las siguientes reservaciones registradas en este vuelo\n\n");
    printf("---------------------------------------------------\n");
    while((row = mysql_fetch_row(res)) != NULL){
        consultar_reservacion(row[0]);
        printf("---------------------------------------------------\n");
    }
    mysql_free_result(res);
    mysql_close(conn);
}