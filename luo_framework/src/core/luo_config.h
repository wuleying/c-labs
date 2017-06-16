//
// Created by luoliang on 16/6/2.
//

#ifndef LUO_FRAMEWORK_LUO_CONFIG_H
#define LUO_FRAMEWORK_LUO_CONFIG_H

#define luo_cdecl
#define luo_libc_cdecl

#define luo_align(d, a)     (((d) + (a - 1)) & ~(a - 1))

#endif //LUO_FRAMEWORK_LUO_CONFIG_H
