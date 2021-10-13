#include "mensajesCliente.h"
string mandarUsuario(string usuario)
{
    string cadena = ("USUARIO " + usuario + "\n");
    cout << cadena <<endl; 
    return cadena; 
}
string mandarPasswd(string passwd)
{
    string cadena = ("PASSWORD " + passwd + "\n");
    cout << cadena <<endl; 
    return cadena; 
}
string mandarRegistro(string usuario, string contrasena)
{
    string cadena = ("REGISTRO -u " + usuario + " -p "+contrasena + "\n");
    cout << cadena <<endl; 
    return cadena; 
}
string iniciarPartida()
{
    return "INICIAR-PARTIDA"; 
}
string introducirVocal(char vocal)
{
    string voc(1,vocal);
    return ("VOCAL " + voc + "\n"); 
}
string introducirConsonante(char consonante)
{
    string con(1,consonante);
    return ("CONSONANTE " + con + "\n"); 
}
string introducirPuntuacion()
{
    return "PUNTUACION\n"; 
}
string resolverFrase(string frase)
{
    return ("RESOLVER " + frase + "\n"); 
}
string salir()
{
    return "SALIR\n"; 
}