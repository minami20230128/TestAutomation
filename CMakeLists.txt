cmake_minimum_required(VERSION 3.5.1) #cmakeの最小バージョンを決める
project(main) #プロジェクト名
add_executable(main main.cpp excel.hpp pipe.hpp) #ターゲットを決める
find_package(OpenXLSX REQUIRED) #OpenXLSXというライブラリを探す(詳細は下記)
target_include_directories(main PUBLIC ${OpenXLSX_INCLUDE_DIR}) #インクルードディレクトリ(上記)にあるヘッダファイルをインクルード
link_directories(${OpenXLSX_LIBRARY_DIR}) #ライブラリディレクトリ(上記)にある静的ライブラリをインクルード
target_link_libraries(main PUBLIC /usr/local/lib/libOpenXLSX.a) #動的ライブラリディレクトリ(上記)にある動的ライブラリをインクルード
target_compile_features(main PUBLIC cxx_std_17) #c++のバージョンを決める(17行目にもあるが、c++11以降を利用するときは改めて決める必要がある)
message(STATUS "INCLUDE_DIR="${OpenXLSX_INCLUDE_DIR}) #ライブラリのディレクトリを出力し、見つけられているか確認
message(STATUS "LIBRARY_DIR="${OpenXLSX_LIBRARY_DIR})
message(STATUS "LIBS="${OpenXLSX_LIBRARIES})
