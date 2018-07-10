#include "TResource.h"
#include <stdlib.h>
#include <string.h>

//Setter of the name variable. We must make a copy of the route to avoid name-resource mismatching
void TResource::setName(const char * n) {
    char* copy = (char*) malloc( strlen(n) + 1);
    strcpy(copy, n);

    name = copy;

}