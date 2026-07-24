// o backend pool representa os servidores disponiveis
#pragma once

#include "load_balancer/Backend.hpp"
#include <vector>

class BackendPool {
public:
  void addBackend(const Backend &backend);
  const std::vector<Backend> &getBackends() const;

private:
  std::vector<Backend> backends;
};