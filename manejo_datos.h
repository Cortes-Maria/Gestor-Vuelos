#include <string.h>
#include <ctype.h>
#include <stdio.h>

struct cliente{
    char dato[50];
} clientes[6];


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
            if (strlen(dato) != 10){//Si no tiene los 12 caracteres se descarta
                return 1;
            }
            strcpy(data, dato);//Lo copia para pasarlo a un array de caracteres

            while (i < 10){
                
                if (i == 4 || i == 7){//si no hay un guión donde deberia se descarta
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



void generar_reporte(int linea, int ind, char nombre_archivo[50]){

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


int guardar_datos(char linea[80]){
    /*
    Función encargada de tomar los datos por línea de los clientes en el archivo
    y guarda cada dato en un struct.
    E: Una línea entera del archivo txt con los datos de los clientes
    S: Un 0 si los datos están correctos, un 1 si no lo están
    R: N/A
    */

    char *tok;
    int i = 0;
    tok = strtok(linea, ",");
    
    //0: Pasaporte, 1: Nombre, 2: Apellido1, 3: Apellido2, 4: Sexo, 5: Fecha de Nacimiento
    while (i < 6){
        if (i == 0){//Pasaporte, no necesita comillas simples
            strcpy(clientes[i].dato, tok);
            i++;
            tok = strtok(NULL, ",");
            continue;
        }
        else if (i == 5){//Fecha de nacimiento
            if (validar_datos(tok, 4) == 1){//VALIDA FORMATO DE FECHA
                return 1;
            }
        }
        else if (i != 4){//Nombre y apellidos, sexo no se valida en C
            if (validar_datos(tok, 2) == 1){
                return 1;
            }
        }
        
        strcpy(clientes[i].dato, "'");
        strcat(clientes[i].dato, tok);
        strcat(clientes[i].dato, "'");

        if (i == 4){
            tok = strtok(NULL, "\n");//Elimina el \n para la última iteración
        }
        else if (i == 6){
            break;        
        }else{tok = strtok(NULL, ",");}
        i++;
    }
    
    return 0;
}