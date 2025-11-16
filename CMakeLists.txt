cmake_minimum_required(VERSION 3.27)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 20)

# Опция для включения проверки кода
option(ENABLE_CODE_ANALYSIS "Enable code analysis tools (clang-tidy, cppcheck)" OFF)

# Функция для организации файлов по фильтрам в Visual Studio
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if(IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        string(REPLACE "src\\" "" _source_path_msvc "${_source_path_msvc}")
        if(_source_path_msvc)
            source_group("${_source_path_msvc}" FILES "${_source}")
        else()
            source_group("" FILES "${_source}")
        endif()
    endforeach()
endfunction(assign_source_group)

# Собираем все исходные файлы
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.hpp
    src/*.h
)

# Организуем файлы по фильтрам
assign_source_group(${SOURCES})

add_executable(sw_battle_test ${SOURCES})

target_include_directories(sw_battle_test PUBLIC src/)

# Настройка рабочей директории для отладки в Visual Studio
if(MSVC)
    set_target_properties(sw_battle_test PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)"
    )
endif()

# Настройка проверки кода
if(ENABLE_CODE_ANALYSIS)
    # clang-tidy
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(CLANG_TIDY_EXE)
        message(STATUS "clang-tidy найден: ${CLANG_TIDY_EXE}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" 
            -checks=*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers
            -warnings-as-errors=*)
    else()
        message(WARNING "clang-tidy не найден. Установите его для проверки кода.")
    endif()
    
    # cppcheck
    find_program(CPPCHECK_EXE NAMES cppcheck)
    if(CPPCHECK_EXE)
        message(STATUS "cppcheck найден: ${CPPCHECK_EXE}")
        # Добавляем cppcheck как кастомную команду
        add_custom_target(cppcheck
            COMMAND ${CPPCHECK_EXE}
                --enable=all
                --suppress=missingIncludeSystem
                --suppress=unusedFunction
                --inline-suppr
                --std=c++20
                ${CMAKE_SOURCE_DIR}/src
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Запуск cppcheck для проверки кода"
        )
    else()
        message(WARNING "cppcheck не найден. Установите его для проверки кода.")
    endif()
endif()
