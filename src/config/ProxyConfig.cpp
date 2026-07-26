#include "config/ProxyConfig.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

static std::string trim(const std::string &str) {
  size_t first = str.find_first_not_of(" \t\r\n");
  if (first == std::string::npos)
    return "";
  size_t last = str.find_last_not_of(" \t\r\n");
  return str.substr(first, (last - first + 1));
}

void ProxyConfig::addBackend(const std::string &host, uint16_t port) {
  backends_.push_back({host, port});
}

bool ProxyConfig::loadFromFile(const std::string &filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "[ProxyConfig] Erro ao abrir arquivo de configuracao: "
              << filepath << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    line = trim(line);

    if (line.empty() || line[0] == '#') {
      continue;
    }

    std::istringstream is_line(line);
    std::string key;
    if (std::getline(is_line, key, '=')) {
      std::string value;
      if (std::getline(is_line, value)) {
        key = trim(key);
        value = trim(value);

        if (key == "PORT" || key == "port") {
          port_ = static_cast<uint16_t>(std::stoi(value));

        } else if (key == "BACKEND" || key == "backend") {
          std::istringstream backend_stream(value);
          std::string host, portStr;

          if (std::getline(backend_stream, host, ':') &&
              std::getline(backend_stream, portStr, ':')) {
            addBackend(host, static_cast<uint16_t>(std::stoi(portStr)));
          }
        } else if (key == "RATE_LIMIT_CAPACITY") {
          rateLimitCapacity_ = static_cast<size_t>(std::stoul(value));
        } else if (key == "RATE_LIMIT_REFILL_RATE") {
          rateLimitRefillRate_ = std::stod(value);
        } else if (key == "HEALTH_CHECK_INTERVAL_MS") {
          healthCheckIntervalMs_ = std::stoi(value);
        }
      }
    }
  }

  std::cout << "[ProxyConfig] Configuracao carregada. Porta: " << port_
            << ", Backends cadastrados: " << backends_.size() << std::endl;
  return true;
}