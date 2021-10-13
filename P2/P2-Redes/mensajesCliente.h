#include <stdio.h> 
#include <string>
#include <iostream>
#include <algorithm>
#ifndef MENSAJES_CLIENTE
using namespace std; 

string mandarUsuario(string usuario);
string mandarPasswd(string passwd); 
string mandarRegistro(string usuario, string contrasena); 
string iniciarPartida(); 
string introducirVocal(char vocal); 
string introducirConsonante(char consonante); 
string introducirPuntuacion(); 
string resolverFrase(string frase); 
string salir(); 
#endif