
find_package( Threads REQUIRED )
find_path( MYSQL_INCLUDE_DIR mysql/mysql.h )
find_library( MYSQL_CLIENT_LIB NAMES mysqlclient )

if( NOT MYSQL_INCLUDE_DIR OR NOT MYSQL_CLIENT_LIB )
    message(
        FATAL_ERROR
        "Could not find MySQL client development libraries. Install libmysqlclient-dev."
    )
endif()

add_executable( ${PROJECT_NAME}
    main.c
    Database.c
    Link.c
    Process.c
    Server.c
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${MYSQL_INCLUDE_DIR}
)

target_link_libraries( ${PROJECT_NAME}
    ${MYSQL_CLIENT_LIB}
    Threads::Threads
)

