#include "io.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

// Конструктор
InputOutputManager::InputOutputManager(
    const std::string &config,
    const std::string &input,
    const std::string &output)
    : config(config),
      input(input),
      output(output) {}

// Метод для чтения конфигурационных данных
std::array<std::string, 2> InputOutputManager::conf()
{
    std::ifstream conf_file(this->config);
    if (!conf_file.is_open())
    {
        throw InputOutputException(
            "Failed to open configuration file \"" +
                this->config + "\"",
            "InputOutputManager.conf()");
    }

    std::array<std::string, 2> credentials;
    std::string line;
    std::getline(conf_file, line);
    conf_file.close();

    std::istringstream iss(line);
    std::getline(iss, credentials[0], ':');
    std::getline(iss, credentials[1]);

    // Проверка на наличие обоих значений
    if (credentials[0].empty() || credentials[1].empty())
    {
        throw DataDecodeException(
            "Missing login or password",
            "InputOutputManager.conf()");
    }

    // Логирование считанных логина и пароля
    std::cout << "Log: \"InputOutputManager.conf()\"\n";
    std::cout << "UserData: " << credentials[0]
              << " " << credentials[1] << "\n";

    return credentials;
}

std::vector<std::vector<int32_t>> InputOutputManager::read()
{
    std::ifstream input_file(this->input);
    if (!input_file.is_open())
        throw InputOutputException(
            "Failed to open input file for reading.",
            "InputOutputManager.read()");

    // Чтение количества векторов
    uint32_t num_vectors;
    input_file >> num_vectors;

    std::vector<std::vector<int32_t>> data(num_vectors);

    // Чтение каждого вектора
    for (uint32_t i = 0; i < num_vectors; ++i)
    {
        // Чтение размера вектора
        uint32_t vector_size;
        input_file >> vector_size;

        // Чтение значений вектора
        std::vector<int32_t> vec(vector_size);
        for (uint32_t j = 0; j < vector_size; ++j)
        {
            input_file >> vec[j]; // Чтение в десятичном формате
        }

        data[i] = vec;
    }

    input_file.close();

    // Логирование всех прочитанных векторов
    std::cout << "Log: InputOutputManager.read()\n";
    std::cout << "Vectors: {";
    for (const auto &vec : data)
    {
        std::cout << "{";
        for (const auto &val : vec)
            std::cout << val << ", ";
        if (!vec.empty())
            std::cout << "\b\b";
        std::cout << "}, ";
    }
    if (!data.empty())
        std::cout << "\b\b";
    std::cout << "}\n";

    return data;
}

// Метод для записи числовых данных в текстовый файл
void InputOutputManager::write(const std::vector<int32_t> &data)
{
    std::ofstream output_file(this->output);
    if (!output_file.is_open())
    {
        throw InputOutputException(
            "Failed to open output file \"" +
                this->output + "\"",
            "InputOutputManager.write()");
    }

    // Запись количества результатов
    uint32_t count = data.size();
    output_file << count << "\n";

    // Запись самих результатов
    for (const auto &num : data)
    {
        output_file << num << " "; // Запись значений без дополнения нулями
    }
    output_file << "\n";

    output_file.close();
}
