#include "mensajesCliente.h"
string mandarUsuario(string usuario)
{
    transform(usuario.begin(), usuario.end(), usuario.begin(), ::toupper);
    string cadena = ("USUARIO " + usuario + "\n");
    cout << cadena <<endl; 
    return cadena; 
}
string mandarPasswd(string passwd)
{
    transform(passwd.begin(), passwd.end(), passwd.begin(), ::toupper);
    string cadena = ("PASSWORD " + passwd + "\n");
    cout << cadena <<endl; 
    return cadena; 
}
string mandarRegistro(string usuario, string passwd)
{
    transform(usuario.begin(), usuario.end(), usuario.begin(), ::toupper);
    transform(passwd.begin(), passwd.end(), passwd.begin(), ::toupper);
    string cadena = ("REGISTRO -u " + usuario + " -p "+passwd + "\n");
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
    transform(voc.begin(), voc.end(), voc.begin(), ::toupper);
    return ("VOCAL " + voc + "\n"); 
}
string introducirConsonante(char consonante)
{
    string con(1,consonante);
    transform(con.begin(), con.end(), con.begin(), ::toupper);
    return ("CONSONANTE " + con + "\n"); 
}
string introducirPuntuacion()
{
    return "PUNTUACION\n"; 
}
string resolverFrase(string frase)
{
    transform(frase.begin(), frase.end(), frase.begin(), ::toupper);
    return ("RESOLVER " + frase + "\n"); 
}
string salir()
{
    return "SALIR\n"; 
}