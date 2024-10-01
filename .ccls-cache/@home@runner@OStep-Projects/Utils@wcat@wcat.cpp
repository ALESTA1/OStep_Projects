#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

void fileHandler(std::string fileName) {

  FILE *fp = fopen(fileName.c_str(), "r");
  if (fp == NULL) {
    printf("wcat: cannot open file\n");
    exit(1);
  }
  char buffer[4096];
  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    std::cout << buffer;
  }

  fclose(fp);
}
int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "No file name provided" << std::endl;
    exit(0);
  }

  std::vector<std::thread> threads;
  for (int i = 1; i < argc; i++) {

    std::string fileName = argv[i];
    std::thread temp(fileHandler, move(fileName));
    threads.push_back(move(temp));
  }

  for (auto &t : threads)
    t.join();

  return 0;
}