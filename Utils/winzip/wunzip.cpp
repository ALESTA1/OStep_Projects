#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void processCompressedFile(ifstream &file) {
    while (true) {
        
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));

       
        if (file.eof()) break;
        if (file.gcount() != sizeof(count)) {
            cerr << "wunzip: Read error while reading count" << endl;
            exit(1);
        }

        char ch;
        file.read(&ch, sizeof(ch));

        if (file.gcount() != sizeof(ch)) {
            cerr << "wunzip: Read error while reading character" << endl;
            exit(1);
        }

        for (int i = 0; i < count; ++i) {
            putchar(ch);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "wunzip: file1 [file2 ...]" << endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        ifstream file(argv[i], ios::binary);
        if (!file.is_open()) {
            cerr << "wunzip: Cannot open file " << argv[i] << endl;
            return 1;
        }
        processCompressedFile(file);
    }

    return 0;
}
