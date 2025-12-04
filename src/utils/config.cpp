#include "config.h"

void Config::ParseToml(std::string_view config_file) {
  using namespace std::literals;
  auto config = toml::parse_file(config_file);
  Config::GetConfig().values_["x_min"] = config["dimensions"]["x"][0].value_or(0.f);
  Config::GetConfig().values_["x_max"] = config["dimensions"]["x"][1].value_or(90.f);
  Config::GetConfig().values_["y_min"] = config["dimensions"]["y"][0].value_or(0.f);
  Config::GetConfig().values_["y_max"] = config["dimensions"]["y"][1].value_or(120.f);
  Config::GetConfig().values_["z_min"] = config["dimensions"]["z"][0].value_or(0.1f);
  Config::GetConfig().values_["z_max"] = config["dimensions"]["z"][1].value_or(2.5f);
  Config::GetConfig().values_["top_speed"] = config["dimensions"]["top_speed"].value_or(12.5f);
  Config::GetConfig().values_["sample_frequency"] = config["dimensions"]["sample_frequency"].value_or(20.f);
}