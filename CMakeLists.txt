cmake_minimum_required(VERSION 3.5)
project(PruebaOpenGL)

add_executable(main 
"src/main.cpp" 
"src/glad.c" 
"src/IndexBuffer.cpp" 
"src/Renderer.cpp" 
"src/Shader.cpp" 
"src/Texture.cpp" 
"src/Utils.cpp" 
"src/VertexArray.cpp" 
"src/VertexBuffer.cpp" 
"src/Camera.cpp"
"src/VertexBufferLayout.cpp" 
"src/vendor/stb/stb_image.cpp" 
"src/figures/Square.cpp" 
"src/figures/Circle.cpp" 
"src/vendor/imgui/imgui_demo.cpp"
"src/vendor/imgui/imgui_draw.cpp"
"src/vendor/imgui/imgui_impl_glfw_gl3.cpp"
"src/vendor/imgui/imgui.cpp"
"src/tests/Test.cpp"
"src/tests/TestClearColor.cpp"
"src/tests/TestTexture.cpp"
"src/tests/TestFPS.cpp"
"src/tests/TestHugeObject.cpp"
"src/tests/TestLight.cpp"
"src/tests/TestLightTexture.cpp"
"src/tests/TestCollisions.cpp"
)
target_include_directories(main PRIVATE include)
target_include_directories(main PRIVATE src)
target_include_directories(main PRIVATE src/vendor)
target_include_directories(main PRIVATE src/figures)

target_compile_features(main PRIVATE cxx_std_17)

find_library(LIB_NAME glfw3 PATHS lib)

target_link_libraries(main PRIVATE "${LIB_NAME}")


