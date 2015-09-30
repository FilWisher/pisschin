/*
 * Implements a doubly-linked list of Pieces (piece-chain)
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

typedef struct {
  int file;
  int pos;
  int len;
  void *prev;
  void *next;
} Piece_Descriptor;

typedef Piece_Descriptor *Piece;

typedef struct {
  int len;
  Piece head;
} Piece_Chain;

Piece_Descriptor *p_alloc(void);
void p_free(Piece_Descriptor *);

Piece_Descriptor *
p_alloc(void) {
  return (Piece_Descriptor *)malloc(sizeof(Piece_Descriptor));
}

void
p_free(Piece_Descriptor *p)
{
  free(p); 
}

Piece_Description
/*
typedef struct {
  int len;
  char *name;
  Piece_Descriptor contents[8];
} Text;
*/

void
print_file(int file, int pos, int len)
{
  char buf[100];
  lseek(file, pos, SEEK_SET);
  read(file, buf, len);
  write(1, buf, len);
}

int
main(int argc, char *argv[])
{
  printf("AOEUAOEU");
}
