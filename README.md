**Очистка мусора из CSV-файлов для модуля [ОВЕН МСД-200](https://owen.ru/product/msd200)**

К сожалению, вот уже много лет, сам производитель данного
модуля ничего не делает для исправления детских и дурацких
ошибок как при записи данных на самом модуле, так и при
синхронизации из конфигуратора и, особенно, при загрузке
данных в "Программу для построения графиков МСД-200".

Данная программа очищает строки с ошибками:
 - Повторяющиеся строки
 - Повторяющиеся блоки
 - Некорректные строки (например, неполные)
 - Заголовки файла кроме первой строки

Как использовать:
- Компилировать с поддержкой C++11, например для Linux:
  > clang++ --std=c++11 -o bin/msd200_cleanup msd200_cleanup.cpp

- В командной строке передать исходный файл и целевой,
  например для Linux:
  > ./msd200_cleanup 2019_10_03.csv ../out/2019_10_03.csv
