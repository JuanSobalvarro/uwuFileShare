#include <pybind11/pybind11.h>

namespace py = pybind11;

void say_hello() {
    printf("Hello, World!\n");
}

PYBIND11_MODULE(hello, m) {
    m.def("say_hello", &say_hello, "Test function to say hello from c++");
}
