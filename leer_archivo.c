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
    char tok_char[50];
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
            printf("Pasaporte: %s\n", tok);

            //NOMBRE
            tok = strtok(NULL, ",");//Continua en la misma linea hasta la siguiente llave indicada
            printf("Nombre: %s\n", tok);

            //PRIMER APELLIDO
            tok = strtok(NULL, ",");
            printf("Primer apellido: %s\n", tok);

            //SEGUNDO APELLIDO
            tok = strtok(NULL, ",");
            printf("Segundo apellido: %s\n", tok);

            //SEXO
            tok = strtok(NULL, ",");
            if (validar_datos(tok, 3) == 1){//VALIDAR SEXO
                printf("Sexo: formato incorrecto\n");
            }else{
            printf("Sexo: %s\n", tok);}

            //FECHA DE NACIMIENTO
            tok = strtok(NULL, ",");
            strcpy(tok_char, tok);//COPIA LA FECHA A UN CHAR
            if (validar_datos(tok, 4) == 1){//VALIDA FECHA
                printf("Fecha de Nacimiento: formato incorrecto\n");
            }else{
            printf("Fecha de nacimiento: %s\n\n", tok_char);
            }
            contador ++;
        }
        
    }
    fclose(archivo);
}

int validar_datos(char* dato, int id){
    char data[5];
    switch (id)
    {
        //VALIDAR PASAPORTE
        case 1:
            //Implementar funcionalidad
            break;
        
        //VALIDAR NOMBRE Y APELLIDOS
        case 2:
            //Implementar funcionalidad
            break;
        
        //VALIDAR SEXO
        case 3:
            if (strcmp(dato, "F")!=0 && strcmp(dato, "M")!=0){
                return 1;
            }
            return 0;
        
        //VALIDAR FECHA DE NACIMIENTO
        case 4:
            dato = strtok(dato, "-");//Agarra el año
            int i = 0;//Recorre el año, mes y dia para comprobar que sean dígitos
            int j = 0;//Son los 3 ciclos (año, mes, día)
            int limite = 0;//Indica la cantidad máxima de caracteres (año=4, mes=2, dia=2)
            strcpy(data, dato);//Lo copia para pasarlo a un array de caracteres
            while (j < 3){
                if (j == 0){
                    limite = 4;//Se validaría el formato del año
                }
                if (strlen(data)!=limite){//Se valida que tenga 4 o 2 caracteres
                    return 1;
                }
                while (i<limite){//Verifica que todos los caracteres sean números
                    if (isdigit(data[i]) != 0){//isdigit() retorna 0 si no es un dígito
                        i ++;
                        continue;
                    }
                    return 1;
                }
                limite = 2;//A partir de la segunda vuelta solo queda mes y año (cantidad maxima de 2 caracteres)
                j ++;//se aumenta el contador
                i = 0;//i vuelve a ser 0, para recorrer de nuevos el mes y año
                
                if (j == 2){//Si ya se va a validar el día, se pone de llave un cambio de línea, para que este no se tome en cuenta
                    dato = strtok(NULL, "\n");
                }
                else {
                    dato = strtok(NULL, "-");//Agarra la misma cadena hasta la siguiente llave (-)
                }
                if (dato != NULL){//Para que cuando esté en la última linea no se caiga el programa por ser NULL
                    strcpy(data, dato);
                }
            }
            return 0;
    }
}