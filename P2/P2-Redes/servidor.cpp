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

using namespace std;

/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
int buscarPosicionPartida(int sd, vector<partida> v);
int buscarPosicionUsuario(int sd, vector<usuario> v);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int * numGames, int arrayClientes[], vector<partida> &p, vector<usuario> &u);

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
    char comando[20];
    char args[25];
    char fraseArg[200];
    char PasswdArg[25];
    char * aux; //para los string to char *
    vector<usuario> usuariosVec;
    vector<partida> partidasVec;
    list<usuario> enEspera; 
    list<char> letrasDichas;
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
		while(1)
        {
            
            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0)
            {
                
                for(i=0; i<FD_SETSIZE; i++)
                {
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) 
                    {
                        if( i == sd)
                        {
                            
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
                            {
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
                                
                                    strcpy(buffer, "+Ok. Usuario conectado\n");
                                
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
                        else if (i == 0)
                        {
                            //Se ha introducido información de teclado
                            bzero(buffer, sizeof(buffer));
                            fgets(buffer, sizeof(buffer),stdin);
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(buffer,"SALIR\n") == 0)
                            {
                             
                                for (j = 0; j < numClientes; j++)
                                {
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
                                
                            }
                            else
                            {
                                send(arrayClientes[j],buffer , sizeof(buffer),0);
                            }
                            
                        } 
                        else
                        {
                            bzero(buffer,sizeof(buffer));
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                        
                            if(recibidos > 0)
                            {
                                //comprobaciones                                
                                string mensajeRec(buffer);
                                if(mensajeRec.find("USUARIO ")==0)
                                {
                                    sscanf(buffer, "USUARIO %s\n",args);
                                    string userName(args);
                                    for(auto it = usuariosVec.begin(); it!= usuariosVec.end(); it++){
                                        if((*it).getSd()==i){
                                            if(!isLogged(userName, usuariosVec)){
                                                if((*it).userExist(userName))
                                                {
                                                    if((*it).getEstado()==0) 
                                                    {
                                                        (*it).setUsername(args);
                                                        (*it).setEstado(1);
                                                        bzero(buffer, sizeof(buffer));
                                                        aux = &(*it).getUsername()[0];              
                                                        sprintf(identificador,"+Ok. Usuario correcto. Bienvenido, %s, introduzca su contraseña.",aux);
                                                        strcpy(buffer,identificador);
                                                        send(i,buffer,sizeof(buffer),0);
                                                        break; 
                                                    }
                                                    else
                                                    {
                                                        bzero(buffer, sizeof(buffer));              
                                                        sprintf(identificador,"-Err. Accion no permitida");
                                                        strcpy(buffer,identificador);
                                                        send(i,buffer,sizeof(buffer),0);
                                                    }    
                                                }
                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"-Err. Usuario no reconocido.");
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                }
                                            }
                                            else
                                            {
                                                bzero(buffer, sizeof(buffer));              
                                                sprintf(identificador,"-Err. Usuario ya loggeado.");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                        else if(it== usuariosVec.end()-1){
                                            bzero(buffer, sizeof(buffer));              
                                            sprintf(identificador,"-Err. Usuario incorrecto");
                                            strcpy(buffer,identificador);
                                            send(i,buffer,sizeof(buffer),0);
                                        }
                                    }

                                }
                                else if(mensajeRec.find("PASSWORD ")==0)
                                {
                                    sscanf(buffer, "PASSWORD %s\n",PasswdArg);
                                    string passwd(PasswdArg);
                                     for(auto it=usuariosVec.begin(); it!=usuariosVec.end(); it++){
                                        if((*it).getSd()==i){
                                            if((*it).getEstado()==1)
                                            {
                                                if((*it).login((*it).getUsername(), passwd))
                                                {
                                                    (*it).setPasswd(passwd);
                                                    (*it).setEstado(2);
                                                    bzero(buffer, sizeof(buffer));              
                                                    aux = &(*it).getUsername()[0]; 
                                                    sprintf(identificador,"+Ok. Usuario validado. Bienvenido %s al sistema.",aux);
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                    break; 
                                                }
                                                else
                                                {
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"-Err. Error en la validacion.");
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
    
                                }
                                else if(mensajeRec.find("REGISTER -u ")==0)
                                {
                                    sscanf(buffer, "REGISTER -u %s -p %s\n",args, PasswdArg);
                                    string username(args);
                                    string passwd(PasswdArg);
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
                                                    sprintf(identificador,"+Ok. Registro correcto. Bienvenido %s al sistema.",aux);
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
                                }
                                else if(mensajeRec.find("INICIAR-PARTIDA")==0)
                                {
                                    
                                    for(auto it = usuariosVec.begin(); it!=usuariosVec.end(); it++){
                                        if((*it).getSd() == i){
                                            if((*it).getEstado()==2)
                                            { 
                                                (*it).setEstado(3);
                                                enEspera.push_back(*it);
                                                bzero(buffer, sizeof(buffer));              
                                                aux = &(*it).getUsername()[0]; 
                                                sprintf(identificador,"+Ok. Peticion recibida, %s. Quedamos a la espera de mas jugadores.", aux);
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                        else if ((*it).getSd() == i && (*it).getEstado()!=2)
                                        {
                                            bzero(buffer, sizeof(buffer)); 
                                            sprintf(identificador,"-Err Accion no permitida");
                                            strcpy(buffer,identificador);
                                            send(i,buffer,sizeof(buffer),0); 
                                        }
                                    }
                                
                                }
                                else if(mensajeRec.find("CONSONANTE ")==0)
                                {
                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            int pos;
                                            int turn;
                                            int auxProximoTurno;  
                                            if((pos = buscarPosicionPartida(i,partidasVec)) != 11){
                                                if(partidasVec[pos].getTurno() == i){
                                                    sscanf(buffer, "CONSONANTE %s\n",args);
                                                    string consonante(args);
                                                    if(consonante.size() == 1){
                                                        char cons = (char)consonante[0];
                                                        cons = toupper(cons);
                                                        if(isalpha(cons) && isConsonant(cons)){
                                                            partidasVec[pos].getTurno() == partidasVec[pos].getSockets()[0] ? turn=0 : turn = 1;
                                                            bool yaDicho = false;
                                                            for(char c : letrasDichas){
                                                                if(c==cons) 
                                                                    yaDicho = true;
                                                            }
                                                            if(!yaDicho)
                                                            {
                                                                if(partidasVec[pos].rellenarConsonantes(cons,turn)){
                                                                    letrasDichas.push_back(cons);
                                                                    if(partidasVec[pos].getRefranResuelto().compare(partidasVec[pos].getRefranOculto())==0)
                                                                    {
                                                                        bzero(buffer, sizeof(buffer));     
                                                                        sprintf(identificador,"+Ok. Correcto. El refran es : %s", partidasVec[pos].getRefranResuelto().c_str());
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);
                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"+Ok. Partida finalizada. Ha ganado el jugador %s con %d puntos", partidasVec[pos].getJugadores()[turn].c_str(), partidasVec[pos].getPuntos()[turn]);
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);

                                                                        usuariosVec[buscarPosicionUsuario(partidasVec[pos].getSockets()[0],usuariosVec)].setEstado(2);
                                                                        usuariosVec[buscarPosicionUsuario(partidasVec[pos].getSockets()[1],usuariosVec)].setEstado(2);
                                                                        partidasVec.erase(partidasVec.begin() + buscarPosicionPartida(partidasVec[pos].getSockets()[0], partidasVec));
                                                                        break; 
                                                                    }
                                                                    else
                                                                    { 
                                                                        bzero(buffer, sizeof(buffer));             
                                                                        sprintf(identificador,"+Ok. El refran actual : %s", partidasVec[pos].getRefranOculto().c_str());
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);

                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"+Ok. Sigue siendo el turno de %s", partidasVec[pos].getJugadores()[turn].c_str());
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);
                                                                        break; 
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    bzero(buffer, sizeof(buffer));              
                                                                    sprintf(identificador,"+Ok. La consonante %c no se encuentra en el refran", cons);
                                                                    strcpy(buffer,identificador);
                                                                    send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                    send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0); 
                                                                    bzero(buffer, sizeof(buffer));               
                                                                    sprintf(identificador,"+Ok. El refran actual : %s", partidasVec[pos].getRefranOculto().c_str());
                                                                    strcpy(buffer,identificador);
                                                                    send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                    send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);

                                                                    partidasVec[pos].nextTurno();
                                                                    turn == 0 ? auxProximoTurno=1 : auxProximoTurno=0;
                                                                    bzero(buffer, sizeof(buffer));              
                                                                    sprintf(identificador,"+Ok. Ahora es el turno de %s", partidasVec[pos].getJugadores()[auxProximoTurno].c_str());
                                                                    strcpy(buffer,identificador);
                                                                    send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                    send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);
                                                                    break; 
                                                                }
                                                            }
                                                            else
                                                            {
                                                                bzero(buffer, sizeof(buffer));              
                                                                sprintf(identificador,"-Err. La consonante aportada ya ha sido dicha.");
                                                                strcpy(buffer,identificador);
                                                                send(i,buffer,sizeof(buffer),0);
                                                                break; 
                                                            }
                                                        }
                                                        else
                                                        {
                                                            bzero(buffer, sizeof(buffer));              
                                                            sprintf(identificador,"-Err. El argumento introducido no es una consonante.");
                                                            strcpy(buffer,identificador);
                                                            send(i,buffer,sizeof(buffer),0);
                                                            break; 
                                                        }
                                                    }
                                                    else{
                                                        bzero(buffer, sizeof(buffer));              
                                                        sprintf(identificador,"-Err. El argumento introducido no es valido.");
                                                        strcpy(buffer,identificador);
                                                        send(i,buffer,sizeof(buffer),0);
                                                        break; 
                                                    }
                                                }
                                                else{
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"-Err. No es su turno. Espere a que le corresponda");
                                                    strcpy(buffer,identificador);
                                                    send(i,buffer,sizeof(buffer),0);
                                                    break; 
                                                }
                                            }
                                            else{
                                                bzero(buffer, sizeof(buffer));              
                                                sprintf(identificador,"-Err. Su partida aun no ha comenzado. Por favor, espere");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                                break; 
                                            }
                                        }
                                    }                                        
                                    
                                }
                                else if(mensajeRec.find("VOCAL ")==0)
                                {
                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            int pos;
                                            int turn;
                                            int auxProximoTurno; 
                                            if((pos = buscarPosicionPartida(i,partidasVec)) != 11){
                                                if(partidasVec[pos].getTurno() == i){
                                                partidasVec[pos].getTurno() == partidasVec[pos].getSockets()[0] ? turn = 0 : turn =1;
                                                    if(partidasVec[pos].getPuntos()[turn]<50)
                                                    {
                                                        bzero(buffer, sizeof(buffer));              
                                                        sprintf(identificador,"+Err No tienes puntos suficientes.");
                                                        strcpy(buffer,identificador);
                                                        send(partidasVec[pos].getSockets()[turn],buffer,sizeof(buffer),0);
                                                    }
                                                    else
                                                    {
                                                        sscanf(buffer, "VOCAL %s\n",args);
                                                        string vocal(args);
                                                        if(vocal.size() == 1){
                                                            char voc = (char)vocal[0];
                                                            voc = toupper(voc);
                                                            if(isalpha(voc) && isVowel(voc)) 
                                                            {
                                                                bool yaDicho = false;
                                                                for(char c : letrasDichas){
                                                                    if(c==voc) 
                                                                        yaDicho = true;
                                                                }
                                                                if(!yaDicho)
                                                                {
                                                                    if(partidasVec[pos].comprobarVocales(voc) && partidasVec[pos].getRefranResuelto().compare(partidasVec[pos].getRefranOculto())==0)
                                                                    {
                                                                        letrasDichas.push_back(voc);
                                                                        partidasVec[pos].comprarVocales(voc,turn); 
                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"+Ok. Correcto. El refran es : %s", partidasVec[pos].getRefranResuelto().c_str());
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);

                                                                        bzero(buffer, sizeof(buffer));          
                                                                        sprintf(identificador,"+Ok. Partida finalizada. Ha ganado el jugador %s con %d puntos", partidasVec[pos].getJugadores()[turn].c_str(), partidasVec[pos].getPuntos()[turn]);
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);

                                                                        usuariosVec[buscarPosicionUsuario(partidasVec[pos].getSockets()[0],usuariosVec)].setEstado(2);
                                                                        usuariosVec[buscarPosicionUsuario(partidasVec[pos].getSockets()[1],usuariosVec)].setEstado(2);
                                                                        partidasVec.erase(partidasVec.begin() + buscarPosicionPartida(partidasVec[pos].getSockets()[0], partidasVec));
                                                                        }
                                                                    else if(partidasVec[pos].comprobarVocales(voc))
                                                                    {
                                                                        letrasDichas.push_back(voc);
                                                                        partidasVec[pos].comprarVocales(voc,turn); 
                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"+Ok. El refran actual : %s", partidasVec[pos].getRefranOculto().c_str());
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);

                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"+Ok. Sigue siendo el turno de %s", partidasVec[pos].getJugadores()[turn].c_str());
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);
                                                                    }
                                                                    else{
                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"+Ok. La vocal %c no se encuentra en el refran", voc);
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);
                                                                        bzero(buffer, sizeof(buffer));    
                                                                        sprintf(identificador,"+Ok. El refran actual : %s", partidasVec[pos].getRefranOculto().c_str());
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);

                                                                        partidasVec[pos].nextTurno();
                                                                        turn == 0 ? auxProximoTurno=1 : auxProximoTurno=0; 
                                                                        bzero(buffer, sizeof(buffer));              
                                                                        sprintf(identificador,"+Ok. Ahora es el turno de %s", partidasVec[pos].getJugadores()[auxProximoTurno].c_str());
                                                                        strcpy(buffer,identificador);
                                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);
                                                                    }
                                                                }
                                                                else{
                                                                bzero(buffer, sizeof(buffer));              
                                                                sprintf(identificador,"-Err. La vocal a comprar ya ha sido dicha.");
                                                                strcpy(buffer,identificador);
                                                                send(i,buffer,sizeof(buffer),0);
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
                                                        else{
                                                            bzero(buffer, sizeof(buffer));              
                                                            sprintf(identificador,"-Err. El argumento introducido no es valido.");
                                                            strcpy(buffer,identificador);
                                                            send(i,buffer,sizeof(buffer),0);
                                                        }
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
                                                sprintf(identificador,"-Err. Su partida aun no ha comenzado. Por favor, espere");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                    }  

                                }
                                else if(mensajeRec.find("RESOLVER ")==0)
                                {
                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            int pos;
                                            int turn; 
                                            if((pos = buscarPosicionPartida(i,partidasVec)) != 11){
                                                if(partidasVec[pos].getTurno() == i){
                                                    
                                                    string refran(buffer);
                                                    int posicion = refran.find(" "); 
                                                    refran = refran.substr(posicion+1);
                                                    for_each(refran.begin(), refran.end(), [](char &c){
                                                        c = ::toupper(c);
                                                    });
                                                    partidasVec[pos].getTurno() == partidasVec[pos].getSockets()[0] ? turn = 0 : turn =1;
                                                    if(partidasVec[pos].resolverRefran(refran))
                                                    {
                                                        bzero(buffer, sizeof(buffer));     
                                                        sprintf(identificador,"+Ok. Correcto. El refran es : %s", partidasVec[pos].getRefranResuelto().c_str());
                                                        strcpy(buffer,identificador);
                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);
                                                        bzero(buffer, sizeof(buffer));             
                                                        sprintf(identificador,"+Ok. Partida finalizada. Ha ganado el jugador %s con %d puntos", partidasVec[pos].getJugadores()[turn].c_str(), partidasVec[pos].getPuntos()[turn]);
                                                        strcpy(buffer,identificador);
                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);

                                                        usuariosVec[buscarPosicionUsuario(partidasVec[pos].getSockets()[0],usuariosVec)].setEstado(2);
                                                        usuariosVec[buscarPosicionUsuario(partidasVec[pos].getSockets()[1],usuariosVec)].setEstado(2);
                                                        partidasVec.erase(partidasVec.begin() + buscarPosicionPartida(partidasVec[pos].getSockets()[0], partidasVec)); 
                                                    }
                                                    else
                                                    {
                                                        bzero(buffer, sizeof(buffer));          
                                                        sprintf(identificador,"+Ok. Partida finalizada. Frase: %s. No se ha acertado la frase", partidasVec[pos].getRefranResuelto().c_str());
                                                        strcpy(buffer,identificador);
                                                        send(partidasVec[pos].getSockets()[0],buffer,sizeof(buffer),0);
                                                        send(partidasVec[pos].getSockets()[1],buffer,sizeof(buffer),0);

                                                        usuariosVec[buscarPosicionUsuario(partidasVec[pos].getSockets()[0],usuariosVec)].setEstado(2);
                                                        usuariosVec[buscarPosicionUsuario(partidasVec[pos].getSockets()[1],usuariosVec)].setEstado(2);
                                                        partidasVec.erase(partidasVec.begin() + buscarPosicionPartida(partidasVec[pos].getSockets()[turn], partidasVec)); 
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
                                                sprintf(identificador,"-Err. Su partida aun no ha comenzado. Por favor, espere");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                    }

                                }
                                else if(mensajeRec.find("PUNTUACION")==0)
                                {
                                    for(usuario user : usuariosVec){
                                        if(user.getSd()==i){
                                            int pos;
                                            int turn; 
                                            if((pos = buscarPosicionPartida(i,partidasVec)) != 11){
                                                partida p = partidasVec[pos];
                                                if(p.getTurno() == i){
                                                    p.getTurno() == p.getSockets()[0] ? turn = 0 : turn =1;
                                                    bzero(buffer, sizeof(buffer));              
                                                    sprintf(identificador,"+Ok. Usted tiene %d puntos", p.getPuntos()[turn]);
                                                    strcpy(buffer,identificador);
                                                    send(p.getSockets()[turn],buffer,sizeof(buffer),0);
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
                                                sprintf(identificador,"-Err. Su partida aun no ha comenzado. Por favor, espere");
                                                strcpy(buffer,identificador);
                                                send(i,buffer,sizeof(buffer),0);
                                            }
                                        }
                                    }
                                }
                                else if(mensajeRec.find("SALIR") == 0)
                                {
                                    salirCliente(i,&readfds,&numClientes,&numGames, arrayClientes,partidasVec, usuariosVec);
                                }
                                
                                else
                                {
                                    bzero(buffer, sizeof(buffer));              
                                    sprintf(identificador,"-Err. Solicitud rechazada.");
                                    strcpy(buffer,identificador);
                                    send(i,buffer,sizeof(buffer),0);
                                } 
                                
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numClientes,&numGames, arrayClientes, partidasVec, usuariosVec);
                            }
                        }
                    }
                }
            }
            if(!enEspera.empty()  && enEspera.size() >=2 && numGames<MAX_GAMES)
            {
                auto it=enEspera.begin(); 
                auto SDAux1 = it; 
                it++; 
                auto SDAux2 = it;
                for(int i=0; i<2; i++)
                {
                    enEspera.pop_front();
                }
                int sd1 = SDAux1->getSd();
                int sd2 = SDAux2->getSd();
                
                string user1 = SDAux1->getUsername();
                string user2 = SDAux2->getUsername();
                int pos1 = buscarPosicionUsuario(sd1, usuariosVec);
                int pos2 = buscarPosicionUsuario(sd2, usuariosVec);
                
                usuariosVec[pos1].setEstado(4);
                usuariosVec[pos2].setEstado(4); 
                partida nuevaPartida(sd1,sd2,user1,user2); 
                cout<< nuevaPartida.getSockets()[0]<<endl; 
                cout<< nuevaPartida.getSockets()[1]<<endl;
                nuevaPartida.pickRefran(); 
                cout<< nuevaPartida.getRefranResuelto(); 
                nuevaPartida.ocultarRefran(); 
                nuevaPartida.setTurno(sd1);
                partidasVec.push_back(nuevaPartida); 
                numGames++; 
                bzero(buffer, sizeof(buffer));
                aux= &nuevaPartida.getJugadores()[0][0];
                sprintf(identificador,"+Ok. La partida va a comenzar. Frase: %s.", nuevaPartida.getRefranOculto().c_str());
                strcpy(buffer,identificador);
                send(nuevaPartida.getSockets()[0],buffer,sizeof(buffer),0);
                send(nuevaPartida.getSockets()[1],buffer,sizeof(buffer),0); 

                sprintf(identificador,"+Ok. Es el turno de %s.", aux);
                strcpy(buffer,identificador);
                send(nuevaPartida.getSockets()[0],buffer,sizeof(buffer),0);
                send(nuevaPartida.getSockets()[1],buffer,sizeof(buffer),0); 
            }
            
	    }

	close(sd);
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int * numGames, int arrayClientes[], vector<partida>& p, vector<usuario>& u){
  
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
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión de su rival, escriba INICIAR-PARTIDA para buscar nueva partida.");
    int posiUsuario = buscarPosicionUsuario(socket, u);
    if(u[posiUsuario].getEstado()>3)
    {
        int pos = buscarPosicionPartida(socket, p);
        if(socket==p[pos].getSockets()[1])
        {
            //le ponemos al otro usuario el estado = 2
            int auxPosi = buscarPosicionUsuario(p[pos].getSockets()[0] ,u); 
            u[auxPosi].setEstado(2); 
            send(p[pos].getSockets()[0], buffer, sizeof(buffer), 0);
        }
        else if(socket==p[pos].getSockets()[0])
        {
            //le ponemos al otro usuario el estado = 2
            int auxPosi = buscarPosicionUsuario(p[pos].getSockets()[1] ,u); 
            u[auxPosi].setEstado(2); 
            send(p[pos].getSockets()[1], buffer, sizeof(buffer), 0); 
        }
        //terminamos la partida y decrementamos el numgames
        p.erase(p.begin() + pos); 
        (*numGames)--; 
    } 

    //quitamos al usuario del vector de usuarios
    int posUsuario = buscarPosicionUsuario(socket, u);
    u.erase(u.begin()+posUsuario);  
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