#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <string>
#include <vector>
#include <array>
#include "exceptions.h"

/** 
* @file io.h
* @brief Определения классов для управления вводом-выводом.
* @details Этот файл содержит определения классов для работы с файлами ввода-вывода и конфигурационными данными.
* @date 23.11.2024
* @version 1.0
* @authors Макин Д. Ю.
* @copyright ИБСТ ПГУ
*/

/** 
* @brief Класс для управления вводом и выводом данных.
*/
class InputOutputManager {
public:
    /**
    * @brief Конструктор класса InputOutputManager.
    * @param config Путь к файлу конфигурации.
    * @param input Путь к входному файлу.
    * @param output Путь к выходному файлу.
    */
    InputOutputManager(
        const std::string& config, 
        const std::string& input, 
        const std::string& output
    );

    /**
    * @brief Метод для чтения конфигурационных данных.
    * @return Массив строк с конфигурационными данными.
    * @throw InputOutputException Если не удалось открыть файл конфигурации.
    * @throw DataDecodeException Если отсутствуют логин или пароль.
    */
    std::array<std::string, 2> conf();

    /**
    * @brief Метод для чтения данных из файла.
    * @return Двумерный вектор с данными.
    * @throw InputOutputException Если не удалось открыть входной файл для чтения.
    */
    std::vector<std::vector<int32_t>> read();

    /**
    * @brief Метод для записи данных в файл.
    * @param data Вектор данных для записи.
    * @throw InputOutputException Если не удалось открыть выходной файл для записи.
    */
    void write(const std::vector<int32_t>& data);

private:
    std::string config; ///< Путь к файлу конфигурации.
    std::string input; ///< Путь к входному файлу.
    std::string output; ///< Путь к выходному файлу.
};

#endif // IO_MANAGER_H
