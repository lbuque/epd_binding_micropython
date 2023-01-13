
set(url "https://github.com/Xinyuan-LilyGO/LilyGo-EPD47.git")
set(commit "deae2d2cbfb062198e6b4647d29eab6bc4d80d8a")

if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/lib)
    execute_process(COMMAND mkdir -p ${CMAKE_CURRENT_LIST_DIR}/lib)
endif()

if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/lib/LilyGo-EPD47)
    execute_process(COMMAND
                        git clone ${url}
                    WORKING_DIRECTORY
                        ${CMAKE_CURRENT_LIST_DIR}/lib)
    execute_process(COMMAND
                        git checkout ${commit}
                    WORKING_DIRECTORY
                        ${CMAKE_CURRENT_LIST_DIR}/lib/LilyGo-EPD47)
endif()
