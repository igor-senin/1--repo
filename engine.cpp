#include <cmath>
#include <string>
#include <vector>

#include "files.cpp"

/* Used in comparisons with floats */
constexpr float eps = 1e-5;


size_t LongestCommonSubsequenceLength(const std::string& first,
                                      const std::string& second,
                                      float percent);

int RunMain(const std::string& firstDir, const std::string& secondDir, float percent) {
  std::vector<File> first{GetDirectoryFiles(firstDir)};
  std::vector<File> second{GetDirectoryFiles(secondDir)};

  /* For each file of second dir contains first index
   * of file in first dir that equals to current file
   * (from 1 to first.size(); 0 means no equal files) */
  std::vector<size_t> eqIndexes(second.size(), 0);

  std::vector<bool> areInFirst(second.size(), false);
  std::vector<bool> areInSecond(first.size(), false);

  std::vector<std::vector<size_t>> identicalTo{first.size()};
  std::vector<std::vector<std::pair<size_t, float>>> similarTo{first.size()};
  std::vector<std::vector<size_t>> different{first.size()};

  for (size_t i = 0; i < first.size(); ++i) {
    for (size_t j = 0; j < second.size(); ++j) {
      size_t first_size = first[i].GetContent().size();
      size_t second_size = second[j].GetContent().size();

      if (first_size != second_size
          || first[i].GetHash() != second[j].GetHash()
          || first[i].GetContent() != second[j].GetContent()) {
        // Case files' contents aren't equal
        float common_len = static_cast<float>(LongestCommonSubsequenceLength(
            first[i].GetContent(), second[j].GetContent(), percent));

        std::cout << i << ' ' << j << '\n';
        std::cout << common_len << "\n";
        float similarity = common_len / static_cast<float>(std::max(first_size, second_size));

        if (similarity + eps < percent) {
          different[i].push_back(j);
        } else {
          similarTo[i].push_back(std::make_pair(j, similarity));
        }

        continue;
      }

      // Case equal
      if (eqIndexes[j] == 0) {
        eqIndexes[j] = i + 1;
        areInFirst[j] = areInSecond[i] = true;
        identicalTo[i].push_back(j);
      } else {
        size_t brother = eqIndexes[j] - 1;

        different[i] = different[brother];
        identicalTo[i] = identicalTo[brother];
        similarTo[i] = similarTo[brother];

        break;
      }
    }
  }

  // Printing results
  std::cout << "These files are identical:\n";
  for (size_t i = 0; i < first.size(); ++i) {
    for (auto j : identicalTo[i]) {
      std::cout << firstDir << '/' << first[i].GetPath()
          << " - " << secondDir << '/' << second[j].GetPath() << "\n";
    }
  }

  std::cout << "\nThese files are similar:\n";
  for (size_t i = 0; i < first.size(); ++i) {
    for (auto [j, similarity] : similarTo[i]) {
      std::cout << firstDir << '/' << first[i].GetPath()
          << " - " << secondDir << '/' << second[j].GetPath()
          << " - " << similarity << "\n";
    }
  }

  std::cout << "\nThese files are only in first directory:\n";
  for (size_t i = 0; i < first.size(); ++i) {
    if (!areInSecond[i]) {
      std::cout << firstDir << '/' << first[i].GetPath() << "\n";
    }
  }

  std::cout << "\nThese files are only in second directory:\n";
  for (size_t j = 0; j < second.size(); ++j) {
    if (!areInFirst[j]) {
      std::cout << secondDir << '/' << second[j].GetPath() << "\n";
    }
  }

  return 0;
}

size_t LongestCommonSubsequenceLength(const std::string& first,
                                      const std::string& second,
                                      float percent = 0.f) {
  if (first.size() == 0 || second.size() == 0) {
    return 0;
  }

  std::vector<std::vector<size_t>> lcsCalculator(
      first.size() + 1,
      std::vector<size_t>(second.size() + 1, 0));

  float biggest = static_cast<float>(std::max(first.size(), second.size()));

  for (size_t i = 1; i <= first.size(); ++i) {
    for (size_t j = 1; j <= second.size(); ++j) {
      if (first[i - 1] == second[j - 1]) {
        lcsCalculator[i][j] = lcsCalculator[i - 1][j - 1] + 1;
      } else {
        lcsCalculator[i][j] = std::max(
            lcsCalculator[i - 1][j],
            lcsCalculator[i][j - 1]);
      }

//      /* Max possible len could be reached */
//      float max_len = static_cast<float>(
//          lcsCalculator[i][j] + std::min(first.size() - i, second.size() - j));
//      if (max_len + eps <  * percent) {
//        /* Further calculations are meaningless; exiting with 0 */
//        return 0;
//      }
    }
  }

  return lcsCalculator[first.size()][second.size()];
}
