#include <boost/asio.hpp>
#include <iostream>

#include "memory.h"
#include "periodic_task.h"

using namespace std::chrono_literals;

void PrintMemoryInfo() {
  inspekku::MemoryInfoProvider memoryInfoProvider;
  auto memoryInfo = memoryInfoProvider.GetMemoryInfo();
  std::cout << "Time=" << memoryInfo.time << std::endl;
  for (const auto& key : {"MemTotal", "MemFree", "MemAvailable"}) {
    std::cout << key << '=' << memoryInfo.values[key] << std::endl;
  }

  std::cout << std::endl;
}

int main(int argc, char* argv[]) {
  boost::asio::io_context ioContext;
  boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
  signals.async_wait([&](const boost::system::error_code&, const int signal) {
    std::cerr << "Graceful shutdown" << std::endl;
    ioContext.stop();
  });

  inspekku::utils::PeriodicTask memoryTask{5, []() { PrintMemoryInfo(); },
                                           ioContext};

  ioContext.run();
  return 0;
}
