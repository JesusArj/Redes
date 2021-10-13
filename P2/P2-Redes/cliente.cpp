#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include "frases.h"
#include "mensajesCliente.h"

using namespace std;

int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd;
	struct sockaddr_in sockname;
	char bufferIn[250];
	char bufferOut[350];
	string refranOculto;
	socklen_t len_sockname;
    	fd_set readfds, auxfds;
    	int salida;
    	int fin = 0;
	
    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}

   
    
	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del 
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  inet_addr("127.0.0.12");

	/* ------------------------------------------------------------------
		Se solicita la conexión con el servidor
	-------------------------------------------------------------------*/
	len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		perror ("Error de conexión");
		exit(1);
	}
    
    //Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    
    FD_SET(0,&readfds);
    FD_SET(sd,&readfds);

    
	/* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	do
	{
        auxfds = readfds;
        salida = select(sd+1,&auxfds,NULL,NULL,NULL);

		cout<<"Introduzca la accion a realizar.\n";
		cout<<"1. Comprar vocal"<<endl;
		cout<<"2. Decir consonante"<<endl;
		cout<<"3. Ver puntuacion"<<endl;
		cout<<"4. Resolver frase"<<endl;
		cout<<"5. Salir"<<endl;
        
        //Tengo mensaje desde el servidor
        if(FD_ISSET(sd, &auxfds)){
            
            bzero(bufferIn,sizeof(bufferIn));
            recv(sd,bufferIn,sizeof(bufferIn),0);
            
			string mensajeRecibido(bufferIn);

			cout<<"\n" << mensajeRecibido << endl;

            if(strcmp(bufferIn,"Demasiados clientes conectados\n") == 0)
                fin =1;
            
            if(strcmp(bufferIn,"Desconexión servidor\n") == 0)
                fin =1;

            if(mensajeRecibido.find("Partida finalizada") != std::string::npos)
				fin = 1;
        }
        else
        {
            //He introducido información por teclado
            if(FD_ISSET(0,&auxfds)){
			int opcion;
			string mensajeEnvio, resolver;
			cin>>opcion;
			switch (opcion)
			{
				case 1:
					cout<<"Ha seleccionado Comprar una vocal. Introduzca la voca a comprar\n";
					char vocal;
					cin >> vocal;
					if (isalpha(vocal)){
						if(isVowel(vocal)){
							mensajeEnvio = introducirVocal(vocal);
						}
					}
					break;
				
				case 2:
					cout<<"Ha seleccionado ver una consonante. Introduzca la consonante a comprobar\n";
					char consonante;
					cin >> consonante;
					if (isalpha(consonante)){
						if(!isVowel(consonante)){
							mensajeEnvio = introducirConsonante(consonante);
						}
					}
					break;

				case 3:
					cout<<"Ha seleccionado ver su puntuacion.\n";
					mensajeEnvio = introducirPuntuacion();
					break;

				case 4:
					cout<<"Ha seleccionado resolver la frase. Por favor, introduzca el refran.\n";
					cin>>resolver;
					mensajeEnvio = resolverFrase(resolver);
					break;
				
				case 5:
					cout<<"Gracias por usar nuestro sistema. Hasta la proxima\n";
					mensajeEnvio = salir();
					break;

				default:
					cout<<"Opcion no valida.\n";
					break;
				}

                bzero(bufferOut,sizeof(bufferOut));
                
				strcpy(bufferOut,mensajeEnvio.c_str());

                if(strcmp(bufferOut,"SALIR\n") == 0)
					fin = 1;
                
                send(sd,bufferOut,sizeof(bufferOut),0);
                
            }
                
        }
        		
    }while(fin == 0);
		
    close(sd);

    return 0;
		
}





















