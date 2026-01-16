project(tests)

set(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})
 
# 获取到当前目录下的所有文件和目录（以相对路径的方式），保存到 ALL_ITEMS 变量
file(GLOB ALL_ITEMS RELATIVE ${SUBDIRS} ${SUBDIRS}/*)
 
# 遍历所有的文件
foreach(ITEM ${ALL_ITEMS})
    # 判断是否为目录
    if(IS_DIRECTORY ${SUBDIRS}/${ITEM})
        add_subdirectory(${ITEM})
    endif()
endforeach()