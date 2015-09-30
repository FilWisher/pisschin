# pisschin
A short excursion away from [wed](https://github.com/FilWisher/pisschin/tree/master) to better understand piece-chains

More detailed sources at the end of this document.

## huh?
Piece-chains are a structure for managing text. 

Imagine loading a big piece of text into a buffer. If you want to make edits to that text, all the changes you make are directly on the buffer. This may mean splitting or copying parts of the buffer whenever you make edits. It also means that time to load or edit a file is function of the file size.

If we create an abstraction layer over the piece of text, all of our edits are on the abstraction layer. 

Piece-chains are an abstraction layer that view a text as a chain of pieces. Each piece is a description of source of data, a starting point in that data, and a length. A piece chain is a chain of these pieces which together make up a text. Now editing the text is simply editing these descriptions: for example to delete text we simple decrement the value of the length. The time to edit or load a file is now a function of the number of pieces, regardless of whether the file is 1 line or 1000000000 lines.

## data types
A piece is an abstract data type of two parts: Piece and Piece\_Descriptor.

A Piece is a pointer to a Piece\_Descriptor:

```c
typedef struct {
	/* ... */
} Piece_Descriptor;

typedef Piece_Descriptor *Piece;
```

A Piece_Descriptor is a struct with a file descriptor, a position, a length, pointer to the next Piece_Descriptor, and a pointer to the previous Piece_Descriptor:

```c
typedef struct piece_descriptor {
	int file; /* file descriptor */
	int pos;
	int len;
	piece_descriptor *next;
	piece_descriptor prev;
} Piece_Descriptor;
```

The collection of Piece\_Descriptors forms a doubly-linked list. This means that the Piece\_Descriptors needn't be contiguous in memory. 


A piece chain is a created by pointing the .next value of piece descriptor A at another piece descriptor B. Piece descriptor B should also have its .prev value set to a pointer to piece descriptor B and, if it is the final piece descriptor, it's .next value should be set to NULL.

```c
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

/* ... */

	Piece first_piece = load_file("as.txt", NULL, NULL);
	Piece second_piece = load_file("bs.txt", first_piece, NULL);
```

## now what?
Once we have a piece chain constructed, there a few simple things we want to do with it:
- Map a position in the text to a position in the piece chain:
```c
int text_pos_to_pc_pos(Piece p, int text_pos);
```
- Insert new pieces anywhere in the chain
- Delete pieces anywhere in the chain
- Write piece chain (text) to file
- Render piece chain (text) on screen
- Send piece chain (text) to another process

These simple things will require slightly more code than if we were working with a buffer but it will pay off in performance. 

## sources
[0] [Data Structures for Text Sequences - Charles Crowley](https://www.cs.unm.edu/~crowley/papers/sds.pdf)
