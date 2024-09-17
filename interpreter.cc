#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
class Interpreter {
  std::vector<int> tape;
  int pointer;

public:
  Interpreter(int tape_size = 42) {
    tape.resize(tape_size);
    std::fill(tape.begin(), tape.end(), 0);
    pointer = 0;
  }
  void run(std::string code) {
    for (int code_iter = 0; code_iter < code.size(); code_iter++) {
      char c = code[code_iter];
      switch (c) {
      case '>':
        pointer++;
        break;
      case '<':
        pointer--;
        break;
      case '+':
        tape[pointer]++;
        break;
      case '-':
        tape[pointer]--;
        break;
      case '.':
        std::cout << (char)(tape[pointer]);
        break;
      case ',':
        std::cin >> tape[pointer];
        break;
      case '[':
        if (tape[pointer] == 0) {
          int loop_count = 1;
          while (loop_count != 0) {
            code_iter++;
            if (code[code_iter] == '[') {
              loop_count++;
            } else if (code[code_iter] == ']') {
              loop_count--;
            }
          }
        }
        break;
      case ']':
        if (tape[pointer] != 0) {
          int loop_count = 1;
          while (loop_count != 0) {
            code_iter--;
            if (code[code_iter] == '[') {
              loop_count--;
            } else if (code[code_iter] == ']') {
              loop_count++;
            }
          }
        }
        break;
      }
    }
  }
  std::vector<int> get_tape() { return tape; }
};
int main(int argc, char *argv[]) {
  std::string file_name = argv[1];
  std::ifstream file(file_name);
  std::stringstream buffer;
  buffer << file.rdbuf();
  Interpreter interpreter(5000);
  interpreter.run(buffer.str());
  std::cout << " Tape looks like: " << std::endl;
  for (auto i : interpreter.get_tape()) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
}
