/**
 *  @file  msd200_cleanup.cpp
 *  @brief Очистка мусора из CSV-файлов для модуля ОВЕН МСД-200
 *         (https://owen.ru/product/msd200)
 *
 *  @note К сожалению, вот уже много лет, сам производитель данного
 *        модуля ничего не делает для исправления детских и дурацких
 *        ошибок как при записи данных на самом модуле, так и при
 *        синхронизации из конфигуратора и, особенно, при загрузке
 *        данных в "Программу для построения графиков МСД-200".
 *        Данная программа очищает строки с ошибками:
 *            - Повторяющиеся строки
 *            - Повторяющиеся блоки
 *            - Некорректные строки (например, неполные)
 *            - Заголовки файла кроме первой строки
 *
 *  @note Как использовать:
 *            - Компилировать с поддержкой C++11, например для Linux:
 *              clang++ --std=c++11 -o bin/msd200_cleanup msd200_cleanup.cpp
 *
 *            - В командной строке передать исходный файл и целевой,
 *              например для Linux:
 *              ./msd200_cleanup 2019_10_03.csv ../out/2019_10_03.csv
 *
 * (c) Nikita Bulaev 2019
 */

#include "msd200_cleanup.h"

int main(int argc, char **args) {

    std::fstream  f;    /*!< Указатель на обрабатываемый файл */
    std::ofstream otxt; /*!< Указатель на текстовый файл с результатами */

    std::string currLine;  /*!< Текущая строка */
    std::string lastLine;  /*!< Предыдущая строка */

    unsigned long int currStamp = 0;  /*!< Время в текущей строке */
    unsigned long int lastStamp = 0;  /*!< Время в предыдущей строке */

    const std::regex baseLine("([\\d]+)\\:([\\d]+)\\:([\\d]+);(\\-{0,1}\\d{0,4};){0,63}(\\-{0,1}\\d{0,4}){1}(#([A-F0-9]){0,32}#){0,1}\\r$");
    std::smatch piecesMatch;  /*!< Раскладка строки по группам */

    if (argc == 1) {
        std::cout << "No file(s) specified in command line" << std::endl;
        return 1;
    }

    f.open(args[1], std::ios::in);

    if (!f.is_open()) {
        std::cout << "Cannot access to file " << args[1] << ". Check if file is present and have correct access rights."
             << std::endl;
        return 1;
    }

    otxt.open(args[2], std::ios::out);

    if (!otxt.is_open()) {
        std::cout << "Cannot create file " << args[2] << ". Check that you have correct access rights."
             << std::endl;
        return 1;
    }

    getline(f, currLine);
    otxt << currLine << std::endl;

    while (getline (f, currLine)) {
        // Если одинаковые строки, вывод ошибки и пропуск строки
        if (currLine.compare(lastLine) == 0) {
            std::cout << "Same string: " << currLine << std::endl;

            continue;
        }

        if (std::regex_match(currLine, piecesMatch, baseLine)) {
            std::ssub_match subMatch = piecesMatch[1];  /*!< Текущая группа regex */
            std::string hours = subMatch.str();    /*!< Часы */

            subMatch = piecesMatch[2];
            std::string minutes = subMatch.str();  /*!< Минуты */

            subMatch = piecesMatch[3];
            std::string seconds = subMatch.str();  /*!< Секунды */

            std::string stamp = hours.append(minutes).append(seconds);  /*!< Время в текущей строке в виде строки */

            currStamp = stoul(stamp, nullptr, 0);  // Переводим время из строки в число

            if (currStamp <= lastStamp) {
                // Выводим ошибку и пропускаем строку, если
                // время в текущей строке меньше предыдущей
                std::cout << "Time is less then last string: " << currLine << std::endl;
            } else {
                otxt << currLine << std::endl;
                lastLine  = currLine;
                lastStamp = currStamp;
            }
        } else {
            // Если нет совпадения по regex, вывод ошибки и пропуск строки
            std::cout << "Incorrect string: " << currLine << std::endl;
        }
    }

    f.close();
    otxt.close();

    return 0;
}
