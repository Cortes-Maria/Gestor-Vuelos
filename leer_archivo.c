#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "leer_archivo.h"

int main()
{
    FILE *archivo;
	char linea[100];
    char *tok;
    int contador = 1;
    archivo = fopen("prueba.txt", "r");

	if (archivo == NULL)
    {
        printf("\nError de apertura del archivo. \n\n");
    }
    else
    {
        while (fgets(linea, 100, archivo) != NULL){//fgets obtiene una línea completa de un archivo
            printf("\nCliente #%d\n", contador);

            //PASAPORTE
            tok = strtok(linea, ",");//srtok lee una cadena hasta llegar a la llave indicada
            if (validar_datos(tok, 1) == 1){//Valida que el pasaporte este correcto
                printf("Pasaporte: formato incorrecto\n");
            }else{
                printf("Pasaporte: %s\n", tok);
            }

            //NOMBRE
            tok = strtok(NULL, ",");//Continua en la misma linea hasta la siguiente llave indicada
            if (validar_datos(tok, 2) == 1){
                printf("Nombre: formato incorrecto\n");
            }else{
                printf("Nombre: %s\n", tok);
            }

            //PRIMER APELLIDO
            tok = strtok(NULL, ",");
            if (validar_datos(tok, 2) == 1){
                printf("Primer apellido: formato incorrecto\n");
            }else{
                printf("Primer apellido: %s\n", tok);
            }

            //SEGUNDO APELLIDO
            tok = strtok(NULL, ",");
            if (validar_datos(tok, 2) == 1){
                printf("Segundo apellido: formato incorrecto\n");
            }else{
                printf("Segundo apellido: %s\n", tok);
            }

            //SEXO
            tok = strtok(NULL, ",");
            if (validar_datos(tok, 3) == 1){//VALIDAR SEXO
                printf("Sexo: formato incorrecto\n");
            }else{
                printf("Sexo: %s\n", tok);
            }

            //FECHA DE NACIMIENTO
            tok = strtok(NULL, "\n");
            if (validar_datos(tok, 4) == 1){//VALIDA FECHA
                printf("Fecha de Nacimiento: formato incorrecto\n");
            }else{
                printf("Fecha de nacimiento: %s\n\n", tok);
            }
            contador ++;
        }
        
    }
    fclose(archivo);
}

int validar_datos(char* dato, int id){
    char data[35];//Para guardar los datos en un arreglo para facilitar su iteración
    int i = 0;//Para recorrer cadenas
    switch (id)
    {
        //VALIDAR PASAPORTE
        case 1:
            strcpy(data, dato);
            if (strlen(dato) != 6){//Si no cumple con los caracteres exactos se descarta
                printf("Pasaporte no tiene 6 caracteres\n");
                return 1;
            }

            for (int i = 0; i<6; i++){
                if (isdigit(data[i]) == 0){
                    printf("%c no es un número\n", data[i]);
                    return 1;
                }

            }
            if (data[0] == '0'){//no puede iniciar por 0
                printf("El pasaporte no puede iniciar con 0\n");
                return 1;
            }
            return 0;
        
        //VALIDAR NOMBRE Y APELLIDOS
        case 2:
            strcpy(data, dato);
            while (i<strlen(dato)){
                if (isdigit(data[i]) != 0){
                    return 1;
                }
                i++;
            }
            return 0;

            break;
        
        //VALIDAR SEXO
        case 3:
            if (strcmp(dato, "F")!=0 && strcmp(dato, "M")!=0){
                return 1;
            }
            return 0;
        
        //VALIDAR FECHA DE NACIMIENTO
        case 4:
            strcpy(data, dato);//Lo copia para pasarlo a un array de caracteres
            if (strlen(dato) != 10){//Si no tiene los 10 caracteres se descarta
                printf("Fecha no tiene número exacto de 10 caracteres\n");
                return 1;
            }

            while (i < 10){
                if (i == 4 || i == 7){//si no hay un guión donde deberia se descarta
                    if (data[i] != '-'){
                        printf("No hay un guión donde debería estar uno\n");
                        return 1;
                    }
                }
                else if (isdigit(data[i]) == 0){//si los demás no son digitos se descarta
                    printf("%c no es un dígito\n", data[i]);
                    return 1;
                }
                i++;
            }
            return 0;
    }
}