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
		try {
			values.resize(values_capacity_);
		} catch (const std::bad_alloc& e) {
			std::cout << "Caught std::bad_alloc during values.resize()\n";
			std::cout << "Error details: " << e.what() << "\n";
		} catch (const std::exception& e) {
			std::cout << "Caught other standard exception: " << e.what() << "\n";
		}
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
		try {
			keys.resize(keys_capacity_);
		} catch (const std::bad_alloc& e) {
			std::cout << "Caught std::bad_alloc during keys.resize()\n";
			std::cout << "Error details: " << e.what() << "\n";
		} catch (const std::exception& e) {
			std::cout << "Caught other standard exception: " << e.what() << "\n";
		}
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
void ArrayArray<T>::remove_at(size_t idx) {
    removeAt(idx);
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
size_t ArrayArray<T>::size() const {
    return keys_index_ / 2;
}

template<ValidIntegerType T>
size_t ArrayArray<T>::len() const {
    return keys_index_ / 2;
}

template<ValidIntegerType T>
size_t ArrayArray<T>::values_size() const {
    return values_index_;
}

template<ValidIntegerType T>
size_t ArrayArray<T>::values_len() const {
    return values_index_;
}

template<ValidIntegerType T>
std::string ArrayArray<T>::to_string(const std::vector<int64_t>& vec) const {
    if (vec.empty()) {
        return "[]";
    }
    std::stringstream ss;
    ss << "[";
    std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<int64_t>(ss, ", "));
    ss << vec.back();
    ss << "]";
    return ss.str();
}

template<ValidIntegerType T>
std::vector<T> ArrayArray<T>::get(size_t idx) const {
    size_t key_idx = 2 * idx;
    if (Globals::verbose) std::cout << "ArrayArray::get() size_ = " << size_ << ", keys_index_ = " << keys_index_ << ", values_index_ = " << values_index_ << std::endl;
    if (Globals::verbose) std::cout << "ArrayArray::get() key_idx = " << key_idx << ", idx = " << idx << ", keys[" << key_idx << "] = " << keys[key_idx] << ", keys[" << (key_idx + 1) << "] = " << keys[key_idx + 1] << std::endl;
	if (Globals::verbose) std::cout << "ArrayArray::get() key_idx + 1 >= keys_index_ ? " << (key_idx + 1 >= keys_index_) << ", keys[key_idx] == -1 ? " << (keys[key_idx] == -1) << ", keys[key_idx + 1] == -1 ? " << (keys[key_idx + 1] == -1) << std::endl;
    if (key_idx >= keys_index_ || key_idx + 1 >= keys_index_ || keys[key_idx] == -1 || keys[key_idx + 1] == -1) {
        return {}; 
    }
    /*
    if (i >= values.size() || j >= values.size()) {
        throw std::out_of_range("Invalid indices in keys vector.");
    }
    */
	if (Globals::verbose) std::cout << "ArrayArray::get() returning " << to_string(std::vector<int64_t>(values.begin() + keys[key_idx], values.begin() + keys[key_idx + 1] + 1)) << std::endl;
    return std::vector<int64_t>(values.begin() + keys[key_idx], values.begin() + keys[key_idx + 1] + 1);
}

template<ValidIntegerType T>
std::vector<std::vector<T>> ArrayArray<T>::to_list() const {
    return to_array();
}
    
template<ValidIntegerType T>
std::vector<std::vector<T>> ArrayArray<T>::to_array() const {
    if (Globals::verbose) std::cout << "ArrayArray::to_array() size_ = " << size_ << ", keys_index_ = " << keys_index_ << ", values_index_ = " << values_index_ << ")" << std::endl;
    std::vector<std::vector<T>> result;
    for (size_t i = 0; i < keys_index_; i += 2) {
        if (keys[i] != -1) {
            result.push_back(get(i / 2));
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

#include <../src/ArrayArrayPy.cpp>

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}