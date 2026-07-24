#pragma once

#include <string>

class Backend {

public:
  Backend(const std::string &host, int port);

  const std::string &gethost() const;
  int getPort() const;

private:
  std::string host;
  int port;
};