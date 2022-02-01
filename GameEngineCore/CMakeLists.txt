cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME GameEngineCore)

#переменные для всех инклудов (hpp)
set(ENGINE_PUBLIC_INCLUDES 
	includes/GameEngineCore/Application.hpp
	includes/GameEngineCore/Log.hpp
	includes/GameEngineCore/Event.hpp
)

set(ENGINE_PRIVATE_INCLUDES
	src/GameEngineCore/Window.hpp
)

#переменные для всех исходных кодов (cpp)
set(ENGINE_PRIVATE_SOURCES 
	src/GameEngineCore/Application.cpp
	src/GameEngineCore/Window.cpp
)

#компилируем статическую библиотеку для проекта
add_library(${ENGINE_PROJECT_NAME} STATIC 
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PRIVATE_SOURCES}
)

#линкуем все, что находится внутри папки inclues
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)

#указываем нужный компилятор
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

#вызывает магию cmake внутри glfw
add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)

#линкуем glfw к таргету, при этом glfw будет использоваться ТОЛЬКО внутри ядра
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

#spdlog
add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

#glad
add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)