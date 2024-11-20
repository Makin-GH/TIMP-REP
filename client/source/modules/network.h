#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <string>
#include <vector>
#include <cstdint>

/** 
* @file network.h
* @brief Определения классов для управления сетевым взаимодействием.
* @details Этот файл содержит определения классов для управления сетевыми подключениями и передачей данных.
* @date 23.11.2024
* @version 1.0
* @authors Макин Д. Ю.
* @copyright ИБСТ ПГУ
*/

/** 
* @brief Класс для управления сетевым подключением и взаимодействием.
*/
class NetworkManager
{
public:
    /**
    * @brief Конструктор класса NetworkManager.
    * @param address Адрес сервера.
    * @param port Порт сервера.
    */
    NetworkManager(const std::string &address, uint16_t port);

    /**
    * @brief Метод для получения адреса сервера.
    * @return Адрес сервера.
    */
    std::string &getAddress();

    /**
   * @brief Метод для получения порта сервера.
    * @return Порт сервера.
    */
    uint16_t &getPort();

    /**
    * @brief Метод для установления сетевого подключения.
    * @throw NetworkException Если не удалось создать сокет, установить соединение или адрес не поддерживается.
    */
    void conn();

    /**
    * @brief Метод для аутентификации пользователя.
    * @param username Имя пользователя.
    * @param password Пароль.
    * @throw AuthException Если не удалось отправить логин, получить соль, отправить хеш или аутентификация не удалась.
    */
    void auth(const std::string &username, const std::string &password);

    /**
    * @brief Метод для передачи данных и получения результата.
    * @param data Данные для обработки.
    * @return Результаты обработки данных.
    * @throw NetworkException Если не удалось отправить или получить данные.
    */
    std::vector<int32_t> calc(const std::vector<std::vector<int32_t>> &data);

    /**
    * @brief Метод для закрытия сетевого подключения.
    */
    void close();

private:
    int socket; ///< Сокет подключения.
    std::string address; ///< Адрес сервера.
    uint16_t port; ///< Порт сервера.
};

#endif // NETWORK_MANAGER_H
