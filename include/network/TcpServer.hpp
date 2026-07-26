// responsabilidade: mantem instancia interna do Socket | inicializa o servidor na porta config | executa loop de aceitação (empacotara a nova conexao e passara para o Connection Handler tratar)
#pragma once

#include "network/Socket.hpp"
#include "network/TcpConnection.hpp"

#include <atomic>
#include <cstdint>
#include <functional>
#include <string>

class TcpServer
{
public:
	// Callback disparado sempre que um novo cliente se conecta com sucesso
	using NewConnectionCallback = std::function<void(TcpConnection::Ptr)>;

	explicit TcpServer(uint16_t port);
	~TcpServer();

	// Desabilita cópia
	TcpServer(const TcpServer &) = delete;
	TcpServer &operator=(const TcpServer &) = delete;

	// Prepara o servidor (socket -> bind -> listen)
	bool start();

	// Para o loop de escuta
	void stop();

	// Define qual função/lambda será chamada quando chegar um novo cliente
	void setNewConnectionCallback(NewConnectionCallback cb) { onNewConnection_ = std::move(cb); }

	// Inicia o loop de aceitação (bloqueante ou a ser rodado numa thread dedicada)
	void run();

	bool isRunning() const { return isRunning_; }
	uint16_t getPort() const { return port_; }

private:
	uint16_t port_;
	Socket listenSocket_;
	std::atomic<bool> isRunning_{false};

	NewConnectionCallback onNewConnection_;
};