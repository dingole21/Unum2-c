# Unum2-c

A Type-2 Unum computational library.

How to use:

1.  Download the package or git clone from github.
2.  make lib creates a shared library in libUnum2.so in the root directory.

You'll want to define a Unum lattice schema, using the function
```
PEnv *create_pfloat_environment(double *lattice, int latticebits, int epochbits, double pivot)
```

This function is passed an array of lattice points.  These are the positive, > 1
components of the lattice; there should be (2^latticebits - 1) values in this array.
For example, the 4-bit Type-2 Unum with exact points {Inf, -2, -1, -1/2, 0, 1/2, 1, 2}
should have a lattice [2], since that is the only component between 1 and infinity.

epochbits are currently unsupported, but represents how many bits will specify the
epoch in the floating point value.  pivot, is also currently unsupported, but should
represent a value greater than the lattice which is the factor whose powers determine
the range of each epoch.

PFloats
=======

Pfloats in this library are 64-bit integers.  They correspond to the binary forms
described in the Gustafson spec, *left shifted*.  for example, the 4-bit unum
value 2 (0b0110) is represented in this library as 0x6000000000000000.  This
enables the library to take advantage of the innate cyclical nature of Z/(2^64)Z
which is the signed integer type.

PBounds
=======

PBounds in this library are the primary data type.  You can initialize PBounds
using the following functions:

```
void set_empty(PBound *dest);
void set_single(PBound *dest, PFloat f);
void set_bound(PBound *dest, PFloat lower, PFloat upper);
void set_allreals(PBound *dest);
```

which initialize the PBound to emptyset, a single pfloat, an interval of contiguous
pfloats, or all projective real numbers, respectively.

the following mathematical operations are available (NB: these may be subject to
  name changes to avoid namespace collisions):

```
void add(PBound *dest, const PBound *lhs, const PBound *rhs);
void sub(PBound *dest, const PBound *lhs, const PBound *rhs);
void mul(PBound *dest, const PBound *lhs, const PBound *rhs);
void div(PBound *dest, const PBound *lhs, const PBound *rhs);
```

If you need a floating point lattice with exact values that are not easily
representable using IEEE floating points, I recommend using the Julia library:

https://github.com/ityonemo/unum2

This production of this work was supported by the [A*STAR institute](https://www.a-star.edu.sg/)
[Computational Resource Centre](https://www.acrc.a-star.edu.sg/) (A*CRC).
