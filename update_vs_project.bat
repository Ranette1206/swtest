@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Обновление проекта Visual Studio
echo ========================================
echo.

REM Определяем путь к проекту
set PROJECT_DIR=%~dp0
cd /d "%PROJECT_DIR%"

REM Проверяем наличие папки build
if not exist "build" (
    echo Папка build не найдена. Запускаю генерацию проекта...
    call "%~dp0generate_vs_project.bat"
    exit /b %ERRORLEVEL%
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

REM Обновляем проект через CMake
echo Обновление проекта...
cmake ..
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Проект успешно обновлен!
    echo ========================================
    echo.
    pause
    exit /b 0
) else (
    echo.
    echo ОШИБКА: Не удалось обновить проект!
    echo Попробуйте удалить папку build и запустить generate_vs_project.bat заново.
    echo.
    pause
    exit /b 1
)

