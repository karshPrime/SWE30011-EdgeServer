
add_executable( ${PROJECT_NAME}
    main.c
    Database.c
    Link.c
    Process.c
    Server.c
)

target_include_directories( ${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include )

