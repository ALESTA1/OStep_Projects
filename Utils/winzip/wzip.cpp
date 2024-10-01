#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

void processFile(ifstream &file) {
    char prevChar = 0;
    int count = 0;

    while (true) {
        char currentChar;
        file.get(currentChar);

        if (file.eof()) {
            break;
        }

        if (currentChar == prevChar) {
            count++;
        } else {
            if (count > 0) {
                fwrite(&count, sizeof(count), 1, stdout);
                fwrite(&prevChar, sizeof(prevChar), 1, stdout);
            }
            prevChar = currentChar;
            count = 1;
        }
    }
  
    if (count > 0) {
        fwrite(&count, sizeof(count), 1, stdout);
        fwrite(&prevChar, sizeof(prevChar), 1, stdout);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "wzip: file1 [file2 ...]" << endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        ifstream file(argv[i], ios::binary);
        if (!file.is_open()) {
            cerr << "wzip: Cannot open file " << argv[i] << endl;
            return 1;
        }
        processFile(file);
    }

    return 0;
}