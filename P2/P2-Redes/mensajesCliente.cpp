#include "mensajesCliente.h"
string mandarUsuario(string usuario)
{
    string cadena = ("USUARIO " + usuario + "\n");
    return cadena; 
}
string mandarPasswd(string passwd)
{
    string cadena = ("PASSWORD " + passwd + "\n");
    return cadena; 
}
string mandarRegistro(string usuario, string passwd)
{
    string cadena = ("REGISTRO -u " + usuario + " -p "+passwd + "\n");
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
