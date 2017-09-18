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

# 编译参数
PARAM_BUILD_MODE="server"

# 控制台输出
_info() {
    printf "[%-5s] %s\n" "${FUNCNAME[1]}" "$1"
}

# 错误信息
_error() {
    printf "[%-5s] Error: %s \n" "${FUNCNAME[1]}" "$1"
    exit
}

# 初始化
init(){
    # 编译参数
    if [ -n "$1" ]; then
        PARAM_BUILD_MODE=$(echo "$1" | tr '[:upper:]' '[:lower:]')
    fi

    ENV_ROOT_DIR=$(cd "$(dirname "$1")" || exit; pwd)
    ENV_BUILD_DIR="$ENV_ROOT_DIR"/build
    ENV_BIN_DIR="$ENV_ROOT_DIR"/bin

    if [[ "$PARAM_BUILD_MODE" != "server" && "$PARAM_BUILD_MODE" != "client" ]]; then
        _error "Build mode must be 'server' or 'client'."
    fi

    _info "PARAM_BUILD_MODE:   $PARAM_BUILD_MODE"
    _info "ENV_ROOT_DIR:       $ENV_ROOT_DIR"
    _info "ENV_BUILD_DIR:      $ENV_BUILD_DIR"
    _info "ENV_BIN_DIR:        $ENV_BIN_DIR"
}

# 清理工作
clean(){
    _info "start..."

    if [[ -d "$ENV_BUILD_DIR" ]]; then
        _info "rm -rf $ENV_BUILD_DIR"
        rm -rf "$ENV_BUILD_DIR"
    fi

    if [[ -f "$ENV_BIN_DIR/$PROJECT_NAME-$PARAM_BUILD_MODE" ]]; then
        _info "rm $ENV_BIN_DIR/$PROJECT_NAME-$PARAM_BUILD_MODE"
        rm "$ENV_BIN_DIR/$PROJECT_NAME-$PARAM_BUILD_MODE"
    fi

    _info "end..."
}

# 成生编译目录
mdir(){
    _info "start..."

    if [[ ! -d "$ENV_BUILD_DIR" ]]; then
        _info "mkdir $ENV_BUILD_DIR"
        mkdir "$ENV_BUILD_DIR"
    fi

    if [[ ! -d "$ENV_BIN_DIR" ]]; then
        _info "mkdir $ENV_BIN_DIR"
        mkdir "$ENV_BIN_DIR"
    fi

    _info "end..."
}

# 编译
build() {
    _info "cd $ENV_BUILD_DIR"
    cd "$ENV_BUILD_DIR" || exit

    _info "cmake -D BUILD_MODE=$PARAM_BUILD_MODE .."
    cmake -D BUILD_MODE="$PARAM_BUILD_MODE" ..

    _info "make"
    make

    if [[ "$PARAM_BUILD_MODE" == "server" ]]; then
        if [[ -f "$ENV_BIN_DIR"/"$SERVER_NAME" ]]; then
            strip "$ENV_BIN_DIR"/"$SERVER_NAME"
            chmod +x "$ENV_BIN_DIR"/"$SERVER_NAME"
        fi
    fi

    if [[ "$PARAM_BUILD_MODE" == "client" ]]; then
        if [[ -f "$ENV_BIN_DIR"/"$CLIENT_NAME" ]]; then
            strip "$ENV_BIN_DIR"/"$CLIENT_NAME"
            chmod +x "$ENV_BIN_DIR"/"$CLIENT_NAME"
        fi
    fi

    _info "done..."
}

main(){
    init "$@"
    clean
    mdir
    build
}

main "$@"