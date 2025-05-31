#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace inspekku {

struct MemoryInfo {
  std::time_t time{};
  std::unordered_map<std::string, uint64_t> values;
};

class MemoryInfoProvider {
 public:
  MemoryInfo GetMemoryInfo() {
    std::ifstream ifs{"/proc/meminfo"};

    MemoryInfo memoryInfo{.time = std::time(nullptr)};

    std::string line;
    while (std::getline(ifs, line)) {
      std::string key;
      uint64_t value = 0;
      ReadMemoryInfoLine(line, key, value);

      memoryInfo.values[std::move(key)] = value;
    }

    return memoryInfo;
  }

 private:
  void ReadMemoryInfoLine(const std::string& line,
                          std::string& key,
                          uint64_t& value) {
    std::istringstream is{line};
    std::getline(is, key, ':');
    is >> value;
  }
};

}  // namespace inspekku
