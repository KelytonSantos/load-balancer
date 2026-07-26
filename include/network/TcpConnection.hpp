/*
                gerencia o estado da conexao (sabe se o cliente ainda esta
   conectado, quanto tempo a conexao esta ociosa ou se houve erro) abstração de
   metodos para a aplicação e controle de leitura nao bloqueante armazenara
   metadados da sessao


                controle de leitura bloqueante: quando se chama recv() ou
   receive o SO suspende a thread até existir dados no buffer da placa de rede
                a questão é: se tivermos 1000 conexões abertas e utilizar uma
   thread para cada aguardando no recv() bloqueante consumiremos muito recurso
   (memoria de stack e context switch entre thread)

                controle de leitura não-bloqueante:se marcamos como nao
   bloqueante(func fcntl), apartir do momento que chamamos uma recv ela
   retornara mesmo se nao tiver nada no buffer



                                                                         INTERNET
                                                                                │
                                                                                │
                                                                                ▼
                                                        ┌───────────┐
                                                        │   Socket  │
                                                        └─────┬─────┘
                                                                                │
                                                        recv()│
                                                                                ▼
                                                 ┌─────────────┐
                                                 │ inBuffer_   │
                                                 └─────────────┘
                                                                                │
                                                                                │ aplicação lê
                                                                                ▼
                                                        HTTP Parser
                                                                                │
                                                                                ▼
                                                 resposta gerada
                                                                                │
                                                                                ▼
                                                 ┌─────────────┐
                                                 │ outBuffer_  │
                                                 └──────┬──────┘
                                                                                │
                                                                 send()
                                                                                │
                                                                                ▼
                                                                 Socket
                                                                                │
                                                                                ▼
                                                                 INTERNET

*/

#pragma once

#include "network/Socket.hpp"

#include <cstddef>
#include <memory>
#include <string>

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {

public:
  using Ptr = std::shared_ptr<TcpConnection>;

  TcpConnection(Socket socket, std::string clientIp, uint16_t clientPort);
  ~TcpConnection() = default;

  // desabilita copia igual no socket
  TcpConnection(const TcpConnection &) = delete;
  TcpConnection &operator=(const TcpConnection &) = delete;

  // movimentação
  TcpConnection(TcpConnection &&) noexcept = default;
  TcpConnection &operator=(TcpConnection &&) noexcept = default;

  bool isConnected() const { return socket_.isValid(); }
  int getFd() const { return socket_.getFd(); }
  const std::string &getClientIp() const { return clientIp_; }
  uint16_t getClientPort() const { return clientPort_; }

  void close();

  ssize_t readToBuffer();
  ssize_t send(const std::string &data);

  ssize_t flushOutputBuffer();

  std::string &getInBuffer() { return inBuffer_; }
  const std::string &getInBuffer() const { return inBuffer_; }

  void consumeInBuffer(std::size_t bytes);

  void appendToOutBuffer(const std::string &data) { outBuffer_ += data; }

private:
  Socket socket_;
  std::string clientIp_;
  uint16_t clientPort_;

  std::string inBuffer_;
  std::string outBuffer_;
};