#include <stdio.h>
#include <string.h>

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
        while (fgets(linea, 100, archivo) != NULL){
            printf("Cliente #%d: \n", contador);

            tok = strtok(linea, ",");//Pasaporte
            printf("Pasaporte: %s\n", tok);

            tok = strtok(NULL, ",");
            printf("Nombre: %s\n", tok);

            tok = strtok(NULL, ",");
            printf("Primer apellido: %s\n", tok);

            tok = strtok(NULL, ",");
            printf("Segundo apellido: %s\n", tok);

            tok = strtok(NULL, ",");
            printf("Sexo: %s\n", tok);

            tok = strtok(NULL, ",");
            printf("Fecha de nacimiento: %s\n", tok);

            contador ++;
        }
        
    }
    fclose(archivo);
}