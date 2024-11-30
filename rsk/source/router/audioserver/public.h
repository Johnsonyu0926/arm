#ifndef PUBLIC_H
#define PUBLIC_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char* exec(const char* cmd);
char* hex_to_string(const char* input);
void network_set(const char* gateway, const char* ipAddress, const char* netMask);

#endif // PUBLIC_H