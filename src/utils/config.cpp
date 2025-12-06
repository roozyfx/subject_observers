#include "config.h"

template <std::floating_point T>
void fx::Config<T>::ParseToml(std::string_view config_file) {
  using namespace std::literals;
  auto config = toml::parse_file(config_file);
  Config::GetConfig().values_["x_min"] = config["dimensions"]["x"][0].value_or(T(0));
  Config::GetConfig().values_["x_max"] = config["dimensions"]["x"][1].value_or(T(90));
  Config::GetConfig().values_["y_min"] = config["dimensions"]["y"][0].value_or(T(0));
  Config::GetConfig().values_["y_max"] = config["dimensions"]["y"][1].value_or(T(120));
  Config::GetConfig().values_["z_min"] = config["dimensions"]["z"][0].value_or(T(0.1));
  Config::GetConfig().values_["z_max"] = config["dimensions"]["z"][1].value_or(T(2.5));
  Config::GetConfig().values_["top_speed"] = config["dimensions"]["top_speed"].value_or(T(12.5));
  Config::GetConfig().values_["sample_frequency"] = config["dimensions"]["sample_frequency"].value_or(T(20));
  Config::GetConfig().values_["measurement_noise"] = config["dimensions"]["measurement_noise"].value_or(T(0.3));
  Config::GetConfig().values_["number_sensors"] = config["dimensions"]["number_sensors"].value_or(size_t(30));
}

template class fx::Config<float>;
template class fx::Config<double>;