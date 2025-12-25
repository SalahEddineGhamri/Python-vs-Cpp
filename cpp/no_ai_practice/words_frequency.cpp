/*
 * <No AI>
 * Task: we read a file that contains a text.
 *       we output the freqency of words
 */

#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <unordered_map>

// open a file
std::string read_file(const std::string& path) {
    /*
     * NOTE: another why to read files:
     *
     *  std::ifstream file(path);
     *  if (!file) throw std::runtime_error("cannot open file");
     *  std::ostringstream ss;
     *  ss << file.rdbuf();
     *  return ss.str();
     *
     */

    // openning the file
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("cannot open file");

    file.seekg(0, std::ios::end);
    std::string data;
    data.resize(static_cast<size_t>(file.tellg())); // resize data to fit file using tell get
    file.seekg(0, std::ios::beg);

    file.read(&data[0], data.size());

    return data;
}

void parse(std::unordered_map<std::string, int>& map, const std::string& text){
  std::stringstream ss(text);
  std::string word;
  while (ss >> word) {
    word.erase(std::remove_if(word.begin(), word.end(), [](unsigned char c){ return std::ispunct(c); }), word.end());
    std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c); });
    map[word] += 1;
  }
}

int main() {
  std::unordered_map<std::string, int> map;
  std::string text = read_file("./words_frequency_test_file.txt");

  size_t estimated_words = std::count(text.begin(), text.end(), ' ') + 1;
  map.reserve(estimated_words);

  parse(map, text);

  std::vector<std::pair<std::string, int>> vec(map.begin(), map.end());

  std::sort(vec.begin(), vec.end(),
            [](const auto& a, const auto& b) -> bool {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first; // lexicographical order
            });

  for (auto& element: vec){
    std::cout << element.first << ": " << element.second << std::endl;
  }

  return 0;
}
