add_executable(unit_test_shell
    test_commands.c
    #test_shell.c
)

target_link_libraries(unit_test_shell
    PRIVATE
    survShell_lib
    ${CJSON_LIBRARY}
)

add_test(NAME shell_tests COMMAND unit_test_shell)
