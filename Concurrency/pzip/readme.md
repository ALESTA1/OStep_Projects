# Parallel Zip (pzip)

`pzip` is a multithreaded, parallel zip utility that compresses files using Run-Length Encoding (RLE). It processes multiple files simultaneously, achieving performance gains on multi-core systems.

## Usage

To compress multiple files using `pzip`, use the following command:

```bash
./pzip file1 file2 file3 ... > out.z
```
##Following are some test results against wzip which is single threaded
##For 100 files
![pzip_wzip_ratio_100_files](https://github.com/user-attachments/assets/7d005d15-4803-44b4-a36b-d0ef0dac46a4)
## For 1000 files
![pzip_wzip_ratio_1000_files](https://github.com/user-attachments/assets/43176c89-a80e-4b85-940c-3142db77da3c)
## For 5000 files
![pzip_wzip_ratio_5000_files](https://github.com/user-attachments/assets/cf48d66a-6dd5-49a3-a667-b7c7b00b0b0f)

