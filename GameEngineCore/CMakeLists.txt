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
	src/GameEngineCore/Rendering/OpenGL/ShaderProgram.hpp
	src/GameEngineCore/Rendering/OpenGL/VertexBuffer.hpp
	src/GameEngineCore/Rendering/OpenGL/VertexArray.hpp
	src/GameEngineCore/Rendering/OpenGL/IndexBuffer.hpp
	src/GameEngineCore/Rendering/OpenGL/Camera.hpp
	src/GameEngineCore/Rendering/OpenGL/Texture.hpp
	src/GameEngineCore/Rendering/OpenGL/Renderer.hpp
	src/GameEngineCore/Rendering/OpenGL/CameraObject.hpp
	src/GameEngineCore/Rendering/OpenGL/Object.hpp
	src/GameEngineCore/Rendering/OpenGL/Mesh.hpp
	src/GameEngineCore/Rendering/OpenGL/Model.hpp
)

#переменные для всех исходных кодов (cpp)
set(ENGINE_PRIVATE_SOURCES 
	src/GameEngineCore/Application.cpp
	src/GameEngineCore/Window.cpp
	src/GameEngineCore/Rendering/OpenGL/ShaderProgram.cpp
	src/GameEngineCore/Rendering/OpenGL/VertexBuffer.cpp
	src/GameEngineCore/Rendering/OpenGL/VertexArray.cpp
	src/GameEngineCore/Rendering/OpenGL/IndexBuffer.cpp
	src/GameEngineCore/Rendering/OpenGL/Camera.cpp
	src/GameEngineCore/Rendering/OpenGL/Texture.cpp
	src/GameEngineCore/Rendering/OpenGL/Renderer.cpp
	src/GameEngineCore/Rendering/OpenGL/CameraObject.cpp
	src/GameEngineCore/Rendering/OpenGL/Object.cpp
	src/GameEngineCore/Rendering/OpenGL/Mesh.cpp
	src/GameEngineCore/Rendering/OpenGL/Model.cpp
)

#компилируем статическую библиотеку для проекта

set(ENGINE_ALL_SOURCES 
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PRIVATE_SOURCES}
)
add_library(${ENGINE_PROJECT_NAME} STATIC 
	${ENGINE_ALL_SOURCES}
)
#файловая структура отображается в Visual Studio как в проводнике
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${ENGINE_ALL_SOURCES}
)

#линкуем все, что находится внутри папки incldues
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

#glm
add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glm)

#soil
add_subdirectory(../external/soil ${CMAKE_CURRENT_BINARY_DIR}/soil)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE soil)

#assimp
add_subdirectory(../external/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE assimp)


set(IMGUI_INCLUDES 
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_demo.cpp
	../external/imgui/imgui_draw.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
)
add_library(ImGui STATIC 
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)

target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)