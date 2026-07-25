#include "load_balancer/Backend.hpp"

Backend::Backend(const std::string &host, int port) : host(host), port(port) {}

const std::string &Backend::getHost() const { return host; }

int Backend::getPort() const { return port; }