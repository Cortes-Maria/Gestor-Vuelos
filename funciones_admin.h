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

void generar_reporte(int linea, int ind, char nombre_archivo[100]){

    FILE *archivo;
    archivo = fopen(nombre_archivo, "a");
    char linea_char[3];
    sprintf(linea_char, "%d", linea);//convierte entero en char

    fputs("Linea ", archivo);
    if (linea < 10){
        fputc('0', archivo);//para mantener simetría
    }
    fputs(linea_char, archivo);
    fputs(": ", archivo);

    if (ind == 0){
        fputs("Incluido\n", archivo);
    }
    else if (ind == 1){
        fputs("Error\n", archivo);
    }
    else {
        fputs("Repetido\n", archivo);
    }

    fclose(archivo);



    

}

void carga_usuarios(){

    char nombre_archivo[100];
    printf("Ingrese la dirección del archivo: ");
    scanf(" %[^\n]", nombre_archivo);
    FILE *archivo;//Puntero a el archivo .txt
    archivo = fopen(nombre_archivo, "r");//se abre el archivo

    //Variables para sacar el nombre del archivo reporte
    char *nombre;
    char *extension;
    //variables char por incompatibilidad de punteros
    char nombre_char[100];
    char extension_char[10];

    nombre = strtok(nombre_archivo, ".");
    extension = strtok(NULL, "");
    
    strcpy(nombre_char, nombre);//copiamos a variables estáticas
    strcpy(extension_char, extension);//para poder usar strcat

    strcat(nombre_char, "_reporte.");
    strcat(nombre_char, extension_char);//nombre_char tiene el nombre del archivo reporte


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
                contador ++;
                generar_reporte(contador, 1, nombre_char);
                continue;
            }

            //PRIMER APELLIDO
            tok = strtok(NULL, ",");
            if (validar_datos(tok, 2) == 1){
                contador ++;
                generar_reporte(contador, 1, nombre_char);
                continue;
            }

            //SEGUNDO APELLIDO
            tok = strtok(NULL, ",");
            if (validar_datos(tok, 2) == 1){
                contador ++;
                generar_reporte(contador, 1, nombre_char);
                continue;
            }

            //SEXO no se valida en C
            tok = strtok(NULL, ",");

            //FECHA DE NACIMIENTO
            tok = strtok(NULL, "\n");
            if (validar_datos(tok, 4) == 1){//VALIDA FORMATO DE FECHA
                contador ++;
                generar_reporte(contador, 1, nombre_char);
                continue;
            }
            //Si llega hasta aqui todos los datos validados en C están bien
            
            //Se termina de completar el query
            strcpy(query, INSERT_CLIENTE);
            strcat(query, parametros);
            strcat(query, ", 'A')");
            
            if (mysql_query(conn, query)) {//Ejecuta el query de insert
		            strcpy(tok, mysql_error(conn));
                    tok = strtok(tok, " ");
                    if (strcmp(tok, "Duplicate") == 0){//Verifica si el error fue por pasaporte repetido
                        generar_reporte(contador, 2, nombre_char);
                    }else{
                        generar_reporte(contador, 1, nombre_char);
                    }
            }
            else{
                generar_reporte(contador, 0, nombre_char);
            }
            contador ++;
        }
        
    }
    mysql_close(conn);//se cierra la conexion con mySQL
    fclose(archivo);//se cierra el archivo
}