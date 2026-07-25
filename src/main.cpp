#include "load_balancer/Backend.hpp"
#include "load_balancer/BackendPool.hpp"
#include "load_balancer/RoundRobinBalancer.hpp"

#include <iostream>

int main() {

  BackendPool pool;

  pool.addBackend(Backend("127.0.0.1", 8081));
  pool.addBackend(Backend("127.0.0.1", 8082));
  pool.addBackend(Backend("127.0.0.1", 8083));

  RoundRobinBalancer balancer;

  for (int i = 0; i < 6; i++) {
    Backend backend = balancer.selectBackend(pool);

    std::cout << backend.getHost() << ":" << backend.getPort() << '\n';
  }
  return 0;
}
