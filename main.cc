#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>
#include<fstream>
#include<iostream>

int main(int argc, char ** argv) {
  if(argc <= 2) {
    std::cout <<
      "usage:" << std::endl <<
      "argv[1]: set --lf or --crlf" << std::endl <<
      "argv[2]: file path" << std::endl <<
      "argv[3](optional): --force" << std::endl <<
      std::endl <<
      "--lf: crlf to lf" << std::endl <<
      "--crlf: lf to crlf" << std::endl <<
      "--force: replace original file with converted file" << std::endl;
    return EXIT_SUCCESS;
  }

  enum Mode {
    INVALID,
    LF,
    CRLF
  };
  Mode mode = INVALID;
  char prev = 0;
  std::string tmp = argv[2] + std::string(".tmp");
  std::ifstream ifs;
  std::ofstream ofs;

  ifs.open(argv[2], std::ios::binary);
  ofs.open(tmp, std::ios::binary);
  if( ! ifs || ! ofs) {
    std::cerr << "file cannot opened." << std::endl;
    goto fail;
  }

  if(strcmp(argv[1], "--lf") == 0) mode = LF;
  if(strcmp(argv[1], "--crlf") == 0) mode = CRLF;
  if(mode == INVALID) {
    std::cerr << "invalid option has set." << std::endl;
    goto fail;
  }

  while(true) {
    char c = 0;

    if(ifs.read(&c, 1).eof()) break;

    if (mode == CRLF) {
      if(prev != '\r' && c == '\n') {
        if(ofs.write("\r", 1).fail()) {
          std::cerr << "file cannot write." << std::endl;
          goto fail;
        }
      }
    }

    if (mode == LF) {
      if(c == '\r') continue;
    }

    if(ofs.write(&c, 1).fail()) {
      std::cerr << "file cannot write." << std::endl;
      goto fail;
    }

    prev = c;
  }

  if(argc == 4 && strcmp(argv[3], "--force") == 0) {
    ifs.close();
    ofs.close();

    if(remove(argv[2]) != 0) {
      std::cerr << "file cannot delete." << std::endl;
      goto fail;
    }

    if(rename(tmp.c_str(), argv[2]) != 0) {
      std::cerr << "file cannot rename." << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;

fail:
  ofs.close();
  remove(tmp.c_str());
  return EXIT_FAILURE;
}
