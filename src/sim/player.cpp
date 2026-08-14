#include "sim/player.h"

#include <iostream>

#include "utils/config.h"

template <std::floating_point T>
fx::Player<T>::Player()
    : mutex_{},
      observers_{},
      p_{Point<T>(T(0), T(0), T(0))},
      rng_{},
      configurations_{Config<T>::GetValues()} {
  // Set player's location randomly, upon creation
  auto x_min{configurations_.at("x_min")};
  auto x_max{configurations_.at("x_max")};
  auto y_min{configurations_.at("y_min")};
  auto y_max{configurations_.at("y_max")};
  auto z_min{configurations_.at("z_min")};
  auto z_max{configurations_.at("z_max")};
  rng_.GenerateUniformRandom(p_, Point<T>(x_min, y_min, z_min),
                             Point<T>(x_max, y_max, z_max));

  top_speed_ = T(configurations_.at("top_speed"));
  sample_frequency_ = T(configurations_.at("sample_frequency"));
  d_t_ = T(1) / sample_frequency_;
}

template <std::floating_point T>
void fx::Player<T>::Update() {
  {
    Vector<T> v{};
    Point<T> position;

    {
      lock_guard l(mutex_);
#ifdef DEBUG std::clog << "generated velocity: " << v << std::endl;
      std::clog << "player's location before update: " << p_ << std::endl;
#endif

      // Player can move in 2D with a max speed of top_speed, or can stand
      // still. It is assumed that z dimension represents the head of the player
      // and the player can bend/dodge his/her head with a max speed of 1m/s
      rng_.GenerateUniformRandom(v, Vector<T>(-top_speed_, -top_speed_, -T(1)),
                                 Vector<T>(top_speed_, top_speed_, T(1)));
      p_ += d_t_ * v;
      // Trim x and y values to the range (specified in the config file)
      p_.x = std::clamp(p_.x, T(configurations_.at("x_min")),
                        T(configurations_.at("x_max")));
      p_.y = std::clamp(p_.y, T(configurations_.at("y_min")),
                        T(configurations_.at("y_max")));
      p_.z = std::clamp(p_.z, T(configurations_.at("z_min")),
                        T(configurations_.at("z_max")));
      position = p_;
#ifdef DEBUG
      std::clog << "\t\t  after update:  " << p_ << std::endl;
#endif
    }
    NotifyWith(position);
  }
}

template class fx::Player<float>;
template class fx::Player<double>;
