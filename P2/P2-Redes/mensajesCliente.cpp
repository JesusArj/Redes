#include "mensajesCliente.h"
string mandarUsuario(string usuario)
{
    string cadena = ("USUARIO " + usuario);
    cout << cadena <<endl; 
    return cadena; 
}
string mandarPasswd(string passwd)
{
    string cadena = ("PASSWORD " + passwd);
    cout << cadena <<endl; 
    return cadena; 
}
string mandarRegistro(string usuario, string contrasena)
{
    string cadena = ("REGISTRO -u " + usuario + " -p "+contrasena);
    cout << cadena <<endl; 
    return cadena; 
}
string iniciarPartida()
{
    return "INICIAR-PARTIDA"; 
}
string introducirVocal(char vocal)
{
    return ("VOCAL "+vocal); 
}
string introducirConsonante(char consonante)
{
    return ("CONSONANTE "+consonante); 
}
string introducirPuntuacion()
{
    return "PUNTUACION"; 
}
string resolverFrase(string frase)
{
    return ("RESOLVER " + frase); 
}
string salir()
{
    return "SALIR"; 
}