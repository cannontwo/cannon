#include <cannon/utils/statistics.hpp>

using namespace cannon::utils;

// Initialize static variable
std::vector<std::function<void(StatsAccumulator &)>> *StatRegisterer::funcs_;

StatRegisterer::StatRegisterer(std::function<void(StatsAccumulator &)> func) {
  if (!funcs_)
    funcs_ = new std::vector<std::function<void(StatsAccumulator &)>>;

  funcs_->push_back(func);
}

void StatRegisterer::CallCallbacks(StatsAccumulator &accum) {
  for (auto func : *funcs_)
    func(accum);
}

void cannon::utils::get_category_and_title(const std::string& str,
    std::string *category, std::string *title) {
  const char *s = str.c_str();
  const char *slash = std::strchr(s, '/');

  if (!slash)
    *title = str;
  else {
    *category = std::string(s, slash - s);
    *title = std::string(slash + 1);
  }
}

void cannon::utils::report_thread_stats() {
  static std::mutex mutex;
  std::lock_guard<std::mutex> lock(mutex);
  StatRegisterer::CallCallbacks(stats_accumulator);
}

void cannon::utils::print_stats(const std::string& filename) {
  stats_accumulator.print(filename);
}
