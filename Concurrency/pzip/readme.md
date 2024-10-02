# Parallel Zip (pzip)

`pzip` is a multithreaded, parallel zip utility that compresses files using Run-Length Encoding (RLE). It processes multiple files simultaneously, achieving performance gains on multi-core systems.

## Usage

To compress multiple files using `pzip`, use the following command:

```bash
./pzip file1 file2 file3 ... > out.z
