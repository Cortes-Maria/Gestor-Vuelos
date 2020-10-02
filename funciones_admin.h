#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <ctype.h>
#include "conecta_mySQL.h"
#include "manejo_datos.h"
#define INSERT_CLIENTE "insert into Cliente(pkCliente, Nombre, Apellido1, Apellido2, Sexo, FechaNacimiento, Edad) values ("


void carga_usuarios(){

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

            if (contador == 10){
                }
            if (mysql_query(conn, query)) {//Ejecuta el query de insert
                if (contador == 10){
                }
                strcpy(tok, mysql_error(conn));
                if (contador == 10){
                }
                tok = strtok(tok, " ");
                if (contador == 10){
                }
                if (strcmp(tok, "Duplicate") == 0){//Verifica si el error fue por pasaporte repetido
                    generar_reporte(contador, 2, nombre_reporte);
                }else{
                    generar_reporte(contador, 1, nombre_reporte);
                }
            }
            else{
                generar_reporte(contador, 0, nombre_reporte);
            }
            contador ++;
        }
    }
    mysql_close(conn);//se cierra la conexion con mySQL
    fclose(archivo);//se cierra el archivo
}

