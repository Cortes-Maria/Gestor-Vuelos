#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <ctype.h>
#include "conecta_mySQL.h"
#define INSERT_CLIENTE "insert into Cliente(pkCliente, Nombre, Apellido1, Apellido2, Sexo, FechaNacimiento, Edad) values ("

int validar_datos(char* dato, int id){
    char data[35];//Para guardar los datos en un arreglo para facilitar su iteración
    int i = 0;//Para recorrer cadenas
    switch (id)
    {
        //VALIDAR NOMBRE Y APELLIDOS NO TENGA NÚMEROS
        case 2:
            strcpy(data, dato);
            while (i<strlen(dato)){
                if (isdigit(data[i]) != 0){
                    return 1;
                }
                i++;
            }
            return 0;
        
        //VALIDAR FECHA DE NACIMIENTO
        case 4:
            if (strlen(dato) != 12){//Si no tiene los 12 caracteres se descarta
                return 1;
            }
            strcpy(data, dato);//Lo copia para pasarlo a un array de caracteres

            while (i < 12){
                
                if (i == 0 || i == 11){//Si la fecha no está entre comillas
                    if (data[i] != '\''){
                        return 1;
                    }
                }
                else if (i == 5 || i == 8){//si no hay un guión donde deberia se descarta
                    if (data[i] != '-'){
                        return 1;
                    }
                }
                else if (isdigit(data[i]) == 0){//si los demás no son digitos se descarta
                    return 1;
                }
                i++;
            }
            return 0;
    }
}

void carga_usuarios(){

    char nombre_archivo[] = "prueba.txt";//Tiene el nombre del archivo a abrir
    FILE *archivo;//Puntero a el archivo .txt
    archivo = fopen(nombre_archivo, "r");//se abre el archivo

	char linea[100];//Guarda cada línea del .txt
    char *tok;//Guarda porciones de cada linea del .txt
    int contador = 1;//Lleva la cuenta de las lineas del archivo

    MYSQL *conn = mysql_init(NULL);//se inicializa un puntero para conectarse a mySQL
    conn = conexion_mySQL();

    //Chars para pasar a los inserts
    char query[200];//Tiene el query que se va a enviar a mySQL
    char parametros[100];//Guarda una línea entera del .txt para agregarla al query

	if (archivo == NULL)
    {
        printf("\nError de apertura del archivo. \n\n");
    }
    else
    {
        while (fgets(linea, 100, archivo) != NULL){//Se obtiene una línea entera del archivo
            strcpy(parametros, linea);//se copía la linea entera

            //PASAPORTE
            tok = strtok(linea, ",");//El pasaporte no se valida en C

            //NOMBRE
            tok = strtok(NULL, ",");//Validamos que el nombre no tenga números
            if (validar_datos(tok, 2) == 1){
                printf("Error de inserción en la linea: %d\n", contador);
                contador ++;
                //generar_reporte("ERROR");
                continue;
            }

            //PRIMER APELLIDO
            tok = strtok(NULL, ",");
            if (validar_datos(tok, 2) == 1){
                printf("Error de inserción en la linea: %d\n", contador);
                contador ++;
                //generar_reporte("ERROR");
                continue;
            }

            //SEGUNDO APELLIDO
            tok = strtok(NULL, ",");
            if (validar_datos(tok, 2) == 1){
                printf("Error de inserción en la linea: %d\n", contador);
                contador ++;
                //generar_reporte("ERROR");
                continue;
            }

            //SEXO no se valida en C
            tok = strtok(NULL, ",");

            //FECHA DE NACIMIENTO
            tok = strtok(NULL, "\n");
            if (validar_datos(tok, 4) == 1){//VALIDA FORMATO DE FECHA
                printf("Error de inserción en la linea: %d\n", contador);
                contador ++;
                //generar_reporte("ERROR");
                continue;
            }
            //Si llega hasta aqui todos los datos validados en C están bien
            
            //Se termina de completar el query
            strcpy(query, INSERT_CLIENTE);
            strcat(query, parametros);
            strcat(query, ", 'A')");
            
            if (mysql_query(conn, query)) {//Ejecuta el query de insert
                    fprintf(stderr, "%s en linea %d\n", mysql_error(conn), contador);
		            //exit(1);
                    //generar_reporte("ERROR");
            }
            else{
                //generar_reporte("EXITO");
                printf("Se insertó la linea #%d\n", contador);
            }
            contador ++;
        }
        
    }
    mysql_close(conn);//se cierra la conexion con mySQL
    fclose(archivo);//se cierra el archivo
}

void generar_reporte(int ind, char desc[50]){

}