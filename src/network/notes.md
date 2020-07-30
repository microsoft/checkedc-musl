## Convert C strings to checked pointers

Handling nt_array_ptr whose length implicitly depends on its content.

Options:
- Add propagate the bounds information from the allocation sites to the use sites.
- Use dynamic casts with bounds widening in an unchecked block.

## Modifying pointers with self-referential bounds
