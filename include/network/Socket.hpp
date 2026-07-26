#pragma once

#include <cstdint>
#include <string>

class Socket {
public:
  // Construtor cria um socket (AF_INET, SOCK_STREAM por padrão)
  explicit Socket(int fd = -1);
  ~Socket();

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  /*
  Socket socket1;
  Socket socket2(socket1);  proibido

  Socket socket1;
  Socket socket2;

  socket2 = socket1;         proibido

  desabilita copia pra evitar que dois objetos fechem o mesmo file descriptor (o
  delete instrui ao comp proibir a copia de instancias dessa classe)
  */

  // Permite movimentação (Move Semantics)
  /*
    sendo a cópia proibida entao para fazer um retorn socket dentro por exemplo
    de da func accept oq fazemos é transferir a posse (ownership) do
    antigo(other) para o novo

    O novo objeto pega o fd_ de other.

    O objeto other tem seu fd_ alterado para -1 (inválido).

    Quando other for destruído, ele não fechará o socket, pois seu fd_ agora é
    -1.

    noexcept garante que nao lançara exceção

  */

  Socket(Socket &&other) noexcept;
  Socket &operator=(Socket &&other) noexcept;

  bool isValid() const { return fd_ != -1; }
  int getFd() const { return fd_; }

  void close();

  bool bind(uint16_t port);
  bool listen(int backlog = 128) const; // tamanho da fila de conexoes pendentes
  Socket accept(std::string &clientIp, uint16_t &clientPort) const;

  ssize_t send(const std::string &data) const;
  ssize_t receive(char *buffer, std::size_t size) const;

private:
  int fd_ = -1;
};

/*
  Resource Acquisition is initialization


*/