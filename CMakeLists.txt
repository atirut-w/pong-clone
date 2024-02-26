add_subdirectory("3rd/raylib")
add_subdirectory("nodetree")

add_executable("pongclone"
    "src/main.cpp"
    "src/node2d.cpp"
    "src/label.cpp"
)
target_include_directories("pongclone" PRIVATE "include")
target_link_libraries("pongclone" PRIVATE "raylib" "nodetree")
