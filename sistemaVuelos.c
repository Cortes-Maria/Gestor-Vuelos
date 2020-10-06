#include <stdio.h>
#include <string.h>
#include "sistemaVuelos.h"
#include "funciones_admin.h"
#include "estadisticas.h"
#include "aviones.h"
#include "reserva.h"

int main()
{
    menu_admin();
    return 0;
}

void menu(){
    int eleccion;
    int tipo_usuario;
    
    do{
        printf("\n\tMenú Principal\n");
        printf( " _______________________________\n");
        printf( "|                               |\n");
        printf( "| 1. Iniciar sesión             |\n");
        printf( "| 0. Salir                      |\n" );
        printf( "._______________________________.\n");
        printf("\nIngrese su elección: ");
        scanf("%d", &eleccion);
        printf("\n");
        
        switch (eleccion){
            
            case 1:
                tipo_usuario = iniciar_sesion();
                printf("%d", tipo_usuario);
                //agregarReservacion();
                if (tipo_usuario == 1){//Datos no existentes en la base de datos
                    printf("Los datos que ingresó son erroneos\n");
                }
                else if (tipo_usuario == 2){//Usuario admin                    
                    menu_admin();
                }
                else{//usuario cliente
                    menu_cliente();
                }
                break;        
        }
    }while(eleccion != 0);
}

void menu_admin()
{
    int eleccion;
    do{
        printf("\n  Menú Administrador\n");
        printf( " _________________________\n");
        printf( "|                         |\n");
        printf( "| 1. Registro de Aviones  |\n");
        printf( "| 2. Carga de usuarios    |\n");
        printf( "| 3. Estado de vuelo      |\n");
        printf( "| 4. Estadísticas         |\n");
        printf( "| 0. Salir                |\n" );
        printf( "._________________________.\n");
        printf("\nIngrese su elección: ");
        scanf("%d", &eleccion);
        printf("\n");
        
        switch (eleccion){
            
            case 1:
                menuRegistrarAvion();
                break;
            
            case 2:
                carga_usuarios();
                break;
            
            case 3:
                estado_vuelo();
                break;
            
            case 4:
                menuEstadisticas();
                break;
        }
    }while(eleccion != 0);
}

void menu_cliente()
{
    int eleccion;
    char idReserva[10];
    do{
        printf("\n  Menú Cliente\n");
        printf( " __________________________\n");
        printf( "|                          |\n");
        printf( "| 1.Reservación de vuelo   |\n");
        printf( "| 2.Información de reserva |\n");
        printf( "| 0.Salir                  |\n" );
        printf( ".__________________________.\n");
        printf("\nIngrese su elección: ");
        scanf("%d", &eleccion);
        printf("\n");
        
        switch (eleccion){
            
            case 1:
                reservar_vuelo();
                break;
            
            case 2:
                printf("Ingrese el código de su reserva: ");
                scanf( "%s", idReserva);
                consultar_reservacion(idReserva);
                break;
        }
    }while(eleccion != 0);
}
