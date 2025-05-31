#include <boost/asio.hpp>
#include <iostream>

#include "memory.h"
#include "periodic_task.h"

using namespace std::chrono_literals;

class StorageEngine {
 public:
  StorageEngine(const char* const fileName)
      : storageFile_{fileName, std::ios::out | std::ios::app} {}

  void Write(std::time_t timestamp,
             const std::string& statName,
             const uint64_t value) {
    storageFile_ << timestamp << ' ' << statName << ' ' << value << std::endl;
  }

 private:
  std::ofstream storageFile_;
};

void SaveMemoryInfo(StorageEngine& storage) {
  inspekku::MemoryInfoProvider memoryInfoProvider;
  inspekku::MemoryInfo memoryInfo = memoryInfoProvider.GetMemoryInfo();

  const int memoryTotal = memoryInfo.values["MemTotal"];
  const int memoryUsed = memoryTotal - memoryInfo.values["MemAvailable"];
  storage.Write(memoryInfo.time, "system:memory_used", memoryUsed);
  storage.Write(memoryInfo.time, "system:memory_total", memoryTotal);
}

int main(int argc, char* argv[]) {
  boost::asio::io_context ioContext;
  boost::asio::signal_set signals{ioContext, SIGINT, SIGTERM};
  signals.async_wait([&](const boost::system::error_code&, const int signal) {
    std::cerr << "Graceful shutdown" << std::endl;
    ioContext.stop();
  });

  StorageEngine storageEngine{"/workspaces/inspekku/build/collector/memory.db"};

  const int intervalSec = 5;
  inspekku::utils::PeriodicTask memoryTask{
      intervalSec, [&]() { SaveMemoryInfo(storageEngine); }, ioContext};

  ioContext.run();
  return 0;
}
