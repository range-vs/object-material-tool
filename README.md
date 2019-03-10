# Object Material Tool

## English description
**Autors:**

tervel - algorithm. [livejournal.com](https://tervel.livejournal.com) [github.com] (https://github.com/TerVel76)

range36rus - adaptation for .NET and C++/CLI/Win Forms. It's me :).

A program that allows you to automatically adjust the materials in a 3d-model format *. Object.
Supports any objects from X-Ray SDK version 0.4 - 0.7.

### Use:
* Run the program
* Select any model in the format * .object or a folder with models of the format * .object
* Choose save path
* Get ready file (s)

### How it works?
Sometimes, when exporting/decompiling/retrieving locations from builds incorrectly, you need to adjust a huge amount of materials. The program does it
for you. All that is required is to have the config material_full.ini(the file is in the root of the program, look for it there), which describes the custom sections. Sections can be replenished,
the instruction will be described below

### Add-on instruction for the material_full.ini file:
```
#texture path = engine shader; compiler shader; game materials; flags (2 sided, etc)
# this comments

act\act_flow = models\window;def_shaders\def_vertex;objects\glass;1

Some blocks can be skipped by simply inserting a ';'
act\act_flow = models\window ;;; 1

Paths are written without spaces
```

## Русское описание
**Авторы:**

tervel - алгоритм. [livejournal.com](https://tervel.livejournal.com)

range36rus - адаптация для .NET и C++/CLI/Win Forms. Это я:)

Программа, которая позволяет производить автоматическую настройку материалов в 3d-модели формата *.object.
Поддерживает любые объекты от X-Ray SDK версии 0.4 - 0.7.

### Использование:
* Запустите программу
* Выберите любую модель в формате *.object или папку с моделями формата *.object
* Выберите путь сохранения
* Получите готовый файл(-ы)

### Как это работает?
Иногда, при не корректном экспорте/декомпиляции/получении локации из билдов требуется настроить огромное количество матриалов. Программа делает это
за Вас. Всё что требуется - иметь конфиг material_full.ini(файл лежит в корне программы ищите его там.), в котором описаны настраиваемые секции. Секции можно пополнять, 
инструкция будет описана ниже

### Инструкция дополнения файла material_full.ini:
```
#texture path = engine shader;compiler shader;game materials;flags (2 sided, etc)
#это комментарии

act\act_flow = models\window;def_shaders\def_vertex;objects\glass;1

Некоторый блоки можно пропускать, просто вставляя ';'
act\act_flow = models\window;;;1

Пути записываются без пробелов
```
