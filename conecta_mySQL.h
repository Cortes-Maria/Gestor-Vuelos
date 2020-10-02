#include <mysql.h>
#include <stdio.h>
#include <string.h>
#define LOGIN "select verificar_usuario('"

MYSQL* conexion_mySQL(){
    MYSQL *conn;
    char *server = "localhost";
	char *user = "root";
	char *password = "root";
	char *database = "gestorVuelos";
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

    return conn;
}

int iniciar_sesion(){
    MYSQL *conn = mysql_init(NULL);
    conn = conexion_mySQL();
    MYSQL_RES *res;
	MYSQL_ROW row;
    
    int res_int;//Guarda el resultado de la query
    char usuario[15];
    char contrasenna[35];
    char q_login[200];//Guarda el query para iniciar sesión

    strcpy(q_login, LOGIN);//Agrega una parte de la query para la función de iniciar sesión
    printf("Si es un usuario admin, ingresar 'admin/' antes de su nombre de usuario\n");
    printf("Usuario: ");
    scanf("%s", usuario);
    strcat(q_login, usuario);//Agrega el usuario al string del query
    printf("Contraseña: ");
    scanf("%s", contrasenna);
    
    //Se agrega lo que hace falta para completar el query
    strcat(q_login, "', '");
    strcat(q_login, contrasenna);
    strcat(q_login, "')");

    if (mysql_query(conn, q_login)) {//Ejecuta la función de la base de datos
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);//En row[0] está el valor que retornó la función, un entero indicando el tipo de usuario
    
    //Convierte el resultado en un entero
    res_int = (int)row[0][0] -48;

    mysql_free_result(res);
    mysql_close(conn);

    return res_int;
}