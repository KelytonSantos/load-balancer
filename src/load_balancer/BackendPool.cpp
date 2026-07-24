#include "load_balancer/BackendPool.hpp"

void BackendPool::addBackend(const Backend &backend) {
  backends.push_back(backend);
}

const std::vector<Backend> &BackendPool::getBackends() const {
  return backends;
}