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

    echo "ENV_ROOT_DIR:     $ENV_ROOT_DIR"
    echo "ENV_BUILD_DIR:    $ENV_BUILD_DIR"
    echo "ENV_BIN_DIR:      $ENV_BIN_DIR"
}

# 清理工作
cleanup(){
    echo "cleanup start..."
    rm -rf "$ENV_BUILD_DIR"
    rm -rf "$ENV_BIN_DIR"
    echo "cleanup end..."
}

# 成生编译目录
make_dir(){
    echo "mkdir start..."
    mkdir "$ENV_BUILD_DIR"
    mkdir "$ENV_BIN_DIR"
    echo "mkdir end..."
}

# 编译
build() {
    cd "$ENV_BUILD_DIR" || exit
    echo "cd $ENV_BUILD_DIR"

    cmake ..
    echo "cmake .."

    make
    echo "make"

    if [[ -f "$ENV_BIN_DIR"/"$SERVER_NAME" ]]; then
        strip "$ENV_BIN_DIR"/"$SERVER_NAME"
        chmod +x "$ENV_BIN_DIR"/"$SERVER_NAME"
    fi

    if [[ -f "$ENV_BIN_DIR"/"$CLIENT_NAME" ]]; then
        strip "$ENV_BIN_DIR"/"$CLIENT_NAME"
        chmod +x "$ENV_BIN_DIR"/"$CLIENT_NAME"
    fi

    echo "build is done."
}

main(){
    init
    cleanup
    make_dir
    build
}

main "$@"