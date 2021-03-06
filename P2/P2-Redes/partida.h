#ifndef PARTIDA
#define PARTIDA

#include <string>
#include <list>
#include "frases.h"

using namespace std;

class partida {
    private:
        int turno; //lo indicas con 0 o 1 para mirar el socket que toque
        string refranResuelto;
        string refranOculto;
        int sockets[2]; //sd de los jugadores;
        string jugadores[2]; //puede ser el login para en el envio poner Turno de <login> en vez de 1 o 2.
        int puntos[2];
        list<char> letrasDichas;

    public:
        partida(int sd1, int sd2, string j1, string j2);

        int getTurno();
        string getRefranResuelto();
        string getRefranOculto();
        int* getSockets();
        string* getJugadores();
        int* getPuntos();
        list<char> getLetras();

        void pickRefran(); 
        void ocultarRefran();
        bool comprarVocales(char vocal, int puntos );
        bool comprobarVocales(char vocal); 
        bool resolverRefran(string refranIntento);
        bool rellenarConsonantes(char consonante, int puntos );
        bool letraDicha(char c);
        void decirLetra(char c);

        void setTurno(int i);
        void setRefranResuelto(string refran);
        void setRefranOculto(string refran);
        void setSockets(int sd1, int sd2);
        void setJugadores(string jugador1, string jugador2);
        void setPuntos(int puntos1, int puntos2);

        void nextTurno();
    
};


#endif