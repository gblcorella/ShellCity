/*
  get_path.c
  Ben Miller

  Just a little sample function that gets the PATH env var, parses it and
  puts "components" into a linked list, which is returned.
*/
#include "get_path.h"

struct pathelement *get_path()
{

  char *path, *p;
  struct pathelement *tmp, *pathlist = NULL;

  p = getenv("PATH");
  path = malloc((strlen(p)+1)*sizeof(char));
  strncpy(path, p, strlen(p));
  path[strlen(p)] = '\0';

  p = strtok(path, ":");
  do
  {
    if ( !pathlist )
    {
      tmp = calloc(1, sizeof(struct pathelement));
      pathlist = tmp;
    }
    else
    {
      tmp->next = calloc(1, sizeof(struct pathelement));
      tmp = tmp->next;
    }
    tmp->element = p;	
    tmp->next = NULL;
  } while ( p = strtok(NULL, ":") );

  return pathlist;
}

