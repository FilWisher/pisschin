/*
 * Implements a doubly-linked list of Pieces (piece-chain)
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAXLENGTH 10000

typedef struct pd {
  int file;
  char *name;
  int pos;
  int len;
  struct pd *prev;
  struct pd *next;
} Piece_Descriptor;

typedef Piece_Descriptor *Piece;

typedef struct {
  int len;
  Piece head;
  Piece tail;
} Chain;

Piece_Descriptor *p_alloc(void);
void p_free(Piece);
int file_size(int);
void p_print(Piece);
void print_chain(Chain *);
int file_size(int);

Piece
p_alloc(void) {
  return (Piece)malloc(sizeof(Piece_Descriptor));
}

void
p_free(Piece p)
{
  Piece next_p;
  do {
    next_p = p->next;
    free(p);
    p = next_p;
  } while (p != NULL);
}

void
p_print(Piece p)
{
  char buf[p->len];
  lseek(p->file, p->pos, SEEK_SET);
  read(p->file, buf, p->len);
  write(1, buf, p->len);
}

void
print_chain(Chain *chain)
{
  Piece p = chain->head;
  do {
    p_print(p);
    p = p->next;
  } while(p != NULL);
}

int
file_size(int file)
{
  struct stat buf;
  fstat(file, &buf);
  return buf.st_size;
}

Piece
load_file(char *name, Piece prev, Piece next)
{
  Piece p = p_alloc();
  p->file = open(name, O_RDONLY, 0);
  p->len = file_size(p->file);
  p->prev = prev;
  if (prev != NULL) prev->next = p;
  p->next = next;
  if (next != NULL) next->prev = p;
  return p;
}

int
main(int argc, char *argv[])
{
  Piece first_piece = load_file("as.txt", NULL, NULL);
  Piece second_piece = load_file("bs.txt", first_piece, NULL);
  Piece third_piece = load_file("cs.txt", second_piece, NULL);
  
  Chain chain = {
    .len = 3,
    .head = first_piece,
    .tail = third_piece
  };

  print_chain(&chain);

  p_free(first_piece);
  return 0;
}
