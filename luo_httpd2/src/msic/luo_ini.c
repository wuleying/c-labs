//
// Created by luoliang on 16/4/14.
//

#include "luo_ini.h"

/**
 * 加载ini文件 并存为字典对象
 */
luo_dict_t *
luo_ini_load(const char *ini_file_name) {
    // 初始化变量
    FILE *fd;

    char line[INT_BUF_SIZE];
    char section[INT_BUF_SIZE];
    char key[INT_BUF_SIZE];
    char val[INT_BUF_SIZE];
    char tmp[TMP_BUF_SIZE];

    int len;
    int last    = 0;
    int line_no = 0;
    int errer   = 0;

    luo_dict_t *dict;

    // 打开配置文件
    if (NULL == (fd = fopen(ini_file_name, "r"))) {
        fprintf(stderr, "Cannot open %s.\n", ini_file_name);
        return NULL;
    }

    // 创建字典对象
    dict = luo_dict_create(0);

    if (!dict) {
        fclose(fd);
        return NULL;
    }

    // 分配内存
    memset(line, 0, ASCII_LINE_SIZE);
    memset(section, 0, ASCII_LINE_SIZE);
    memset(key, 0, ASCII_LINE_SIZE);
    memset(val, 0, ASCII_LINE_SIZE);

    // 按行处理文件
    while (NULL != fgets(line + last, ASCII_LINE_SIZE - last, fd)) {
        // 行号
        line_no++;

        // 行长度
        len = (int) strlen(line) - 1;

        // 空行
        if (len <= 0) {
            continue;
        }

        if (line[len] != '\n' && !feof(fd)) {
            fprintf(stderr, "Error: Input line too long in %s (%d).\n", ini_file_name, line_no);
            luo_dict_delete(dict);
            fclose(fd);
            return NULL;
        }

        // 去掉行末尾换行符与空格
        while (len >= 0 && ((line[len] == '\n') || luo_isspace(line[len]))) {
            line[len] = 0;
            len--;
        }

        if (len < 0) {
            len = 0;
        }

        if (line[len] == '\\') {
            last = len;
            continue;
        } else {
            last = 0;
        }

        switch (luo_ini_line(line, section, key, val)) {
            case LINE_EMPTY:
            case LINE_COMMENT:
                break;

            case LINE_SECTION:
                errer = luo_dict_set(dict, section, NULL);
                break;

            case LINE_VALUE:
                sprintf(tmp, "%s.%s", section, key);
                errer = luo_dict_set(dict, tmp, val);
                break;

            case LINE_ERROR:
                fprintf(stderr, "Error: Syntax error in %s line:%d [%s].\n", ini_file_name, line_no, line);
                errer++;
                break;

            default:
                break;
        }

        memset(line, 0, ASCII_LINE_SIZE);
        last = 0;
        if (errer < 0) {
            fprintf(stderr, "Error: Memory allocation failure.\n");
            break;
        }
    }

    if (errer) {
        // 删除字典
        luo_dict_delete(dict);
        dict = NULL;
    }

    fclose(fd);

    return dict;
}

/**
 * 按行处理 并返回行状态码
 */
static luo_line_status_t
luo_ini_line(const char *input_line, char *section, char *key, char *val) {
    luo_line_status_t line_status;
    char              *line = NULL;
    size_t            len;

    line = luo_string_dup(input_line);
    len  = luo_string_strip(line);

    line_status = LINE_UNPROCESSED;

    if (len < 1) {
        // 空行
        line_status = LINE_EMPTY;
    } else if ('#' == line[0] || ';' == line[0]) {
        // 注释
        line_status = LINE_COMMENT;
    } else if ('[' == line[0] && ']' == line[len - 1]) {
        // 配置区段
        sscanf(line, "[%[^]]", section);
        luo_string_strip(section);
        luo_string_lower(section, section, len);

        line_status = LINE_SECTION;
    } else if (2 == sscanf(line, "%[^=] = \"%[^\"]\"", key, val) ||
               2 == sscanf(line, "%[^=] = '%[^\']'", key, val)) {
        // 配置项
        luo_string_strip(key);
        luo_string_lower(key, key, len);

        if (!strcmp(val, "\"\"") || !strcmp(val, "''")) {
            val[0] = 0;
        }

        line_status = LINE_VALUE;
    } else if (2 == sscanf(line, "%[^=] = %[^;#]", key, val)) {
        // 配置项
        luo_string_strip(key);
        luo_string_lower(key, key, len);
        luo_string_strip(val);

        line_status = LINE_VALUE;
    } else if (2 == sscanf(line, "%[^=] = %[;#]", key, val) ||
               2 == sscanf(line, "%[^=] %[=]", key, val)) {
        /**
         * 配置项,如:
         * key=
         * key=;
         * key=#
         */
        luo_string_strip(key);
        luo_string_lower(key, key, len);
        val[0] = 0;

        line_status = LINE_VALUE;
    } else {
        // 错误
        line_status = LINE_ERROR;
    }

    free(line);

    return line_status;
}

/**
 * 返回字典对象的section:key对应的字串值
 */
const char *
luo_ini_get_string(luo_dict_t *dict, const char *key) {
    const char *lower_key;
    const char *result;
    char       tmp[INT_BUF_SIZE];

    if (NULL == dict || NULL == key) {
        return NULL;
    }

    lower_key = luo_string_lower(key, tmp, sizeof(tmp));

    result = luo_dict_get(dict, lower_key);

    return result;
}

/**
 * 返回字典对象的section:key对应的整型值
 */
int
luo_ini_get_int(luo_dict_t *dict, const char *key) {
    const char *str;

    str = luo_ini_get_string(dict, key);

    if (INI_INVALID_KEY == str) {
        return 0;
    }

    return (int) strtol(str, NULL, 0);
}

/**
 * 返回字典对象的section:key对应的双精度浮点值
 */
double luo_ini_get_double(luo_dict_t *dict, const char *key) {
    const char *str;

    str = luo_ini_get_string(dict, key);

    if (INI_INVALID_KEY == str) {
        return 0;
    }

    return atof(str);
}

/**
 * 释放
 */
void luo_ini_free(luo_dict_t *dict) {
    luo_dict_delete(dict);
}