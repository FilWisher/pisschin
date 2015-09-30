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

typedef struct {
  Piece piece;
  int offset;
} Location;

/* text is our access to the text */
typedef struct {
  int len;
  Piece head; /* head is the first item in the text */
  Piece tail; /* tail is the last item in teh text */
} Text;

/* p_* functions for pieces */
Piece p_alloc(void);
Piece p_load_file(char *, Piece, Piece);
void p_print(Piece);
Location p_location_of(Text, int);
void p_free(Piece);
void p_set_prev(Piece, Piece);
void p_set_next(Piece, Piece);

/* t_* functions for text */
Text t_load_file(char *);
void t_print(Text);
void t_cleanup(Text);

/* e_* functions for editing */
void e_delete(Location);

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
  } while (p != NULL); }

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

void
p_set_prev(Piece piece, Piece prev)
{
  if (piece == NULL) return;
  else piece->prev = prev;
}

void
p_set_next(Piece piece, Piece next)
{
  if(piece == NULL) return;
  else piece->next = next;
}

/* finds piece that contains pos and returns its offset within piece */
Location 
p_location_of(Text t, int pos)
{
  Location loc;
  loc.piece = t.head;
  loc.offset = pos;
  while (loc.piece != NULL && pos > loc.piece->len) {
    pos -= loc.piece->len;
    loc.piece = loc.piece->next;
  }
  if (loc.piece == NULL) loc.offset = -1; 
  else loc.offset = pos;
  return loc;
}

Piece
p_make(int file, int pos, int len, Piece prev, Piece next)
{
  Piece p = p_alloc();
  p->file = file;
  p->pos = pos;
  p->len = len;
  p->prev = prev;
  p->next = next;
  return p;
}

/*  p->prev    p     p->next
 *   [ ] ---- [ ] ---- [ ]
 *            ...
 *  p->prev           p->next
 *   [ ]  before|after  [ ]
 *       \ [ ] -- [ ] /
 */
void
e_delete(Location loc) {

  Piece p = loc.piece;
  Piece before_del = p_make(p->file, p->pos, (loc.offset -1), p->prev, NULL);
  
  /* reroute prev to first-1/2 of split */
  p_set_next(p->prev, before_del); 
  
  Piece after_del = p_make(p->file, (loc.offset), (p->len - loc.offset), 
    before_del, p->next);
  
  /* reroute first-1/2 to second-1/2 */
  p_set_next(before_del, after_del); 
  /* reroute prev of p->next to second-1/2 */
  p_set_prev(p->next, after_del); 
  
  p_free(p);
}

int
main(int argc, char *argv[])
{
  char *name = "as.txt";
  Text text = t_load_file(name);
  Piece p = text.head;
  Piece p_2 = p_load_file("bs.txt", p, NULL);
  int in_i = file_size(p->file);; /* 4 into second */

  t_print(text);
  
  Location location = p_location_of(text, in_i);
  Location d_loc = {
    .piece = p_2,
    .offset = 9 
  };
  e_delete(d_loc); 
  t_print(text);

  t_cleanup(text);
  return 0;
}
