#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <mysql.h>
#define EXISTE_AVION "select existeAvion("
#define INSERTAR_AVION "select InsertarAvion("
#define MOSTRAR_AVION "CALL verAvion("
#define VERIFICAR_AVION "select AvionVuelo("


int existeAvion(char pMatricula[]) {
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;

    int res_int;//Guarda el resultado de la query
    char q_existeAvion[200];//Guarda el query para iniciar sesión
    strcpy(q_existeAvion, EXISTE_AVION);//Agrega una parte de la query para la función
     //Se agrega lo que hace falta para completar el query
    strcat(q_existeAvion, pMatricula);
    strcat(q_existeAvion, ")");

    if (mysql_query(conn, q_existeAvion)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);//En row[0] está el valor que retornó la función, un entero indicando si existe la matricula
    
    //Convierte el resultado en un entero
    res_int = (int)row[0][0] -48;

    mysql_free_result(res);
    mysql_close(conn);
    return res_int;

}

char * escogerMarca() {
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;

    int res_int;//Guarda el resultado de la query
    char q_mostrarMarcas[200];//Guarda el query para iniciar sesión
    strcpy(q_mostrarMarcas, "SELECT * FROM Marca");//Agrega una parte de la query para la función

    if (mysql_query(conn, q_mostrarMarcas)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    	
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s", row[0]);
        printf(" : ");
        printf("%s\n", row[1]);
    }
    printf("Seleccione la marca de su preferencia:\n(Debe indicarlo con el número identificador):\n");
    char *marca = malloc(3);
    scanf("%s", &marca);

    mysql_free_result(res);
    mysql_close(conn);
    return marca;
}

char * escogerModelo(char *pMarca) {
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;

    int res_int;//Guarda el resultado de la query
    char q_mostrarModelos[300];//Guarda el query para iniciar sesión
    strcpy(q_mostrarModelos, "SELECT * FROM Modelo WHERE Modelo.fkMarca = ");//Agrega una parte de la query para la función
    strcat(q_mostrarModelos, pMarca);
    strcat(q_mostrarModelos, ";");

    if (mysql_query(conn, q_mostrarModelos)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    	
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s", row[0]);
        printf(" : ");
		printf("%s\n", row[1]);
    }
    printf("Seleccione el modelo de su preferencia.\n(Debe indicarlo con el número identificador): \n");
    char *modelo = malloc(4);
    scanf("%s", &modelo);


    mysql_free_result(res);
    mysql_close(conn);
    return modelo;
}

void incluirAvionMYSQL(char *año, char *modelo, char matricula[]) {
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;

    int res_int;//Guarda el resultado de la query
    char q_insertarAvion[300];//Guarda el query para iniciar sesión
    strcpy(q_insertarAvion, INSERTAR_AVION);//Agrega una parte de la query para la función
    strcat(q_insertarAvion, modelo);
    strcat(q_insertarAvion, ",");
    strcat(q_insertarAvion, matricula);
    strcat(q_insertarAvion, ",");
    strcat(q_insertarAvion, año);
    strcat(q_insertarAvion, ")");

    if (mysql_query(conn, q_insertarAvion)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);//En row[0] está el valor que retornó la función, un entero indicando si existe la matricula
    
    //Convierte el resultado en un entero
    res_int = (int)row[0][0] -48;

    if(res_int==1){
        printf("Avión insertado con éxito!\n");
    }else printf("Algo salió mal\n");

    mysql_free_result(res);
    mysql_close(conn);
    return;
}


void incluirAvion () {
    char matricula[15];
    char año[15];
    
    printf("Matricula: ");
    scanf("%s", matricula);
    
    
    int existe = existeAvion(matricula);
    if (!existe){
       printf("Año: ");
       scanf("%s", año);
       char *marca = escogerMarca();
       char *modelo = escogerModelo(&marca);
       incluirAvionMYSQL(año,&modelo,matricula);
       return;
    }
    else {
        printf("Esta matrícula de avión ya existe\n");
        return;
    }
}
char * escogerMatricula() {
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;

    int res_int;//Guarda el resultado de la query
    char q_mostrarMatricula[300];//Guarda el query para iniciar sesión
    strcpy(q_mostrarMatricula, "SELECT Avion.pkAvion FROM Avion");//Agrega una parte de la query para la función
    strcat(q_mostrarMatricula, ";");

    if (mysql_query(conn, q_mostrarMatricula)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    if((row = mysql_fetch_row(res)) == NULL) {
        return 0;
    }
    	
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("%s", row[0]);
        printf("\n");
    }
    printf("Seleccione la matricula de su preferencia.\n(Debe escribirla tal y como se muestra): \n");
    char *matricula = malloc(80);
    scanf("%s", matricula);

    mysql_free_result(res);
    mysql_close(conn);

    return matricula;
}

void mostrarAvion() {
    char *matricula = escogerMatricula();
    if(matricula==0){
        printf("No hay aviones registrados\n");
        return;
    }
    
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;

    int res_int;//Guarda el resultado de la query
    char q_mostrarAvion[300];//Guarda el query para iniciar sesión
    strcpy(q_mostrarAvion, MOSTRAR_AVION);//Agrega una parte de la query para la función
    strcat(q_mostrarAvion, matricula);
    strcat(q_mostrarAvion, ")");

    if (mysql_query(conn, q_mostrarAvion)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    	
	while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%s", "Matricula: ");
		printf("%s", row[0]);
        printf("\n");
        printf("%s", "Modelo: ");
		printf("%s", row[1]);
        printf("\n");
        printf("%s", "Marca: ");
		printf("%s", row[2]);
        printf("\n");
    }

    mysql_free_result(res);
    mysql_close(conn);
    return;
}
int verificarAvionVuelo(char *pMatricula) {
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;

    printf("%s",pMatricula);
    int res_int;//Guarda el resultado de la query
    char q_verificarAvion[200];//Guarda el query para iniciar sesión
    strcpy(q_verificarAvion, VERIFICAR_AVION);//Agrega una parte de la query para la función
     //Se agrega lo que hace falta para completar el query
    strcat(q_verificarAvion, pMatricula);
    strcat(q_verificarAvion, ")");

    if (mysql_query(conn, q_verificarAvion)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);//En row[0] está el valor que retornó la función, un entero indicando si existe la matricula
    
    //Convierte el resultado en un entero
    res_int = (int)row[0][0] -48;

    mysql_free_result(res);
    mysql_close(conn);
    return res_int;
}

void eliminarAvion() {
    char *matricula = escogerMatricula();
    int registradoConVuelo = verificarAvionVuelo(matricula);
    
    if(registradoConVuelo){
        printf("Este avión está registrado en un vuelo, por lo tanto no se puede eliminar.");
    }
    else {
        MYSQL *conn = mysql_init(NULL);
        conn = conexion_mySQL();
        MYSQL_RES *res;
        MYSQL_ROW row;

        int res_int;//Guarda el resultado de la query
        char q_eliminarAvion[300];//Guarda el query para iniciar sesión
        strcpy(q_eliminarAvion, "DELETE FROM Avion WHERE Avion.pkAvion = ");//Agrega una parte de la query para la función
        strcat(q_eliminarAvion, matricula);

        if (mysql_query(conn, q_eliminarAvion)) {//Ejecuta la función de la base de datos
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

        res = mysql_use_result(conn);

        printf("Avión eliminado con éxito!\n");

        mysql_free_result(res);
        mysql_close(conn);
    }
    return;
}

void menuRegistrarAvion() {
    int eleccion;
    do{
        printf("\n  Registro de Aviones\n");
        printf( " __________________________\n");
        printf( "|                          |\n");
        printf( "| 1.Incluir nuevo avión    |\n");
        printf( "| 2.Eliminar avión         |\n");
        printf( "| 3.Mostrar aviones        |\n");
        printf( "| 0.Salir                  |\n" );
        printf( ".__________________________.\n");
        printf("\nIngrese su elección: ");
        scanf(" %d", &eleccion);
        printf("\n");
        
        switch (eleccion){
            case 1:
                incluirAvion();
                break;
            case 2:
                eliminarAvion();
                break;
            case 3:
                mostrarAvion();
                break;
            case 0:
                break;
        }
    }while(eleccion != 0);

}
