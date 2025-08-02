#include <iostream>
#include <functional>

int impl_a(int x) {
    return x * 2;
}

int impl_b(int x) {
    return x + 10;
}

// 接收任意 int(int) 函数
void run(const std::function<int(int)>& func) {
    int result = func(5);
    std::cout << "Result: " << result << std::endl;
}

auto main(const int argc, char **argv) -> int {
    std::function chosen = impl_a;
    if (argc > 2)
        chosen = impl_b;

    run(chosen);
}
