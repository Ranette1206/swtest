@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Проверка кода проекта
echo ========================================
echo.

REM Определяем путь к проекту
set PROJECT_DIR=%~dp0
cd /d "%PROJECT_DIR%"

REM Проверяем наличие папки build
if not exist "build" (
    echo Папка build не найдена. Генерирую проект с включенной проверкой кода...
    if not exist "build" mkdir build
    cd build
    cmake -G "Visual Studio 17 2022" -A x64 -DENABLE_CODE_ANALYSIS=ON ..
    if %ERRORLEVEL% NEQ 0 (
        cmake -G "Visual Studio 16 2019" -A x64 -DENABLE_CODE_ANALYSIS=ON ..
    )
    if %ERRORLEVEL% NEQ 0 (
        echo ОШИБКА: Не удалось сгенерировать проект!
        pause
        exit /b 1
    )
) else (
    cd build
    REM Обновляем проект с включенной проверкой кода
    echo Обновляю проект с включенной проверкой кода...
    cmake -DENABLE_CODE_ANALYSIS=ON ..
    if %ERRORLEVEL% NEQ 0 (
        echo ОШИБКА: Не удалось обновить проект!
        pause
        exit /b 1
    )
)

echo.
echo ========================================
echo Запуск проверки кода
echo ========================================
echo.

REM Проверяем наличие clang-tidy
where clang-tidy >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo.
    echo [1/2] Запуск clang-tidy...
    echo.
    clang-tidy --version
    echo.
    REM Запускаем clang-tidy для всех исходных файлов
    cd /d "%PROJECT_DIR%"
    set ERRORS_FOUND=0
    for /r src %%f in (*.cpp *.hpp) do (
        echo Проверка: %%f
        clang-tidy "%%f" -p build --checks=*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers 2>nul
        if !ERRORLEVEL! NEQ 0 (
            set /a ERRORS_FOUND+=1
            echo ОШИБКА: Найдены проблемы в %%f
        )
    )
    if !ERRORS_FOUND! GTR 0 (
        echo.
        echo ВНИМАНИЕ: Найдено !ERRORS_FOUND! файлов с проблемами.
    )
    echo.
    echo [1/2] clang-tidy завершен.
    echo.
) else (
    echo ПРЕДУПРЕЖДЕНИЕ: clang-tidy не найден. Пропускаю проверку clang-tidy.
    echo Для установки: choco install llvm
    echo.
)

REM Проверяем наличие cppcheck
where cppcheck >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo.
    echo [2/2] Запуск cppcheck...
    echo.
    cppcheck --version
    echo.
    cd /d "%PROJECT_DIR%"
    cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction --inline-suppr --std=c++20 --platform=win64 src/
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo [2/2] cppcheck завершен успешно.
    ) else (
        echo.
        echo [2/2] cppcheck завершен с предупреждениями.
    )
    echo.
) else (
    echo ПРЕДУПРЕЖДЕНИЕ: cppcheck не найден. Пропускаю проверку cppcheck.
    echo Для установки: choco install cppcheck
    echo.
)

echo ========================================
echo Проверка кода завершена
echo ========================================
echo.
pause

