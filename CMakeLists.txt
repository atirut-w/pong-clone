add_executable("pongclone"
    "src/main.cpp"
)
target_include_directories("pongclone" PRIVATE "include")
target_link_libraries("pongclone" PRIVATE "raylib")

add_subdirectory("3rd/raylib")
