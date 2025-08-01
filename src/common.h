#pragma once

#define NULLPTR nullptr
#define NOEXCEPT noexcept
#define OVERRIDE override
#define EXPORT export

// 同时禁止复制和赋值
#define NO_COPY_ASSIGN(ClassName)                                                                                      \
    ClassName(const ClassName&);            /*複製構造函數*/                                                           \
    ClassName& operator=(const ClassName&); /*賦值構造函數*/
