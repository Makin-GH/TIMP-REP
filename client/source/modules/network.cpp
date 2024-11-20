#include <cstring>
#include <unistd.h>
#include <iostream>

#include "network.h"
#include "exceptions.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>

// Конструктор
NetworkManager::NetworkManager(const std::string &address, uint16_t port)
    : address(address), port(port), socket(-1) {}

std::string &NetworkManager::getAddress()
{
    return this->address;
};
uint16_t &NetworkManager::getPort()
{
    return this->port;
};
// Метод для установки соединения
void NetworkManager::conn()
{
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket < 0)
    {
        throw NetworkException("Failed to create socket", "NetworkManager.conn()");
    }

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);

    if (inet_pton(AF_INET, this->address.c_str(), &server_addr.sin_addr) <= 0)
        throw NetworkException("Invalid address/ Address not supported", "NetworkManager.conn()");

    if (connect(this->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        throw NetworkException("Connection failed", "NetworkManager.conn()");
}

// Метод для аутентификации
void NetworkManager::auth(const std::string &login, const std::string &password)
{
    // Отправка логина серверу
    if (send(this->socket, login.c_str(), login.size(), 0) < 0)
    {
        throw AuthException("Failed to send login", "NetworkManager.auth()");
    }

    // Получение соли от сервера
    char salt[17]; // Соль должна быть 16 символов
    int salt_length = recv(this->socket, salt, sizeof(salt) - 1, 0);
    if (salt_length < 0)
    {
        throw AuthException("Failed to receive salt", "NetworkManager.auth()");
    }
    salt[salt_length] = '\0';

    // Вычисление хеша с использованием CryptoPP
    CryptoPP::SHA256 hash_func; // создаем объект хеш-функции
    std::string hash_hex;

    // формирование хэша и преобразование в шестнадцатеричную строку
    CryptoPP::StringSource(
        std::string(salt) + password,
        true,
        new CryptoPP::HashFilter(
            hash_func,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(hash_hex),
                true // Заглавные буквы
                )));

    // Отправка хеша серверу
    if (send(this->socket, hash_hex.c_str(), hash_hex.size(), 0) < 0)
    {
        throw AuthException("Failed to send hash", "NetworkManager.auth()");
    }

    // Получение ответа от сервера
    char response[1024];
    int response_length = recv(this->socket, response, sizeof(response) - 1, 0);
    if (response_length < 0)
    {
        throw AuthException("Failed to receive auth response", "NetworkManager.auth()");
    }

    response[response_length] = '\0';
    if (std::string(response) != "OK")
    {
        throw AuthException("Authentication failed", "NetworkManager.auth()");
    }
}

std::vector<int32_t> NetworkManager::calc(const std::vector<std::vector<int32_t>> &data)
{
    // Передача количества векторов
    uint32_t num_vectors = data.size();
    if (send(this->socket, &num_vectors, sizeof(num_vectors), 0) < 0)
    {
        throw NetworkException("Failed to send number of vectors", "NetworkManager.calc()");
    }

    // Передача каждого вектора
    for (const auto &vec : data)
    {
        uint32_t vec_size = vec.size();
        if (send(this->socket, &vec_size, sizeof(vec_size), 0) < 0)
        {
            throw NetworkException("Failed to send vector size", "NetworkManager.calc()");
        }
        if (send(this->socket, vec.data(), vec_size * sizeof(int32_t), 0) < 0)
        {
            throw NetworkException("Failed to send vector data", "NetworkManager.calc()");
        }
    }

    // Получение результатов
    std::vector<int32_t> results(num_vectors);
    for (uint32_t i = 0; i < num_vectors; ++i)
    {
        if (recv(this->socket, &results[i], sizeof(int32_t), 0) < 0)
        {
            throw NetworkException("Failed to receive result", "NetworkManager.calc()");
        }
    }

    // Логирование результата
    std::cout << "Log: \"NetworkManager.calc()\"\n";
    std::cout << "Results: {";
    for (const auto &val : results)
    {
        std::cout << val << ", ";
    }
    if (!results.empty())
    {
        std::cout << "\b\b"; // Удалить последнюю запятую и пробел
    }
    std::cout << "}\n";

    return results;
}

// Метод для закрытия соединения
void NetworkManager::close()
{
    if (this->socket >= 0)
    {
        ::close(this->socket);
        this->socket = -1;
    }
}
