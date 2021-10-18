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
#include "partida.h"
#include "usuario.h"

using namespace std;

int main (int argc, char* argv[])
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd;
	char* ip = argv[1];
	struct sockaddr_in sockname;
	char bufferIn[250];
	char bufferOut[350];
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
	sockname.sin_port = htons(2050);
	sockname.sin_addr.s_addr = inet_addr(ip);

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

    cout<<"Estas son las acciones que puede realizar.\n";
	cout<<"Iniciar sesión.\n\t Para ello debe escribir USUARIO <suUsuario> y luego PASSWORD <suPasswd>."<<endl;
	cout<<"Registrarse.\n\t Para ello debe escribir REGISTER -u <suUsuario> -p <suPasswd>."<<endl;
	cout<<"Iniciar partida si ya está dentro del sistema.\n\t Para ello debe escribir INICIAR-PARTIDA."<<endl;
	cout<<"Comprar una vocal.\n\t Para ello debe escribir VOCAL <vocal>."<<endl;
	cout<<"Elegir una consonante.\n\t Para ello debe escribir CONSONANTE <consonante>."<<endl;
	cout<<"Ver su puntuacion.\n\t Para ello debe escribir PUNTUACION."<<endl;
	cout<<"Resolver la frase.\n\t Para ello debe escribir RESOLVER <elRefranResuelto>."<<endl;
	cout<<"Salir del sistema.\n\t Para ello debe escribir SALIR."<<endl;

	/* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	do
	{
        auxfds = readfds;
        salida = select(sd+1,&auxfds,NULL,NULL,NULL);
        
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
			string mensajeEnvio;
			bzero(bufferOut,sizeof(bufferOut));
			fgets(bufferOut, sizeof(bufferOut),stdin);
			if(strcmp(bufferOut,"SALIR\n") == 0)
				fin = 1;
			send(sd,bufferOut,sizeof(bufferOut),0);

            }                
        }	
    }while(fin == 0);
		
    close(sd);

    return 0;
		
}





















