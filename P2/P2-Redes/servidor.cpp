#ifndef SERVER
#define SERVER

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <list>
#include <time.h>
#include <arpa/inet.h>
#include "usuario.h"
#include "partida.h"
#define MSG_SIZE 250
#define MAX_CLIENTS 30
#define MAX_GAMES 10


/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
int buscarPosicionPartida(int sd, vector<partida> v);
int buscarPosicionUsuario(int sd, vector<usuario> v);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int * numGames, int arrayClientes[], vector<partida> p, vector<usuario> u);

int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
    int sd, new_sd;
    struct sockaddr_in sockname, from;
    char buffer[MSG_SIZE];
    socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
    int numGames=0; 
    char * aux; //para los string to char *
    vector<usuario> usuariosVec;
    vector<partida> partidasVec;
    list<usuario> enEspera; 
    //contadores
    int i,j,k;
    int recibidos;
    char identificador[MSG_SIZE];
    int on, ret;

    
    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}
    
    	// Activaremos una propiedad del socket para permitir· que otros
    	// sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    	// Esto permite· en protocolos como el TCP, poder ejecutar un
    	// mismo programa varias veces seguidas y enlazarlo siempre al
   	 // mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    	// quedase disponible (TIME_WAIT en el caso de TCP)
    	on=1;
    	ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));



	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2050);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}
	

   	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
	from_len = sizeof (from);


	if(listen(sd,1) == -1){
		perror("Error en la operación de listen");
		exit(1);
	}
    
	//Inicializar los conjuntos fd_set
    	FD_ZERO(&readfds);
    	FD_ZERO(&auxfds);
    	FD_SET(sd,&readfds);
    	FD_SET(0,&readfds);
    
   	
    	//Capturamos la señal SIGINT (Ctrl+c)
    	signal(SIGINT,manejador);
    
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
            
            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0){
                
                
                for(i=0; i<FD_SETSIZE; i++){
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) {
                        
                        if( i == sd){
                            
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else
                            {
                                if(numClientes < MAX_CLIENTS){
                                    arrayClientes[numClientes] = new_sd;
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);
                                    usuario u(new_sd,0);
                                    usuariosVec.push_back(u);
                                
                                    strcpy(buffer, "Bienvenido al sistema\n");
                                
                                    send(new_sd,buffer,sizeof(buffer),0);
                                }
                                else
                                {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Demasiados clientes conectados\n");
                                    send(new_sd,buffer,sizeof(buffer),0);
                                    close(new_sd);
                                }
                                
                            }
                            
                            
                        }
                        else if (i == 0){
                            //Se ha introducido información de teclado
                            bzero(buffer, sizeof(buffer));
                            fgets(buffer, sizeof(buffer),stdin);
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(buffer,"SALIR\n") == 0){
                             
                                for (j = 0; j < numClientes; j++){
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer,"Desconexión servidor\n"); 
                                    send(arrayClientes[j],buffer , sizeof(buffer),0);
                                    usuariosVec.clear();
                                    partidasVec.clear();

                                    close(arrayClientes[j]);
                                    FD_CLR(arrayClientes[j],&readfds);
                                }
                                close(sd);
                                exit(-1);
                                
                            }else{
                                send(arrayClientes[j],buffer , sizeof(buffer),0);
                            }
                            
                        } 
                        else{
                            bzero(buffer,sizeof(buffer));
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                        
                            if(recibidos > 0){
                                //comprobaciones                                
                                string mensajeRec(buffer);

                                if(mensajeRec.find("USUARIO ")==0)
                                {
                                    string userName = mensajeRec.substr(mensajeRec.find(" "),mensajeRec.find(mensajeRec.find("\n")));
                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            if(user.getEstado()==0){
                                                if(user.userExist(userName)){
                                                user.setUsername(userName);
                                                user.setEstado(user.getEstado()+1);
                                                bzero(buffer, sizeof(buffer));
                                                aux = &user.getUsername()[0];              
                                                sprintf(identificador,"Bienvenido %s, introduzca su contraseña.",aux);
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                                }
                                            }
                                            else{
                                                bzero(buffer, sizeof(buffer));              
                                                sprintf(identificador,"-Err. Usuario no encontrado");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                           
                                        }
                                        else{
                                            bzero(buffer, sizeof(buffer));              
                                            sprintf(identificador,"-Err. Accion no permitida");
                                            strcpy(buffer,identificador);
                                            send(i,buffer,sizeof(buffer),0);
                                        }
                                    }
    
                                }else if(mensajeRec.find("PASSWORD ")==0)
                                {
                                    string passwd = mensajeRec.substr(mensajeRec.find(" "),mensajeRec.find(mensajeRec.find("\n")));
                                     for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            if(user.getEstado()==1)
                                            {
                                                if(user.login(user.getUsername(), passwd))
                                                {
                                                    user.setPasswd(passwd);
                                                    user.setEstado(user.getEstado()+1);
                                                    bzero(buffer, sizeof(buffer));              
                                                    aux = &user.getUsername()[0]; 
                                                    sprintf(identificador,"Bienvenido %s a la Ruleta de la fortuna.",aux);
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                }
                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"-Err. Contrasena incorrecta");
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                }
                                            }
                                            else
                                            {
                                                bzero(buffer, sizeof(buffer));              
                                                sprintf(identificador,"-Err. Accion no permitida");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }    
                                        }
                                    }
    
                                }else if(mensajeRec.find("REGISTER -u ")==0){
                                    string username = mensajeRec.substr(mensajeRec.find(" -u "),mensajeRec.find(mensajeRec.find(" -p ")));
                                    string passwd = mensajeRec.substr(mensajeRec.find(" -p "),mensajeRec.find(mensajeRec.find("\n")));
                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            if(user.getEstado()==0)
                                            {
                                                if(user.registerUser(username, passwd))
                                                {
                                                    user.setUsername(username);
                                                    user.setPasswd(passwd);
                                                    user.setEstado(user.getEstado()+2);
                                                    bzero(buffer, sizeof(buffer));    
                                                    aux = &user.getUsername()[0];           
                                                    sprintf(identificador,"Bienvenido %s a la Ruleta de la fortuna.",aux);
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                }
                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"-Err. Error en el registro");
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                }
                                            }
                                            else
                                            {
                                                bzero(buffer, sizeof(buffer));              
                                                sprintf(identificador,"-Err. Accion no permitida");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }    
                                        }
                                    }
                                }else if(mensajeRec.find("INICIAR-PARTIDA")==0){
                                    for(usuario user : usuariosVec){
                                        if(user.getSd() == i){
                                            user.setEstado(user.getEstado()+1);
                                        }
                                        if(user.getEstado() == 4){
                                            enEspera.push_back(user);
                                            bzero(buffer, sizeof(buffer));              
                                            aux = &user.getUsername()[0]; 
                                            sprintf(identificador,"%s , ha sido añadido a la cola de espera", aux);
                                            strcpy(buffer,identificador);
                                            send(i,buffer,sizeof(buffer),0);
                                        }
                                    }
                                
                                }else if(mensajeRec.find("CONSONANTE ")==0){
                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            int pos;
                                            if((pos = buscarPosicionPartida(i,partidasVec)) != 11){
                                                partida p = partidasVec[pos];
                                                if(p.getTurno() == i){
                                                    string consonante = mensajeRec.substr(mensajeRec.find(" "),mensajeRec.find(mensajeRec.find("\n")));
                                                    if(consonante.size() == 1){
                                                        char cons = (char)consonante[0];
                                                        if(isalpha(cons)){
                                                            if(isConsonant(cons)){
                                                                p.getTurno() == p.getSockets()[0] ? pos = 0 : pos =1;
                                                                if(p.rellenarConsonantes(cons,pos)){
                                                                    if(p.getRefranResuelto() == p.getRefranOculto()){
                                                                        bzero(buffer, sizeof(buffer));           
                                                                        aux = &p.getRefranResuelto()[0];    
                                                                        sprintf(identificador,"Correcto. El refran es : %s", aux);
                                                                        strcpy(buffer,identificador);
                                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);
                                                                        aux = &p.getJugadores()[pos][0]; 
                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"Partida finalizada. Ha ganado el jugador %s con %d puntos", aux, p.getPuntos()[pos]);
                                                                        strcpy(buffer,identificador);
                                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                                        usuariosVec[buscarPosicionUsuario(p.getSockets()[1],usuariosVec)].setEstado(3);
                                                                        usuariosVec[buscarPosicionUsuario(p.getSockets()[2],usuariosVec)].setEstado(3);
                                                                        partidasVec.erase(partidasVec.begin() + buscarPosicionPartida(p.getSockets()[0], partidasVec));
                                                                    }
                                                                    else{
                                                                        bzero(buffer, sizeof(buffer)); 
                                                                        aux = &p.getRefranOculto()[0];              
                                                                        sprintf(identificador,"El refran actual : %s", aux);
                                                                        strcpy(buffer,identificador);
                                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"Sigue siendo el turno de %d", p.getTurno());
                                                                        strcpy(buffer,identificador);
                                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);
                                                                    }
                                                                }
                                                                else{
                                                                    bzero(buffer, sizeof(buffer));              
                                                                    sprintf(identificador,"La consonante %c no se encuentra en el refran", cons);
                                                                    strcpy(buffer,identificador);
                                                                    send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                    send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                                    bzero(buffer, sizeof(buffer));
                                                                    aux = &p.getRefranOculto()[0];               
                                                                    sprintf(identificador,"El refran actual : %s", aux);
                                                                    strcpy(buffer,identificador);
                                                                    send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                    send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                                    p.nextTurno();

                                                                    bzero(buffer, sizeof(buffer));              
                                                                    sprintf(identificador,"Ahora es el turno de %d", p.getTurno());
                                                                    strcpy(buffer,identificador);
                                                                    send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                    send(p.getSockets()[1],buffer,sizeof(buffer),0);
                                                                }
                                                            
                                                            }
                                                            else
                                                            {
                                                                bzero(buffer, sizeof(buffer));              
                                                                sprintf(identificador,"-Err. El argumento introducido no es una consonante.");
                                                                strcpy(buffer,identificador);
                                                                send(i,buffer,sizeof(buffer),0);
                                                            }
                                                        }
                                                    }
                                                    else{
                                                        bzero(buffer, sizeof(buffer));              
                                                        sprintf(identificador,"-Err. El argumento introducido no es valido.");
                                                        strcpy(buffer,identificador);
                                                        send(i,buffer,sizeof(buffer),0);
                                                    }
                                                }
                                                else{
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"-Err. No es su turno. Espere a que le corresponda");
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                }
                                            }
                                            else{
                                                bzero(buffer, sizeof(buffer));              
                                                sprintf(identificador,"Su partida aun no ha comenzado. Por favor, espere");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                    }                                        
                                    
                                }else if(mensajeRec.find("VOCAL ")==0){
                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            int pos;
                                            if((pos = buscarPosicionPartida(i,partidasVec)) != 11){
                                                partida p = partidasVec[pos];
                                                if(p.getTurno() == i){
                                                    string vocal = mensajeRec.substr(mensajeRec.find(" "),mensajeRec.find(mensajeRec.find("\n")));
                                                    if(vocal.size() == 1){
                                                        char voc = (char)vocal[0];
                                                        if(isalpha(voc)) {
                                                            if(isVowel(voc)){
                                                                p.getTurno() == p.getSockets()[0] ? pos = 0 : pos =1;
                                                                if(p.comprarVocales(voc,pos)){
                                                                    if(p.getRefranResuelto() == p.getRefranOculto()){
                                                                        bzero(buffer, sizeof(buffer));       
                                                                        aux = &p.getRefranResuelto()[0];        
                                                                        sprintf(identificador,"Correcto. El refran es : %s", aux);
                                                                        strcpy(buffer,identificador);
                                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                                        bzero(buffer, sizeof(buffer));   
                                                                        aux = &p.getJugadores()[pos][0];            
                                                                        sprintf(identificador,"Partida finalizada. Ha ganado el jugador %s con %d puntos", aux, p.getPuntos()[pos]);
                                                                        strcpy(buffer,identificador);
                                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                                        usuariosVec[buscarPosicionUsuario(p.getSockets()[1],usuariosVec)].setEstado(3);
                                                                        usuariosVec[buscarPosicionUsuario(p.getSockets()[2],usuariosVec)].setEstado(3);
                                                                        partidasVec.erase(partidasVec.begin() + buscarPosicionPartida(p.getSockets()[0], partidasVec));
                                                                    }
                                                                    else{
                                                                        bzero(buffer, sizeof(buffer));       
                                                                        aux = &p.getRefranOculto()[0];        
                                                                        sprintf(identificador,"El refran actual : %s", aux);
                                                                        strcpy(buffer,identificador);
                                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"Sigue siendo el turno de %d", p.getTurno());
                                                                        strcpy(buffer,identificador);
                                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);
                                                                    }

                                                                    
                                                                }
                                                                else{
                                                                    bzero(buffer, sizeof(buffer));              
                                                                    sprintf(identificador,"La vocal %c no se encuentra en el refran", voc);
                                                                    strcpy(buffer,identificador);
                                                                    send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                    send(p.getSockets()[1],buffer,sizeof(buffer),0);
                                                                    bzero(buffer, sizeof(buffer));     
                                                                    aux = &p.getRefranOculto()[0];          
                                                                    sprintf(identificador,"El refran actual : %s", aux);
                                                                    strcpy(buffer,identificador);
                                                                    send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                    send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                                    p.nextTurno();

                                                                    bzero(buffer, sizeof(buffer));              
                                                                    sprintf(identificador,"Ahora es el turno de %d", p.getTurno());
                                                                    strcpy(buffer,identificador);
                                                                    send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                                    send(p.getSockets()[1],buffer,sizeof(buffer),0);
                                                                }
                                                            
                                                            }
                                                            else
                                                            {
                                                                bzero(buffer, sizeof(buffer));              
                                                                sprintf(identificador,"-Err. El argumento introducido no es una vocal.");
                                                                strcpy(buffer,identificador);
                                                                send(i,buffer,sizeof(buffer),0);
                                                            }
                                                        }
                                                    }
                                                    else{
                                                        bzero(buffer, sizeof(buffer));              
                                                        sprintf(identificador,"-Err. El argumento introducido no es valido.");
                                                        strcpy(buffer,identificador);
                                                        send(i,buffer,sizeof(buffer),0);
                                                    }
                                                }
                                                else{
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"-Err. No es su turno. Espere a que le corresponda");
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                }
                                            }
                                            else{
                                                bzero(buffer, sizeof(buffer));              
                                                sprintf(identificador,"Su partida aun no ha comenzado. Por favor, espere");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                    }  

                                }else if(mensajeRec.find("RESOLVER ")==0){

                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            int pos;
                                            if((pos = buscarPosicionPartida(i,partidasVec)) != 11){
                                                partida p = partidasVec[pos];
                                                if(p.getTurno() == i){
                                                    string refran = mensajeRec.substr(mensajeRec.find(" "),mensajeRec.find(mensajeRec.find("\n")));
                                                    p.getTurno() == p.getSockets()[0] ? pos = 0 : pos =1;
                                                    if(p.resolverRefran(refran)){
                                                        bzero(buffer, sizeof(buffer));         
                                                        aux = &p.getRefranResuelto()[0];      
                                                        sprintf(identificador,"Correcto. El refran es : %s", aux);
                                                        strcpy(buffer,identificador);
                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);
                                                        bzero(buffer, sizeof(buffer));   
                                                        aux = &p.getJugadores()[pos][0];            
                                                        sprintf(identificador,"Partida finalizada. Ha ganado el jugador %s con %d puntos", aux, p.getPuntos()[pos]);
                                                        strcpy(buffer,identificador);
                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                        usuariosVec[buscarPosicionUsuario(p.getSockets()[1],usuariosVec)].setEstado(3);
                                                        usuariosVec[buscarPosicionUsuario(p.getSockets()[2],usuariosVec)].setEstado(3);
                                                        partidasVec.erase(partidasVec.begin() + buscarPosicionPartida(p.getSockets()[0], partidasVec)); 


                                                    }
                                                    else{
                                                        bzero(buffer, sizeof(buffer));    
                                                        aux = &p.getRefranResuelto()[0];        
                                                        sprintf(identificador,"+Ok. Partida finalizada. Frase: %s. No se ha acertado la frase", aux);
                                                        strcpy(buffer,identificador);
                                                        send(p.getSockets()[0],buffer,sizeof(buffer),0);
                                                        send(p.getSockets()[1],buffer,sizeof(buffer),0);

                                                        usuariosVec[buscarPosicionUsuario(p.getSockets()[1],usuariosVec)].setEstado(3);
                                                        usuariosVec[buscarPosicionUsuario(p.getSockets()[2],usuariosVec)].setEstado(3);
                                                        partidasVec.erase(partidasVec.begin() + buscarPosicionPartida(p.getSockets()[0], partidasVec)); 
                                                    }                                                    
                                                }
                                                else{
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"-Err. No es su turno. Espere a que le corresponda");
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                }
                                            }
                                            else{
                                                bzero(buffer, sizeof(buffer));              
                                                sprintf(identificador,"Su partida aun no ha comenzado. Por favor, espere");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                    }

                                }else if(mensajeRec.find("PUNTUACION")==0){
                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            int pos;
                                            if((pos = buscarPosicionPartida(i,partidasVec)) != 11){
                                                partida p = partidasVec[pos];
                                                if(p.getTurno() == i){
                                                    p.getTurno() == p.getSockets()[0] ? pos = 0 : pos =1;
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"Usted tiene %d puntos", p.getPuntos()[pos]);
                                                    strcpy(buffer,identificador);
                                                    send(p.getSockets()[pos],buffer,sizeof(buffer),0);
                                                }
                                                else{
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"-Err. No es su turno. Espere a que le corresponda");
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                }
                                            }
                                            else{
                                                bzero(buffer, sizeof(buffer));              
                                                sprintf(identificador,"Su partida aun no ha comenzado. Por favor, espere");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                    }
                                }
                                else if(mensajeRec.find("SALIR") == 0){
                                    salirCliente(i,&readfds,&numClientes,&numGames, arrayClientes,partidasVec, usuariosVec);
                                }
                                else {
                                    bzero(buffer, sizeof(buffer));              
                                    sprintf(identificador,"-Err.");
                                    strcpy(buffer,identificador);
                                    send(i,buffer,sizeof(buffer),0);
                                }                           
                                
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numClientes,&numGames, arrayClientes,partidasVec, usuariosVec);
                            }
                        }
                    }
                }
            }
            if(enEspera.size() >=2 && numGames<MAX_GAMES)
            {
                auto it=enEspera.begin(); 
                auto SDAux1 = it; 
                it++; 
                auto SDAux2 = it;
                for(int i=0; i<2; i++)
                {
                    enEspera.pop_front();
                }  
                usuariosVec[buscarPosicionUsuario(SDAux1->getSd(), usuariosVec)].setEstado(5);
                usuariosVec[buscarPosicionUsuario(SDAux2->getSd(), usuariosVec)].setEstado(5);
                partida nuevaPartida(SDAux1->getSd(), SDAux2->getSd(), SDAux1->getUsername(), SDAux2->getUsername());
                nuevaPartida.pickRefran();
                nuevaPartida.ocultarRefran();
                nuevaPartida.setTurno(SDAux1->getSd());
                partidasVec.push_back(nuevaPartida); 
                numGames++; 
                bzero(buffer, sizeof(buffer));
                sprintf(identificador,"La partida va a comenzar. Es el turno de %d",nuevaPartida.getSockets()[0]);
                strcpy(buffer,identificador);
                send(nuevaPartida.getSockets()[0],buffer,sizeof(buffer),0);
                send(nuevaPartida.getSockets()[1],buffer,sizeof(buffer),0);
            }
		}

	close(sd);
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int * numGames, int arrayClientes[], vector<partida> p, vector<usuario> u){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--;

    int pos = buscarPosicionPartida(socket, p);
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión de su rival, escriba INICIAR-PARTIDA para buscar nueva partida.");
    if(socket==p[pos].getSockets()[1])
    {
        send(p[pos].getSockets()[0], buffer, sizeof(buffer), 0);
        //le ponemos al otro usuario el estado = 3
        int auxPosi = buscarPosicionUsuario(p[pos].getSockets()[0] ,u); 
        u[auxPosi].setEstado(3); 
    }
    else if(socket==p[pos].getSockets()[0])
    {
        send(p[pos].getSockets()[1], buffer, sizeof(buffer), 0); 
        //le ponemos al otro usuario el estado = 3
        int auxPosi = buscarPosicionUsuario(p[pos].getSockets()[1] ,u); 
        u[auxPosi].setEstado(3); 
    }
    //quitamos al usuario del vector de usuarios
    int posUsuario = buscarPosicionUsuario(socket, u);
    u.erase(u.begin()+posUsuario);  
    //terminamos la partida y decrementamos el numgames
    p.erase(p.begin() + pos); 
    (*numGames)--; 
}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}


int buscarPosicionUsuario(int sd, vector<usuario> v)
{
    for(int i =0; i<v.size(); i++)
    {
        if(v[i].getSd() == sd )
        {
            return i; 
        }
    }
    return 0; 
}

int buscarPosicionPartida(int sd, vector<partida> v)
{
    for(int i =0; i<v.size(); i++)
    {
        if(v[i].getSockets()[0] == sd || v[i].getSockets()[1] == sd)
        {
            return i; 
        }
    }
    return 11; 
}

#endif