#pragma once
#include <string_view>
#include <toml++/toml.hpp>
#include <unordered_map>

namespace fx {
template <std::floating_point T = float>
class Config {
 private:
  Config() = default;
  ~Config() = default;

  Config(const Config&) = delete;
  Config& operator=(const Config&) = delete;

  static Config& GetConfig() {
    static Config instance;
    [[maybe_unused]] volatile char dummy{'0'};
    return instance;
  }

  std::unordered_map<std::string_view, T> values_;

 public:
  static std::unordered_map<std::string_view, T>& GetValues() {
    Config::GetConfig().ParseToml();
    [[maybe_unused]] volatile char dummy{'0'};
    return Config::GetConfig().values_;
  }

  void ParseToml(std::string_view config_file = "configuration.toml");
};
}  // namespace fx

template class fx::Config<float>;
template class fx::Config<double>;