/* ideia pratica: fazer revezamento de servers utilizando aritmetica  basica
modular


utilizamos atomic para resolver a concorrencia em caso de duas threads chamarem
ao mesmo tempo a func de selecionar server (poderiamos utilizar tb mutex com
lock_guard que bloquearia em nivel de S.O)

*enquanto a instrução roda (READ->MODIFY->WRITE) nenhuma out thread consegue
olhar ou mexer na memoria*
*/

#include "load_balancer/RoundRobinBalancer.hpp"

Backend RoundRobinBalancer::selectBackend(const BackendPool &pool) {
  const auto &backends = pool.getBackends();

  std::size_t index = currentIndex.fetch_add(1) % backends.size();

  return backends[index];
}