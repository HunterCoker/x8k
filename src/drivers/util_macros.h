#pragma once

#define DISALLOW_COPY_AND_ASSIGN(T)     \
    T(const T&) = delete;               \
    T& operator=(const T&) = delete;

#define UNUSED(var) (void)(var)