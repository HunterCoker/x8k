#pragma once

#define DISALLOW_COPY_ASSIGN(Type)          \
    Type(const Type&) = delete;             \
    void operator=(const Type&) = delete;