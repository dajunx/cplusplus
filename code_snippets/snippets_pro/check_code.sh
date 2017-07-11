echo "check code snippet in linux"

# 创建临时check 目录:build
#---------------------begin--------------------------
if [ -d "build" ]; then
	echo "build exist"
else
	echo "build not exist, mkdir it."
	mkdir build
fi

# 在build 目录中 归档cpp文件
cd ./build
rm * -rf
find ../ -name "*.cpp" -exec ln -n {} \;
find ../ -name "*.c" -exec ln -n {} \;
#---------------------end--------------------------

# 特殊 只能在win端编译的cpp文件
rm thread_semaphore.cpp
rm cplusplus_time_test.cpp
rm Inter_operate_shareddata.cpp
rm win_calculate_costs_time.cpp

# 
rm coroutine_test.cpp
rm set_container_random.cpp
rm async_tcp_service1.cpp
rm array_copy_snippet.cpp
rm io_service_run_inline_thread.cpp
rm a_named_pipe.cpp

# 编译不过
rm multi_index_add_up_view.cpp
rm string_algo_snippet.cpp
rm prod_no_condvar.c
rm non_boost_function_bind_usage.cpp
rm boost_multi_index_test.cpp
rm mysql_multi_thread_conn_test.cpp
rm wrap_usage.cpp
rm simple_thread.c
rm thread_multijoin.c
rm inputdata_and_localdata_threasafe.c

# 复合代码
rm bind_life_cycle_example.cpp
rm class_inline_class.cpp

# 创建 cmake 项目文件
touch CMakeLists.txt

echo "# CMake 最低版本号要求" >> CMakeLists.txt
echo "cmake_minimum_required (VERSION 2.8)" >> CMakeLists.txt
echo "# 项目信息" >> CMakeLists.txt
echo "project (Demo2)" >> CMakeLists.txt
echo "# 查找当前目录下的所有源文件" >> CMakeLists.txt
echo "# 并将名称保存到 DIR_SRCS 变量" >> CMakeLists.txt
echo "aux_source_directory(. DIR_SRCS)" >> CMakeLists.txt
echo "find_package(Boost 1.53.0)" >> CMakeLists.txt
echo "set(Boost_INCLUDE_DIRS \"/home/kaka/build_third_lib/boost_1_53_0\")" >> CMakeLists.txt
echo "" >> CMakeLists.txt
echo "if(Boost_FOUND)" >> CMakeLists.txt
echo "    include_directories(\${Boost_INCLUDE_DIRS})" >> CMakeLists.txt
# echo "    add_executable(progname file1.cxx file2.cxx)" >> CMakeLists.txt 
# echo "    target_link_libraries(progname \${Boost_LIBRARIES})" >> CMakeLists.txt
# echo "    message(\${Boost_INCLUDE_DIRS} \" \" \${Boost_FOUND})" >> CMakeLists.txt
echo "endif(Boost_FOUND)" >> CMakeLists.txt
echo "" >> CMakeLists.txt
echo "include_directories(\"/home/kaka/include_kaka\")" >> CMakeLists.txt
echo "# 指定生成目标" >> CMakeLists.txt
echo "add_executable(Demo \${DIR_SRCS})" >> CMakeLists.txt

# 生成编译脚本
touch Build.sh
chmod +x Build.sh
echo "cmake . ; make" >> Build.sh
./Build.sh
