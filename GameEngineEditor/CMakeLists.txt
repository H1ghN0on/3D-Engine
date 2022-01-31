cmake_minimum_required(VERSION 3.12)

set(EDITOR_PROJECT_NAME GameEngineEditor)

#exe-шник генерируется на основе cpp файла
add_executable(${EDITOR_PROJECT_NAME}
	src/main.cpp
)

#линкуем ядро к editor (когда будем менять ядро, то будет меняться и editor)
target_link_libraries(${EDITOR_PROJECT_NAME} GameEngineCore)

#указываем нужный компилятор
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)

#exe-шник будет билдится в папке build/bin
set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)