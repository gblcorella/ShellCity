/* 
  get_path.h
  Ben Miller

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


struct pathelement *get_path();

struct pathelement
{
  char *element;
  struct pathelement *next;
};
