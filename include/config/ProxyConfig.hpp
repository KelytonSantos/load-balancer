#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct BackendConfig {
  std::string host;
  uint16_t port;
};

class ProxyConfig {
public:
  ProxyConfig() = default;
  ~ProxyConfig() = default;

  bool loadFromFile(const std::string &filepath);
  void addBackend(const std::string &host, uint16_t port);

  uint16_t getPort() const { return port_; }
  const std::vector<BackendConfig> &getBackends() const { return backends_; }

  int getHealthCheckIntervalMs() const { return healthCheckIntervalMs_; }
  int getHealthCheckTimeoutMs() const { return healthCheckTimeoutMs_; }

  size_t getRateLimitCapacity() const { return rateLimitCapacity_; }
  double getRateLimitRefillRate() const { return rateLimitRefillRate_; }

  void setPort(uint16_t port) { port_ = port; }
  void setHealthCheckIntervalMs(int ms) { healthCheckIntervalMs_ = ms; }
  void setRateLimitCapacity(size_t capacity) { rateLimitCapacity_ = capacity; }
  void setRateLimitRefillRate(double rate) { rateLimitRefillRate_ = rate; }

private:
  uint16_t port_ = 8080;
  std::vector<BackendConfig> backends_;
  int healthCheckIntervalMs_ = 5000;
  int healthCheckTimeoutMs_ = 2000;

  size_t rateLimitCapacity_ = 10;
  double rateLimitRefillRate_ = 2.0;
};