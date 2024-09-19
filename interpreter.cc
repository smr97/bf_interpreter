#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
class Profiler {
  int inc_tape = 0;
  int dec_tape = 0;
  int inc_pointer = 0;
  int dec_pointer = 0;
  int print = 0;
  int read = 0;
  int simple_loops = 0;
  int nonsimp_loops = 0;

public:
  void register_instruction(char c) {
    switch (c) {
    case '>':
      inc_pointer++;
      break;
    case '<':
      dec_pointer++;
      break;
    case '+':
      inc_tape++;
      break;
    case '-':
      dec_tape++;
      break;
    case '.':
      print++;
      break;
    case ',':
      read++;
      break;
    case '[':
    case ']':
      break;
    }
  }
  void register_loop(std::string loop_start) {
    std::string simple_loop = loop_start.substr(0, 3);
    if (simple_loop == "[-]" || simple_loop == "[+]") {
      simple_loops++;
    } else {
      nonsimp_loops++;
    }
  }
  std::string to_string(){
    std::stringstream result;
    result << "Instructions: " << std::endl;
    result << "  Increment tape: " << inc_tape << std::endl;
    result << "  Decrement tape: " << dec_tape << std::endl;
    result << "  Increment pointer: " << inc_pointer << std::endl;
    result << "  Decrement pointer: " << dec_pointer << std::endl;
    result << "  Print: " << print << std::endl;
    result << "  Read: " << read << std::endl;
    result << "Loops: " << std::endl;
    result << "  Simple loops: " << simple_loops << std::endl;
    result << "  Non-simple loops: " << nonsimp_loops << std::endl;
    return result.str();
  }
};
class Interpreter {
  std::vector<int> tape;
  int pointer;
  Profiler profiler;

public:
  Interpreter(int tape_size = 42) {
    tape.resize(tape_size);
    std::fill(tape.begin(), tape.end(), 0);
    pointer = 0;
  }
  void run(std::string code, bool do_profile) {
    for (int code_iter = 0; code_iter < code.size(); code_iter++) {
      char c = code[code_iter];
      if (do_profile)
        profiler.register_instruction(c);
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
          profiler.register_loop(code.substr(code_iter, 3));
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
          profiler.register_loop(code.substr(code_iter, 3));
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
  std::string dump_profiler() { return profiler.to_string(); }
};
int main(int argc, char *argv[]) {
  // If the user has a -p flag, then we will profile the code
  // Otherwise, we will just run the code
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename> [-p]" << std::endl;
    return 1;
  }
  bool do_profile = false;
  if (argc == 3) {
    if (std::string(argv[2]) == "-p") {
      do_profile = true;
    }
  }
  std::string file_name = argv[1];
  std::ifstream file(file_name);
  std::stringstream buffer;
  buffer << file.rdbuf();
  Interpreter interpreter(5000);
  interpreter.run(buffer.str(), do_profile);
  std::cout << " Tape looks like: " << std::endl;
  for (auto i : interpreter.get_tape()) {
    std::cout << i << " ";
  }
  if (do_profile)
    std::cout << interpreter.dump_profiler();
  std::cout << std::endl;
}
