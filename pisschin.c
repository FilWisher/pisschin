/*
 * Implements a doubly-linked list of Pieces (piece-text)
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

/* text is our access to the text */
typedef struct {
  int len;
  Piece head; /* head is the first item in the text */
  Piece tail; /* tail is the last item in teh text */
} Text;

Piece_Descriptor *p_alloc(void);
Piece p_load_file(char *, Piece, Piece);
void p_print(Piece);
void p_free(Piece);

Text t_load_file(char *);
void t_print(Text);
void t_cleanup(Text);

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
t_print(Text text)
{
  Piece p = text.head;
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
p_load_file(char *name, Piece prev, Piece next)
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

Text
t_load_file(char *name)
{
  Text t;
  Piece p = p_load_file(name, NULL, NULL);
  t.head = t.tail = p;
  t.len = 3;
  return t; 
}

void
t_cleanup(Text t)
{
  p_free(t.head);
}

int
main(int argc, char *argv[])
{
  Text text = t_load_file(argv[1]);

  t_print(text);

  t_cleanup(text);
  return 0;
}
