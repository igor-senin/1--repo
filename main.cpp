#include <iostream>

#include <stdlib.h>

#include "engine.cpp"

int main(int argc, char** argv) {
  /*
   * Постановка:
   * на вход программе (в качестве аргументов командной строки)
   * подаются две строки - названия директорий,
   * а также вещественное число - процент (в формате xx.xxxxx).
   *
   * Нужно для каждой пары файлов (A, B) из этих директорий
   * сказать, идентичны они, не идентичны,
   * или похожи на k%, k >= l, l - входной процент.
   */

  if (argc != 4) {
    std::cout << "Format:\n./executable <first_directory> <second_directory> <percent_of_similarity>\n\n";

    std::cerr << "You've entered less or more than 4 arguments\nTerminating...\n";
    return 1;
  }

  std::string firstDirectory{argv[1]};
  std::string secondDirectory{argv[2]};
  float simPercent = atof(argv[3]);

  return RunMain(firstDirectory, secondDirectory, simPercent);
}
