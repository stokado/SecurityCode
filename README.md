# Парсер JSON и EDID файлов

## WSL/Linux

---

**0. Установить WSL и Linux дистрибутив (опционально)**

* Установить [WSL](https://docs.microsoft.com/en-us/windows/wslinstall)
* Установить дистрибутив Linux (протестировано на Ubuntu 20.04 LTS)

---

**1. Обновляем базу данных с доступными пакетами и устанавливаем их**

	sudo apt update && sudo apt -y upgrade

---

**2. Устанавливаем пакеты и библиотеки для сборки**

	sudo apt install cmake              // Установка    CMake
	sudo apt install build-essential    // Установка    компилятора

---

**3. Скачиваем Git репозиторий**
    
    git clone https://github.com/stokado/SecurityCode

---

**4. Меняем директорию**

	* cd SecurityCode/

---

**5. Собираем проект и компилируем код**

	* mkdir build/                  // папка для сконфигурированного проекта
	* cd build/                     // заходим в директорию с проектом
	* cmake ..                      // команда CMake для конфигурирования проекта в текущую папку
	* cmake --build .               // команда CMake для запуска компиляции с нативным компилятором из текущей папки
	* cd ../install/    // заходим в директорию с установленными программами

---

**6. Запуск программы JSON parser**

    * ./TaskOne/TaskOne ../Tests/TaskOne/test.json

---

**7. Запуск программы EDID parser**

    * ./TaskTwo/TaskTwo ../Tests/TaskTwo/edid1.bin

---

## Windows

---

**1. Установливаем необходимые утилиты**

    CMake
    Ninja
    mingw

---

**2. Скачиваем Git репозиторий**
    
    git clone https://github.com/stokado/SecurityCode

---

**3. Меняем директорию**

	* cd SecurityCode/

---

**4. Собираем проект и компилируем код**

	* mkdir build/
	* cd build/
	* cmake -G Ninja ..     // используем генератор Ninja
	* cmake --build .       

---

**5. Запуск программ**

    SecurityCode/install/TaskOne/TaskOne.exe <arg>
    SecurityCode/install/TaskTwo/TaskTwo.exe <arg>

--- 
