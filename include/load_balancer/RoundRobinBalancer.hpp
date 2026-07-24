#pragma once

#include "load_balancer/LoadBalancer.hpp"
#include <atomic>

class RoundRobinBalancer : public LoadBalancer {
public:
  Backend selectBackend(const BackendPool &pool) override;

private:
  std::atomic<std::size_t> currentIndex{0};
};