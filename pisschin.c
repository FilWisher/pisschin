/*
 * Implements a doubly-linked list of Pieces (piece-chain)
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAXLENGTH 10000

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
  Piece tail;
} Chain;

Piece_Descriptor *p_alloc(void);
void p_free(Piece);

Piece_Descriptor *
p_alloc(void) {
  return (Piece_Descriptor *)malloc(sizeof(Piece_Descriptor));
}

void
p_free(Piece p)
{
  free(p); 
}

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
  Piece first_piece = p_alloc();
  
  first_piece->file = open("as.txt", O_RDONLY, 0);
  first_piece->pos = 0;
  first_piece->len = MAXLENGTH;
  first_piece->prev = NULL;
  first_piece->next = NULL;
  
  Chain chain = {
    .len = 1,
    .head = first_piece,
    .tail = first_piece
  };

  p_free(first_piece);
  
  return 0;
}
