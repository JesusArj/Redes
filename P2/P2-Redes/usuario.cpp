#include "usuario.h"

class usuario
{
    private:
        string username;
        string passwd;
        int puntos;
        int estado;
        int sd;     

    public:
        usuario();
        ~usuario();
    
        usuario(string username, string passwd){
            setUsername(username);
            setPasswd(passwd);
        }
    
        string getUsername(){
            return username;
        }
        string getPasswd(){
            return passwd;
        }
        int getPuntos(){
            return puntos;
        }
        int getEstado(){
            return estado;
        }
        int getSd(){
            return sd;
        }

        void setUsername(string username){
            username = username;
        }
        void setPasswd(string passwd){
            passwd = passwd;
        }
        void setPuntos(int puntos){
            puntos = puntos;
        }
        void setEstado(int estado){
            estado = estado;
        }
        void setSd(int sd){
            sd = sd;
        }

        void registerUser(string username, string passwd){
            ofstream file;
            file.open("login.txt",ios::app);
            file << username << "," << passwd << endl;
            file.close();
        }
        bool login(string username, string passwd){
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
        }
        bool userExist(string username){
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

};
