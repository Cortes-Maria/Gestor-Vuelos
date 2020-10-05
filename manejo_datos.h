#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "pdfgen.h"
#include "pdfgen.c"

struct cliente{
    char dato[50];
} clientes[6];


int validar_datos(char* dato, int id){
    /*
      Función que valida los datos de cada linea del archivo de carga de usuarios
      E: el dato a validar y un entero indicando el tipo de dato que se va a validar
      S: un entero indicando si el dato está bien o no
      R: N/A
    */
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
    /*
      Función encargada de generar el reporte de la carga de usuarios
      E: una línea entera el archivo
         un indicador indicando si el registro fue insertado o no en la base
         el nombre del archivo para generar el reporte
      S: N/A
      R: N/A
    */
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

void imprime_asientos(char idVuelo[4], MYSQL *conn){
    /*
      Función encargada de imprimir en la terminal
      los asientos de un vuelo
      E: el código del vuelo, una conexion a mysql
      S: N/A
      R: N/A
    */

    MYSQL_RES *res;
    MYSQL_ROW row;

    char inicio_query[30] = "call obtener_asientos(";
    char fila[4];//Guarda cada que haya cambio de fila en los asientos
    
    strcat(inicio_query, idVuelo);
    strcat(inicio_query, ")");

    //Solicitamos el query que ocupamos
    conn = conexion_mySQL();
    if (mysql_query(conn, inicio_query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);
    row = mysql_fetch_row(res);

    strcpy(fila, row[1]);
    int max = 0;
    int cont = 0;
    //Este while determina cuantas columnas máximo tiene el avión
    while (row != NULL){
        if (strcmp(fila, row[1]) != 0){
            strcpy(fila, row[1]);
            if (cont > max){
                max = cont;
            }
            cont = 0;
        }
        cont++;
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);
    //Este while imprime las columnas
    cont = 0;
    printf("Columnas->");
    while (cont < max){
        if (cont == 0){
            printf("\t%d", cont +1);
        }else{
            printf("\t\t%d", cont +1);
        }
        cont++;
    }
    printf("\n\n");

    mysql_close(conn);
    conn = conexion_mySQL();

    //Este while imprime los asientos, hay que volver a hacer la consulta
    if (mysql_query(conn, inicio_query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);

    strcpy(fila, row[1]);
    printf("Fila #%s", fila);
    while (row != NULL){
        if (strcmp(fila, row[1]) != 0){
            printf("\n\n");
            strcpy(fila, row[1]);
            printf("Fila #%s", fila);
        }
        printf("\t\t%s", row[0]);
        row = mysql_fetch_row(res);
    }
    printf("\n\n");
    mysql_free_result(res);
    mysql_close(conn);
}

void generar_PDF(char idReservacion[10]){
    /*
      Función que se encarga de generar un pdf y
      guardar ahí los datos de la reserva
      E: el código de la reservación
      S: N/A
      R: N/A
    */
    struct pdf_info info = {
        .creator = "My software",
        .producer = "My software",
        .title = "Reservacion",
        .author = "My name",
        .subject = "My subject",
        .date = "Today"};
    struct pdf_doc *pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
    pdf_set_font(pdf, "Times-Roman");
    pdf_append_page(pdf);

    char codigo[50] = "Codigo de la reservación: ";
    strcat(codigo, idReservacion);
    pdf_add_text(pdf, NULL, codigo, 12, 100, 750, PDF_BLACK);
    char query[50] = "call obtenerReservacion(";
    strcat(query, idReservacion);
    strcat(query, ")");
    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = conexion_mySQL();
    char nombrePDF[50];
    strcpy(nombrePDF, idReservacion);
    strcat(nombrePDF, ".pdf");
    if (mysql_query(conn, query)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);

    char nombreAerolinea[50] = "Aerolínea: ";
    strcat(nombreAerolinea, row[7]);
    strcat(nombreAerolinea, "; Número de hub: ");
    strcat(nombreAerolinea, row[8]);

    char fechaRes[40] = "Fecha de la reservación: ";
    strcat(fechaRes, row[0]);

    char idVuelo[35] = "Código de vuelo: ";
    strcat(idVuelo, row[1]);

    char ciudadSalida[50] = "Ciudad y fecha de salida: ";
    strcat(ciudadSalida, row[2]);
    strcat(ciudadSalida, " ");
    strcat(ciudadSalida, row[3]);

    char ciudadLlegada[50] = "Ciudad y fecha de llegada: ";
    strcat(ciudadLlegada, row[4]);
    strcat(ciudadLlegada, " ");
    strcat(ciudadLlegada, row[5]);

    char montoTotal[50] = "Monto total de la reservación: ";
    strcat(montoTotal, row[6]);
    strcat(montoTotal, " dólares");

    char totalAsientos[30] = "Hay ";
    strcat(totalAsientos, row[9]);
    strcat(totalAsientos, " asientos reservados");

    mysql_free_result(res);
    mysql_close(conn);
    pdf_add_text(pdf, NULL, nombreAerolinea, 12, 100, 730, PDF_BLACK);
    pdf_add_text(pdf, NULL, fechaRes, 12, 100, 710, PDF_BLACK);
    pdf_add_text(pdf, NULL, idVuelo, 12, 100, 690, PDF_BLACK);
    pdf_add_text(pdf, NULL, ciudadSalida, 12, 100, 670, PDF_BLACK);
    pdf_add_text(pdf, NULL, ciudadLlegada, 12, 100, 650, PDF_BLACK);
    pdf_add_text(pdf, NULL, montoTotal, 12, 100, 630, PDF_BLACK);
    pdf_add_text(pdf, NULL, totalAsientos, 12, 100, 610, PDF_BLACK);
    pdf_add_text(pdf, NULL, "Clientes en esta reserva", 12, 100, 570, PDF_BLACK);
    
    strcpy(query, "call obtener_query_r(");
    strcat(query, idReservacion);
    strcat(query, ")");
    conn = conexion_mySQL();

    if (mysql_query(conn, query)) {
        return;
	}
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    int posTexto = 550;

    char nombreCopy[15] = "Nombre: ";
    char nombre[100];

    char pasaporteCopy[20] = "Pasaporte: ";
    char pasaporte[30];

    char edadCopy[15] = "Edad: ";
    char edad[15];

    char asientoCopy1[20] = "Asiento tipo ";
    char asientoCopy2[20] = " ubicado en la fila ";
    char asientoCopy3[20] = " y columna ";
    char asiento[100];

    char precioCopy[25] = "Precio del boleto ";
    char precioCopy2[15] = " dólares";
    char precio[50];

    while (row != NULL){
        //Nombre completo
        strcpy(nombre, nombreCopy);
        strcat(nombre, row[1]);
        strcat(nombre, " ");
        strcat(nombre, row[2]);
        strcat(nombre, " ");
        strcat(nombre, row[3]);
        printf("Nombre: %s\n", nombre);
        pdf_add_text(pdf, NULL, nombre, 12, 100, posTexto, PDF_BLACK);
        posTexto -= 15;

        //Pasaporte
        strcpy(pasaporte, pasaporteCopy);
        strcat(pasaporte, row[4]);
        printf("Pasaporte: %s\n", pasaporte);
        pdf_add_text(pdf, NULL, pasaporte, 12, 100, posTexto, PDF_BLACK);
        posTexto -= 15;

        //Edad
        strcpy(edad, edadCopy);
        strcat(edad, row[9]);
        printf("Edad: %s\n", edad);
        pdf_add_text(pdf, NULL, edad, 12, 100, posTexto, PDF_BLACK);
        posTexto -= 15;

        //TipoAsiento
        strcpy(asiento, asientoCopy1);
        strcat(asiento, row[5]);
        strcat(asiento, asientoCopy2);
        strcat(asiento, row[6]);
        strcat(asiento, asientoCopy3);
        strcat(asiento, row[7]);
        printf("Asiento: %s\n", asiento);
        pdf_add_text(pdf, NULL, asiento, 12, 100, posTexto, PDF_BLACK);
        posTexto -= 15;

        //Monto
        strcpy(precio, precioCopy);
        strcat(precio, row[8]);
        strcat(precio, precioCopy2);
        printf("Precio: %s\n\n\n", precio);
        pdf_add_text(pdf, NULL, precio, 12, 100, posTexto, PDF_BLACK);

        row = mysql_fetch_row(res);
        posTexto -= 30;
    }

    pdf_save(pdf, nombrePDF);
    pdf_destroy(pdf);

}
