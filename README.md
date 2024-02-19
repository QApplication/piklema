# piklema
MQTT client with QML interface

# Download and Install QtMqtt
1. Скачиваем и устанавливаем Qt
2. Скачиваем исходники QtMqtt [здесь](https://code.qt.io/cgit/qt/qtmqtt.git/)
3. Переключаемся на ветку QtMQtt сообтветствующей вашей версии бинарной сборки Qt
4. Собираем Debug and Release
   ```
   ~/Qt/Tools/CMake/bin/cmake --build .
   ```
6. Устанавливаем QtMQtt в локацию Qt
   ```
   ~/Qt/Tools/CMake/bin/cmake --install . --verbose
   ```
7. Собираем текущий проект
8. Определяем и собираем зависимсти
```
ldd /path_to_exe_file/ -> dep.txt
```
9. Запускаем
   
# Result
[]![Screenshot 2024-02-17 174914](https://github.com/QApplication/piklema/assets/31853792/26310acc-a27c-41b8-9415-70facaa1d9b0)

![Screenshot 2024-02-18 001358](https://github.com/QApplication/piklema/assets/31853792/949ce0ba-863b-41c2-a7f3-4b0a1a82496d)
