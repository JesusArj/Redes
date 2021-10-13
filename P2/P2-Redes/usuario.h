#ifndef USUARIO
#define USUARIO

#include <string>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

class Usuario{
    private:
        string Username;
        string Passwd;
        int puntos;
        int estado;
        int sd;

    public:
        string getUsername();
        string getPasswd();
        int getPuntos();
        int getEstado();
        int getSd();

        void setUsername(string username);
        void setPasswd(string passwd);
        void setPuntos(int puntos);
        void setEstado(int estado);
        void setSd(int sd);

        void registerUser(string username, string passwd);
        bool login(string username, string passwd);
        bool userExist(string username);

};


#endif