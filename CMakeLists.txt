cmake_minimum_required(VERSION 3.15)
project(reverse_proxy CXX)

# Exportar comandos de compilação para o Servidor de Linguagem (Language Server / IDE) encontrar os includes
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Exigir C++17 para recursos modernos do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Habilitar avisos (warnings) do compilador para garantir a qualidade do código
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -pthread)
endif()

# Buscar biblioteca de Threads (essencial para um reverse proxy concorrente)
find_package(Threads REQUIRED)

# Listar todas as fontes do projeto
set(SOURCES
    src/main.cpp
    src/config/ProxyConfig.cpp
    src/network/TcpServer.cpp
    src/network/TcpConnection.cpp
    src/network/Socket.cpp
    src/proxy/ReverseProxy.cpp
    src/proxy/ConnectionHandler.cpp
    src/load_balancer/Backend.cpp
    src/load_balancer/BackendPool.cpp
    src/load_balancer/RoundRobinBalancer.cpp
    src/health/HealthChecker.cpp
    src/rate_limit/TokenBucket.cpp
    src/rate_limit/RateLimiter.cpp
    src/utils/Logger.cpp
)

# Definir o executável do projeto
add_executable(reverse_proxy ${SOURCES})

# Configurar diretório de includes para permitir inclusões como #include "load_balancer/Backend.hpp"
target_include_directories(reverse_proxy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Vincular bibliotecas necessárias (como Threads)
target_link_libraries(reverse_proxy PRIVATE Threads::Threads)
