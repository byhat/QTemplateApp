# Project Template
Шаблон приложения на Qt6/QML/C++ для Orange/Raspberry/Banana Pi (аналог, не имеющий аналогов).

## Setup build tools
```
sudo apt update && sudo apt upgrade -y
```
```
sudo apt install build-essential cmake git doxygen
```
```
sudo apt install qt6-base-dev qt6-declarative-dev qml6-module-*
```

## Build
```
cmake -S ./ -B ./build/
```
```
cmake --build ./build --target all --parallel $(nproc)
```

## Documentation
```
cd doxygen
doxygen ./Doxyfile
```

## Autostart
Для автоматического запуска приложения после старта операционной системы необходимо прописать путь к исполняемому файлу переменной Exec в tapp.desktop. В зависимости от условий путь к исполняемому файлу разный. Затем выполнить следующую команду:
```
sudo cp ./tools/tapp.desktop /etc/xdg/autostart/
```
Если требуется root доступ, то прописать 
```
sudo visudo
```
и добавить в конец файла
```
orangepi ALL=(ALL) NOPASSWD: /home/orangepi/TemplateApp
```
## Other
### Qt Creator build setup:
В разделе Build поле CMake arguments: ```--parallel  $(nproc)```

### Backup
Если вдруг понадобится бекап всей CD-карты с системой, то:
```
sudo fdisk -l /dev/sdX
```
Вставить вместо "X" значение End для /dev/sdb2 
Возможный вывод:
Device       Start       End   Sectors   Size Type
/dev/sdb1    61440   2158591   2097152     1G Linux extended boot
/dev/sdb2  2158592 247824383 245665792 117,1G Linux filesystem
```
sudo dd if=/dev/sdX of=linux.img bs=512 count=X status=progress
```
