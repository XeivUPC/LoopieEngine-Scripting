cmake_minimum_required(VERSION 3.10)
project("LoopieScripting" NONE)

set(CS_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/LoopieScripting.csproj")

set(CS_OUTPUT_DIR
    ${CMAKE_BINARY_DIR}/LoopieScripting
)

add_custom_target(LoopieScriptingBuild ALL
    COMMAND dotnet build "${CS_PROJECT}"
        -c $<CONFIG>
        -o "${CS_OUTPUT_DIR}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Building LoopieScripting (C#)"
)