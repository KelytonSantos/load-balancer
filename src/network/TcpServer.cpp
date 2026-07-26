#include "network/TcpServer.hpp"
#include "network/TcpConnection.hpp"

#include <iostream>
#include <memory>

TcpServer::TcpServer(uint16_t port) : port_(port) {}

TcpServer::~TcpServer() { stop(); }

bool TcpServer::start() {
  if (!listenSocket_.bind(port_)) {

    std::cerr << "[TcpServer]: Erro ao fazer bind na porta " << port_
              << std::endl;
    return false;
  }

  if (!listenSocket_.listen(128)) {
    std::cerr << "[TcpServer]: Erro ao colocar o socket em escuta" << std::endl;
    return false;
  }

  isRunning_ = true;
  std::cout << "[TcpServer]: Servidor rodando na porta " << port_ << std::endl;

  return true;
}

void TcpServer::stop() {
  if (isRunning_) {
    isRunning_ = false;
    listenSocket_.close();
  }
}

//------------------

void TcpServer::run() {
  while (isRunning_) {
    std::string clientIp;
    uint16_t clientPort = 0;

    Socket clientSocket = listenSocket_.accept(
        clientIp, clientPort); // guarda e aceita o proximo cliente na fila

    if (!clientSocket.isValid()) { // se o socket for invalido
      if (!isRunning_)
        break; // encerra o loop se stop() for chamado(stop altera a flag|line
               // 29)
      continue;
    }

    // encapsula o socket em uma nova tcpconnection com ponteiro shared
    auto conn = std::make_shared<TcpConnection>(
        std::move(clientSocket), std::move(clientIp), clientPort);

    // dispara o callback informando o reverseProxy que um client chegou
    if (onNewConnection_) {
      onNewConnection_(conn);
    }
  }
}