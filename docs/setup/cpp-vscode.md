# С++ в VS Code
- Требования: MinGW (g++, gdb в C:\mingw64\bin\) и расширение ms-vscode.cpptools.

## launch.json

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug C++ (g++ + gdb)",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}\\${fileBasenameNoExtension}.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "C:\\mingw64\\bin\\gdb.exe",
            "preLaunchTask": "g++ build active file",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

## tasks.json

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "cppbuild",
            "label": "g++ build active file",
            "command": "C:\\mingw64\\bin\\g++.exe",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "-Wall",
                "-Wextra",
                "-std=c++17",
                "${file}",
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": ["$gcc"],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

> Конфиги должны быть в .vscode/ в корне проекта

## Кодировка русского текста 

```cpp
SetConsoleOutputCP(CP_UTF8) // #include <windows.h>
```

## Хоткеи

- `Shift` + `F5` - Остановка дебагинга
- `Ctrl` + `F5` - Запуск без дебагинга
- `F5` - Запуск с дебагингом
- `Ctrl` + `Shift` + `B` - Компиляция без последующего запуска
- `F10` и `F11` - Передвигаться внутри дебагинга