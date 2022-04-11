# binary-csv

This project can be used to compress CSV files into a binary format.
At best, the compressed file will be 25% of the size of the original CSV and
is completely lossless compression. For example, the string "-10," is reduced
to only one byte.

## How to use

Compile with `make`.

`./binary_csv --help` for more information.
