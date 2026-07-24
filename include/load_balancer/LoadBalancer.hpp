#pragma once

#include "load_balancer/Backend.hpp"
#include "load_balancer/BackendPool.hpp"

class LoadBalancer {

public:
  virtual ~LoadBalancer() = default;

  virtual Backend selectBackend(const BackendPool &pool) = 0;
};
