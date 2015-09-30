# pisschin
A short excursion away from filwisher/wed to better understand piece-chains



A piece is an abstract data type of two parts: Piece and Piece\_Descriptor.

A Piece is a pointer to a Piece\_Descriptor:

```c
typedef struct {
	/* ... */
} Piece_Descriptor;

typedef Piece_Descriptor *Piece;
```


A piece table is a data structure

[0] [Data Structures for Text Sequences - Charles Crowley](https://www.cs.unm.edu/~crowley/papers/sds.pdf)
