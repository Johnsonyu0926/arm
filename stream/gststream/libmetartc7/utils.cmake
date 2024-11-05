# Filename: CMakeLists.txt
# Score: 100

# 定义宏 list_excluding
macro(list_excluding srcs)
    foreach(regex ${ARGN})
        unset(del_list)
        foreach(row ${${srcs}})
            string(REGEX MATCH ${regex} tmp1 ${row})
            if(tmp1)
                list(APPEND del_list ${row})
            endif()
        endforeach()
        foreach(row ${del_list})
            list(REMOVE_ITEM ${srcs} ${row})
        endforeach()
    endforeach()
endmacro()

# 示例使用
set(SOURCES
    main.cpp
    test.cpp
    example.c
    sample.c
    ignore_me.cpp
)

# 排除以 "ignore_me" 开头的文件
list_excluding(SOURCES "^ignore_me")

# 打印结果
message(STATUS "Filtered sources: ${SOURCES}")

# By GST @Date