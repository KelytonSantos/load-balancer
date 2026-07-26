#include "network/TcpConnection.hpp"

#include <cstdint>
#include <sys/types.h>
#include <unistd.h>

TcpConnection::TcpConnection(Socket socket, std::string clientIp,
                             uint16_t clientPort)
    : socket_(std::move(socket)), clientIp_(std::move(clientIp)),
      clientPort_(clientPort) {}

void TcpConnection::close() { socket_.close(); }

ssize_t TcpConnection::readToBuffer() {
  if (!isConnected())
    return -1;

  char tempBuffer[4096];

  ssize_t bytesRead = socket_.receive(tempBuffer, sizeof(tempBuffer));

  if (bytesRead > 0) {
    inBuffer_.append(tempBuffer, static_cast<std::size_t>(bytesRead));
  }

  return bytesRead;
}

ssize_t TcpConnection::send(const std::string &data) {
  if (!isConnected())
    return -1;

  return socket_.send(data);
}

// envia os dados ja lidos
ssize_t TcpConnection::flushOutputBuffer() {
  if (!isConnected() || outBuffer_.empty())
    return 0;

  ssize_t bytesSent = socket_.send(outBuffer_);

  if (bytesSent > 0)
    outBuffer_.erase(0, static_cast<std::size_t>(bytesSent));

  return bytesSent;
}

// limpa os dados que ja foram lidos
void TcpConnection::consumeInBuffer(std::size_t bytes) {
  if (bytes >= inBuffer_.size()) {
    inBuffer_.clear();
  } else {
    inBuffer_.erase(0, bytes);
  }
}
