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

class usuario{
    private:
        string username;
        string passwd;
        int estado;
        int sd;

    public:
        usuario(int sd, int estado){
            setSd(sd);
            setEstado(estado);
        }
        string getUsername();
        string getPasswd();
        int getEstado();
        int getSd();

        void setUsername(string newusername);
        void setPasswd(string passwd);
        void setEstado(int newestado);
        void setSd(int sd);

        bool registerUser(string username, string passwd);
        bool login(string username, string passwd);
        bool userExist(string username);

};


#endif