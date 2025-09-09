#include "../include/ArrayArray.h"
using namespace std; 

namespace py = pybind11;

template<ValidIntegerType T>
ArrayArray<T>::ArrayArray()
    : ArrayArray(2048, true) {
    if (Globals::verbose) std::cout << "ArrayArray()" << std::endl;
}

template<ValidIntegerType T>
ArrayArray<T>::ArrayArray(size_t capacity, bool resizeable)
        : size_(0),
        keys_capacity_(capacity),
        values_capacity_(2 * capacity),
        keys_index_(0),
        values_index_(0),
        resizeable_(resizeable),
        keys(capacity, 0),
        values(2 * capacity, 0) {
    if (Globals::verbose) std::cout << "ArrayArray(" << capacity << ", " << resizeable << ")" << std::endl;
    }

template<ValidIntegerType T>
void ArrayArray<T>::push_back(const std::vector<T>& ary) {
    append(ary);
}
    
template<ValidIntegerType T>
void ArrayArray<T>::append(const std::vector<T>& ary) {
    if (resizeable_ && values_index_ + ary.size() >= values_capacity_) {
        values_capacity_ += 2048;
        values.resize(values_capacity_);
    }

    for (const auto& a : ary) {
        /*
        if (values_index_ >= values.size()) {
            throw std::out_of_range("Values vector overflow during append.");
        }
        */
        values[values_index_++] = a;
    }

    if (resizeable_ && keys_index_ + 2 >= keys_capacity_) {
        keys_capacity_ += 2048;
        keys.resize(keys_capacity_);
    }
        
    /*
    if (keys_index_ + 1 >= keys.size()) {
        throw std::out_of_range("Keys vector overflow during append.");
    }
    */
    keys[keys_index_] = values_index_ - ary.size();
    keys[keys_index_ + 1] = values_index_ - 1;
    keys_index_ += 2;
    size_++;
}

template<ValidIntegerType T>
void ArrayArray<T>::removeAt(size_t idx) {
        if (2 * idx + 1 >= keys_index_) {
            return;
        }
        keys[2 * idx] = -1;
        keys[2 * idx + 1] = -1;
        size_--;
    }

template<ValidIntegerType T>
size_t ArrayArray<T>::getSize() const {
    return keys_index_ / 2;
}

template<ValidIntegerType T>
std::vector<T> ArrayArray<T>::getItem(size_t idx) const {
    size_t key_idx = 2 * idx;
    if (key_idx >= keys_index_ || key_idx + 1 >= keys_index_ || keys[key_idx] == -1) {
        return {}; // Return empty vector
    }

    size_t i = keys[key_idx];
    size_t j = keys[key_idx + 1];

    /*
    if (i >= values.size() || j >= values.size()) {
        throw std::out_of_range("Invalid indices in keys vector.");
    }
    */

    return std::vector<int64_t>(values.begin() + i, values.begin() + j + 1);
}

template<ValidIntegerType T>
std::vector<std::vector<T>> ArrayArray<T>::to_array() const {
    if (Globals::verbose) std::cout << "ArrayArray::to_array() size_ = " << size_ << ", keys_index_ = " << keys_index_ << ", values_index_ = " << values_index_ << ")" << std::endl;
    std::vector<std::vector<T>> result;
    for (size_t i = 0; i < keys_index_; i += 2) {
        if (keys[i] != -1) {
            result.push_back(getItem(i / 2));
        }
    }
    return result;
}


PYBIND11_MODULE(arrayarray, m) {
    m.doc() = "arrayarray made with pybind11";

    /*
    using ArrayArrayT = ArrayArray<int64_t>;
    std::string type_name = "";
    py::class_<ArrayArrayT>(m, ("ArrayArray" + type_name).c_str())
        .def(py::init<size_t, bool>(), py::arg("capacity"), py::arg("resizeable"))
        .def("append", &ArrayArrayT::append, py::arg("ary"))
        .def("removeAt", &ArrayArrayT::removeAt, py::arg("idx"))
        .def("__len__", &ArrayArrayT::getSize)
        .def("__getitem__", &ArrayArrayT::getItem, py::arg("idx"))
        .def("to_array", &ArrayArrayT::to_array)
        .def_readwrite("keys", &ArrayArrayT::keys)
        .def_readwrite("values", &ArrayArrayT::values);
    */

    py::class_<ArrayArray<int64_t>, std::shared_ptr<ArrayArray<int64_t>>>(m, "ArrayArray")
        .def(py::init())
        .def(py::init<size_t, bool>(), py::arg("capacity"), py::arg("resizeable"))
        .def("append", &ArrayArray<int64_t>::append)
        .def("push_back", &ArrayArray<int64_t>::push_back)
        .def("removeAt", &ArrayArray<int64_t>::removeAt)
        .def("to_array", &ArrayArray<int64_t>::to_array)
        //.def("get_size", &ArrayArray<int64_t>::getSize)
        //.def("get_item", &ArrayArray<int64_t>::getItem)
        .def("__len__", &ArrayArray<int64_t>::getSize)
        .def("__getitem__", &ArrayArray<int64_t>::getItem);


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}