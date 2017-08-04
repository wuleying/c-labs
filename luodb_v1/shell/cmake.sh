#!/usr/bin/env bash
# 项目名称
PROJECT_NAME="luodb"
# 服务端名称
SERVER_NAME="$PROJECT_NAME"-server
# 客户端名称
CLIENT_NAME="$PROJECT_NAME"-client

# 根目录
ENV_ROOT_DIR=""
# build目录
ENV_BUILD_DIR=""
# bin目录
ENV_BIN_DIR=""

# 初始化
init(){
    ENV_ROOT_DIR=$(cd "$(dirname "$1")" || exit; pwd)
    ENV_BUILD_DIR="$ENV_ROOT_DIR"/build
    ENV_BIN_DIR="$ENV_ROOT_DIR"/bin

     _print "ENV_ROOT_DIR:     $ENV_ROOT_DIR"
     _print "ENV_BUILD_DIR:    $ENV_BUILD_DIR"
     _print "ENV_BIN_DIR:      $ENV_BIN_DIR"
}

# 清理工作
clean(){
    _print "start..."
    rm -rf "$ENV_BUILD_DIR"
    rm -rf "$ENV_BIN_DIR"
    _print "end..."
}

# 成生编译目录
mdir(){
    _print "start..."
    mkdir "$ENV_BUILD_DIR"
    mkdir "$ENV_BIN_DIR"
    _print "end..."
}

# 编译
build() {
    cd "$ENV_BUILD_DIR" || exit
    _print "cd $ENV_BUILD_DIR"

    cmake ..
    _print "cmake .."

    make
    _print "make"

    if [[ -f "$ENV_BIN_DIR"/"$SERVER_NAME" ]]; then
        strip "$ENV_BIN_DIR"/"$SERVER_NAME"
        chmod +x "$ENV_BIN_DIR"/"$SERVER_NAME"
    fi

    if [[ -f "$ENV_BIN_DIR"/"$CLIENT_NAME" ]]; then
        strip "$ENV_BIN_DIR"/"$CLIENT_NAME"
        chmod +x "$ENV_BIN_DIR"/"$CLIENT_NAME"
    fi

    _print "build is done."
}

_print() {
    printf "[%-5s] %s\n" "${FUNCNAME[1]}" "$1"
}

main(){
    init "$@"
    clean
    mdir
    build
}

main "$@"