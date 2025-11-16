@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Генерация проекта Visual Studio
echo ========================================
echo.

REM Определяем путь к проекту
set PROJECT_DIR=%~dp0
cd /d "%PROJECT_DIR%"

REM Создаем папку build, если её нет
if not exist "build" (
    echo Создание папки build...
    mkdir build
)

cd build

REM Проверяем наличие CMake
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ОШИБКА: CMake не найден в PATH!
    echo Убедитесь, что CMake установлен и добавлен в переменную окружения PATH.
    pause
    exit /b 1
)

REM Определяем версию Visual Studio
REM Пробуем Visual Studio 2022 (17)
cmake --version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Использование генератора Visual Studio 17 2022...
    cmake -G "Visual Studio 17 2022" -A x64 ..
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo ========================================
        echo Проект успешно сгенерирован!
        echo ========================================
        echo.
        echo Файл решения находится в: %CD%\sw_battle_test.sln
        echo.
        echo Для открытия проекта:
        echo   1. Откройте sw_battle_test.sln в Visual Studio
        echo   2. Или запустите: start sw_battle_test.sln
        echo.
        pause
        exit /b 0
    )
)

REM Если VS 2022 не сработал, пробуем VS 2019 (16)
echo Попытка использовать Visual Studio 16 2019...
cmake -G "Visual Studio 16 2019" -A x64 ..
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Проект успешно сгенерирован!
    echo ========================================
    echo.
    echo Файл решения находится в: %CD%\sw_battle_test.sln
    echo.
    echo Для открытия проекта:
    echo   1. Откройте sw_battle_test.sln в Visual Studio
    echo   2. Или запустите: start sw_battle_test.sln
    echo.
    pause
    exit /b 0
)

REM Если ничего не сработало
echo.
echo ОШИБКА: Не удалось сгенерировать проект Visual Studio!
echo Убедитесь, что установлена Visual Studio 2019 или 2022.
echo.
pause
exit /b 1

