#include "network/Socket.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket(int fd) : fd_(fd) {
  if (fd_ == -1) {
    fd_ = ::socket(AF_INET, SOCK_STREAM,
                   0); // AF_INET = ipv4 SOCK_STREAM = conexao orientada a fluxo
                       // de bytes(tcp)
  }
}

Socket::~Socket() { close(); }

void Socket::close() {
  if (isValid()) {
    ::close(fd_);
    fd_ = -1;
  }
}

// construtor de movimentação
Socket::Socket(Socket &&other) noexcept : fd_(other.fd_) {
  other.fd_ = -1; // rouba o fd e deixa o outro em estado invalido
}

Socket &Socket::operator=(Socket &&other) noexcept {
  if (this != &other) {
    close(); // fechamos o socket atual se ja tinhamos um aberto
    fd_ = other.fd_;
    other.fd_ = -1;
  }

  return *this;
}

bool Socket::bind(uint16_t port) {
  if (!isValid())
    return false;

  int opt = 1;
  ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  return ::bind(fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == 0;
}

bool Socket::listen(int backlog) const {
  if (!isValid())
    return false;
  return ::listen(fd_, backlog) == 0;
}

Socket Socket::accept(std::string &clientIp, uint16_t &clientPort) const {
  if (!isValid())
    return Socket(-1);

  sockaddr_in clientAddr{};

  socklen_t clientLen = sizeof(clientAddr);

  // aguarda cliente se conectar
  int clientFd =
      ::accept(fd_, reinterpret_cast<sockaddr *>(&clientAddr), &clientLen);
  if (clientFd == -1) {
    return Socket(-1);
  }

  // converção de ip binario em texto
  char ipStr[INET_ADDRSTRLEN];
  ::inet_ntop(AF_INET, &(clientAddr.sin_addr), ipStr, INET_ADDRSTRLEN);

  clientIp = ipStr;

  clientPort = ntohs(clientAddr.sin_port);

  return Socket(clientFd);
}

ssize_t Socket::send(const std::string &data) const {
  if (!isValid())
    return -1;

  return ::send(fd_, data.data(), data.size(), 0);
}

ssize_t Socket::receive(char *buffer, std::size_t size) const {
  if (!isValid())
    return -1;

  return ::recv(fd_, buffer, size, 0);
}