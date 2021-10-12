
#include <string>
#include <time.h>

std::string Refran[] = {
#include "refranes"
} ;

std::string pickRefran(){
    srand(time(NULL));
    std::string refran = Refran[rand() % 30];
}
