#pragma once

class NonCopyable {
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

class NonMoveable {
public:
    NonMoveable(const NonMoveable&) = delete;
    NonMoveable& operator=(const NonMoveable&) = delete;

protected:
    NonMoveable() = default;
    ~NonMoveable() = default;
};
