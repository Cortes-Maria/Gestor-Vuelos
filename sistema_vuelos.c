#include <stdio.h>
#include "sistema_vuelos.h"
int main()
{
    menu();
    return 0;
}

void menu(){
    /*Esta función se remplazará cuando se pueda determinar el tipo de usuario 
    mediante los datos ingresados*/
    int eleccion;
    
    do{
        printf("\n\tMenú Principal\n");
        printf( " _______________________________\n");
        printf( "|                               |\n");
        printf( "| 1. Cuenta Admin               |\n");
        printf( "| 2. Cuenta Cliente             |\n");
        printf( "| 0. Salir                      |\n" );
        printf( "._______________________________.\n");
        printf("\nIngrese su elección: ");
        scanf("%d", &eleccion);
        printf("\n");
        
        switch (eleccion){
            
            case 1:
                menu_admin();
                break;
            
            case 2:
                menu_cliente();
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
        printf( "| 1.Registro de Aviones   |\n");
        printf( "| 2.Carga de usuarios     |\n");
        printf( "| 3.Estado de vuelo       |\n");
        printf( "| 4.Estadísticas          |\n");
        printf( "| 0.Salir                 |\n" );
        printf( "._________________________.\n");
        printf("\nIngrese su elección: ");
        scanf("%d", &eleccion);
        printf("\n");
        
        switch (eleccion){
            
            case 1:
                //Implementar función;
                break;
            
            case 2:
                //Implementar función
                break;
            
            case 3:
                //Implementar función;
                break;
            
            case 4:
                //Implementar función;
                break;
        }
    }while(eleccion != 0);
}

void menu_cliente()
{
    int eleccion;
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
                //Implementar función;
                break;
            
            case 2:
                //Implementar función
                break;
        }
    }while(eleccion != 0);
}