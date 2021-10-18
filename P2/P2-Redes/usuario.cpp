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

    void usuario::setUsername(string newusername){
        username = newusername;
    }
    void usuario::setPasswd(string newpasswd){
        passwd = newpasswd;
    }
    void usuario::setEstado(int newestado){
        estado = newestado;
    }
    void usuario::setSd(int newsd){
        sd = newsd;
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
                        getline(file,linea, '\n');
                        if(linea==passwd)
                            found = true;
                    }
                    else
                    {
                        getline(file,linea); 
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
                else
                {
                    getline(file,linea); 
                }
            }
            file.close();   
        }
        return found;
    }
