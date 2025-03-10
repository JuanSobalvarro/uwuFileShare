#include <pybind11/pybind11.h>
#include "../core/network.h"

namespace py = pybind11;

PYBIND11_MODULE(bindings, m)
{
    py::class_<PeerNode>(m, "PeerNode")
        .def(py::init<int>())
        .def("start", &PeerNode::start)
        .def("connectToPeer", &PeerNode::connectToPeer)
        .def("sendToPeer", &PeerNode::sendToPeer);
}