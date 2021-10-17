#include "usuario.h"
    
    string usuario::getUsername(){
        return username;
    }
    string usuario::getPasswd(){
        return passwd;
    }
    int usuario::getEstado(){
        return estado;
    }
    int usuario::getSd(){
        return sd;
    }

    void usuario::setUsername(string username){
        username = username;
    }
    void usuario::setPasswd(string passwd){
        passwd = passwd;
    }
    void usuario::setEstado(int estado){
        estado = estado;
    }
    void usuario::setSd(int sd){
        sd = sd;
    }

    bool usuario::registerUser(string username, string passwd){
        ofstream file;
        bool resultado = false;
        file.open("login.txt",ios::app);
        if(file.is_open()){
            if(!userExist(username)){
                file << username << "," << passwd << endl;
                file.close();
                resultado = true;
            }
        }
        return resultado;
    }
    bool usuario::login(string username, string passwd){
        if (userExist(username)){
            ifstream file("login.txt");
            string linea;
            bool found = false;
            if(file.is_open()){
                while(getline(file,linea, ',') && !found){
                    if(linea==username){
                        getline(file,linea, ',');
                        if(linea==passwd)
                            found = true;
                    }
                }
                file.close();   
            }
            return found;  
        }
        else{
            return false;
        }
    }
    bool usuario::userExist(string username){
        ifstream file("login.txt");
        string linea;
        bool found = false;
        if(file.is_open()){
            while(getline(file,linea, ',') && !found){
                if(linea==username){
                    found = true;
                }
            }
            file.close();   
        }
        return found;
    }
