#include <boost/asio.hpp>
#include <chrono>

namespace inspekku::utils {

class PeriodicTask {
 public:
  PeriodicTask(const int interval,
               std::function<void()> callback,
               boost::asio::io_context& ioContext)
      : callback_{std::move(callback)},
        timer_{ioContext},
        interval_{std::chrono::seconds{interval}} {
    Schedule();
  }

 private:
  void Schedule() {
    timer_.expires_after(std::chrono::seconds(interval_));
    timer_.async_wait([this](const boost::system::error_code& ec) {
      if (!ec) {
        callback_();
        Schedule();
      }
    });
  }

 private:
  std::function<void()> callback_;
  boost::asio::steady_timer timer_;
  const std::chrono::seconds interval_;
};

}  // namespace inspekku::utils
