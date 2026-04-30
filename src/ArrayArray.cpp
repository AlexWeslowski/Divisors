#include "../include/ArrayArray.h"

using namespace std; 

namespace py = pybind11;

template<ValidIntegerType T, size_t NK, size_t NV>
ArrayArray<T, NK, NV>::ArrayArray() : ArrayArray(2048, true) {
    if (Globals::verbose) std::cout << "ArrayArray()" << std::endl;
}

template<ValidIntegerType T, size_t NK, size_t NV>
ArrayArray<T, NK, NV>::ArrayArray(size_t capacity, bool resizeable)
        : size_(0),
        keys_capacity_(capacity),
        values_capacity_(2 * capacity),
        keys_index_(0),
        values_index_(0),
        resizeable_(resizeable),
		large_keys(capacity / 2),
		large_values(capacity) {
	if (Globals::verbose) std::cout << "ArrayArray(), line " << __LINE__ << ", capacity = " << capacity << ", resizeable = " << resizeable << std::endl;
	keys.resize(capacity, -1);
    values.resize(2 * capacity, 0);
    if (Globals::verbose) std::cout << "ArrayArray(), line " << __LINE__ << ", capacity = " << capacity << ", resizeable = " << resizeable << std::endl;
    }

template<ValidIntegerType T, size_t NK, size_t NV>
//template<size_t N>
template<VectorLike<T> V>
//void ArrayArray<T, NK, NV>::push_back(const vec_push<T, N>& ary) {
void ArrayArray<T, NK, NV>::push_back(const V& ary) {
    append(ary);
}
 
/* 
template<ValidIntegerType T, size_t NK, size_t NV>
void ArrayArray<T, NK, NV>::push_back(const std::vector<T>& ary) {
    append(ary);
}
*/

template<ValidIntegerType T, size_t NK, size_t NV>
template<size_t N>
void ArrayArray<T, NK, NV>::push_back_small_vector(const boost::container::small_vector<T, N>& ary) {
    append_small_vector<N>(ary);
}

template<ValidIntegerType T, size_t NK, size_t NV>
//template<size_t N>
template<VectorLike<T> V>
//void ArrayArray<T, NK, NV>::append(const vec_push<T, N>& ary) {
void ArrayArray<T, NK, NV>::append(const V& ary) {
    if (resizeable_ && values_index_ + ary.size() >= values_capacity_) {
        values_capacity_ += 2048;
		try {
			if (Globals::verbose) {
				std::cout << "append(), values_index_ = " << values_capacity_ << std::endl;
				std::cout << "append(), large_values.resize(" << (values_capacity_ - values.capacity()) << ")" << std::endl;
				std::cout << "append(), values_index_ = " << values_index_ << std::endl;
				std::cout << "append(), values.size() = " << values.size() << std::endl;
				std::cout << "append(), values.capacity() = " << values.capacity() << std::endl;
				std::cout << "append(), large_values.size() = " << large_values.size() << std::endl;
				std::cout << "append(), large_values.capacity() = " << large_values.capacity() << std::endl;
			}
			/*
			if (values_capacity_ > values.capacity()) {
				if (values_capacity_ - values.capacity() > large_values.size()) {
					large_values.resize(values_capacity_ - values.capacity(), 0);
				}
			} else {
				values.resize(values_capacity_, 0);
			}
			*/
			values.resize(values_capacity_, 0);
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
		if (values_index_ > values.size()) {
			large_values[values_index_++ - values.size()] = a;
		} else {
			values[values_index_++] = a;
		}
    }

    if (resizeable_ && keys_index_ + 2 >= keys_capacity_) {
        keys_capacity_ += 2048;
		try {
			if (Globals::verbose) {
				std::cout << "append(), keys.resize(" << keys_capacity_ << ")" << std::endl;
				std::cout << "append(), keys.capacity() = " << keys.capacity() << std::endl;
			}
			/*
			if (keys_capacity_ > keys.capacity()) {
				if (keys_capacity_ - keys.capacity() > large_keys.size()) {
					large_keys.resize(keys_capacity_ - keys.capacity(), -1);
				}
			} else {
				keys.resize(keys_capacity_, -1);
			}
			*/
			keys.resize(keys_capacity_, -1);
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
	if (Globals::verbose) {
		std::cout << "append(), keys[420] = " << keys[420] << std::endl;
		std::cout << "append(), keys[421] = " << keys[421] << std::endl;
		std::cout << "append(), keys_index_ = " << keys_index_ << std::endl;
		std::cout << "append(), values_index_ = " << values_index_ << std::endl;
		std::cout << "append(), keys.size() = " << keys.size() << std::endl;
		std::cout << "append(), keys.capacity() = " << keys.capacity() << std::endl;
		std::cout << "append(), ary.size() = " << ary.size() << std::endl;
		std::cout << "append(), keys[" << keys_index_ << "] = " << (values_index_ - ary.size()) << std::endl;
		std::cout << "append(), keys[" << (keys_index_ + 1) << "] = " << (values_index_ - 1) << std::endl;
	}
	if (keys_index_ > keys.size()) {
		large_keys[keys_index_ - keys.size()] = values_index_ - ary.size();
		large_keys[keys_index_ + 1 - keys.size()] = values_index_ - 1;
	} else {
		keys[keys_index_] = values_index_ - ary.size();
		keys[keys_index_ + 1] = values_index_ - 1;
	}
	if (Globals::verbose) {		
		if (keys_index_ > 420 && keys[420] == 0) {
			std::cout << "append(), keys[420] = " << keys[420] << std::endl;
			std::cout << "append(), keys[421] = " << keys[421] << std::endl;
			std::exit(0);
		}
	}

    keys_index_ += 2;
    size_++;
}

/*
template<ValidIntegerType T, size_t NK, size_t NV>
void ArrayArray<T, NK, NV>::append(const std::vector<T>& ary) {
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
        
    keys[keys_index_] = values_index_ - ary.size();
    keys[keys_index_ + 1] = values_index_ - 1;
    keys_index_ += 2;
    size_++;
}
*/

template<ValidIntegerType T, size_t NK, size_t NV>
template<size_t N>
void ArrayArray<T, NK, NV>::append_small_vector(const boost::container::small_vector<T, N>& ary) {
    if (resizeable_ && values_index_ + ary.size() >= values_capacity_) {
        values_capacity_ += 2048;
		try {
			if (Globals::verbose) std::cout << "append_small_vector(), values.resize(" << values_capacity_ << ")" << std::endl;
			if (values_capacity_ > values.capacity()) {
				if (values_capacity_ - values.capacity() > large_values.size()) {
					large_values.resize(values_capacity_ - values.capacity());
				}
			} else {
				values.resize(values_capacity_);
			}
		} catch (const std::bad_alloc& e) {
			std::cout << "Caught std::bad_alloc during values.resize()\n";
			std::cout << "Error details: " << e.what() << "\n";
		} catch (const std::exception& e) {
			std::cout << "Caught other standard exception: " << e.what() << "\n";
		}
    }

    for (const auto& a : ary) {
        if (values_index_ > values.size()) {
			large_values[values_index_++ - values.size()] = a;
		} else {
			values[values_index_++] = a;
		}
    }

    if (resizeable_ && keys_index_ + 2 >= keys_capacity_) {
        keys_capacity_ += 2048;
		try {
			if (Globals::verbose) std::cout << "append_small_vector(), keys.resize(" << keys_capacity_ << ")" << std::endl;
			if (keys_capacity_ > keys.capacity()) {
				large_keys.resize(keys_capacity_ - keys.capacity());
			} else {
				keys.resize(keys_capacity_);
			}
		} catch (const std::bad_alloc& e) {
			std::cout << "Caught std::bad_alloc during keys.resize()\n";
			std::cout << "Error details: " << e.what() << "\n";
		} catch (const std::exception& e) {
			std::cout << "Caught other standard exception: " << e.what() << "\n";
		}
    }
    
	if (Globals::verbose) {
		std::cout << "append_small_vector(), keys[420] = " << keys[420] << std::endl;
		std::cout << "append_small_vector(), keys[421] = " << keys[421] << std::endl;
		std::cout << "append_small_vector(), ary.size() = " << ary.size() << std::endl;
		std::cout << "append_small_vector(), keys[" << keys_index_ << "] = " << (values_index_ - ary.size()) << std::endl;
		std::cout << "append_small_vector(), keys[" << (keys_index_ + 1) << "] = " << (values_index_ - 1) << std::endl;
	}
    keys[keys_index_] = values_index_ - ary.size();
    keys[keys_index_ + 1] = values_index_ - 1;
	if (Globals::verbose) {		
		if (keys_index_ > 420 && keys[420] == 0) {
			std::cout << "append_small_vector(), keys[420] = " << keys[420] << std::endl;
			std::cout << "append_small_vector(), keys[421] = " << keys[421] << std::endl;
			std::exit(0);
		}
	}
    keys_index_ += 2;
    size_++;
}

template<ValidIntegerType T, size_t NK, size_t NV>
void ArrayArray<T, NK, NV>::remove_at(size_t idx) {
    removeAt(idx);
}

template<ValidIntegerType T, size_t NK, size_t NV>
void ArrayArray<T, NK, NV>::removeAt(size_t idx) {
    if (2 * idx + 1 >= keys_index_) {
        return;
    }
    keys[2 * idx] = -1;
    keys[2 * idx + 1] = -1;
    size_--;
}

template<ValidIntegerType T, size_t NK, size_t NV>
size_t ArrayArray<T, NK, NV>::size() const {
    return keys_index_ / 2;
}

template<ValidIntegerType T, size_t NK, size_t NV>
size_t ArrayArray<T, NK, NV>::len() const {
    return keys_index_ / 2;
}

template<ValidIntegerType T, size_t NK, size_t NV>
size_t ArrayArray<T, NK, NV>::keys_size() const {
    return keys_index_;
}

template<ValidIntegerType T, size_t NK, size_t NV>
size_t ArrayArray<T, NK, NV>::keys_len() const {
    return keys_index_;
}

template<ValidIntegerType T, size_t NK, size_t NV>
size_t ArrayArray<T, NK, NV>::keys_capacity() const {
    return keys.capacity();
}

template<ValidIntegerType T, size_t NK, size_t NV>
size_t ArrayArray<T, NK, NV>::values_size() const {
    return values_index_;
}

template<ValidIntegerType T, size_t NK, size_t NV>
size_t ArrayArray<T, NK, NV>::values_len() const {
    return values_index_;
}

template<ValidIntegerType T, size_t NK, size_t NV>
size_t ArrayArray<T, NK, NV>::values_capacity() const {
    return values.capacity();
}

template<ValidIntegerType T, size_t NK, size_t NV>
template<VectorLike<T> V>
V ArrayArray<T, NK, NV>::get(size_t idx) const {
    size_t key_idx = 2 * idx;
    if (Globals::verbose) {
		std::cout << "ArrayArray::get() size_ = " << size_ << ", keys_index_ = " << keys_index_ << ", values_index_ = " << values_index_ << std::endl;
		std::cout << "ArrayArray::get() key_idx = " << key_idx << ", idx = " << idx << ", keys[" << key_idx << "] = " << keys[key_idx] << ", keys[" << (key_idx + 1) << "] = " << keys[key_idx + 1] << std::endl;
		std::cout << "ArrayArray::get() key_idx + 1 >= keys_index_ ? " << (key_idx + 1 >= keys_index_) << ", keys[key_idx] == -1 ? " << (keys[key_idx] == -1) << ", keys[key_idx + 1] == -1 ? " << (keys[key_idx + 1] == -1) << std::endl;
	}
    if (key_idx >= keys_index_ || key_idx + 1 >= keys_index_ || keys[key_idx] == -1 || keys[key_idx + 1] == -1) {
        return {}; 
    }
    /*
    if (i >= values.size() || j >= values.size()) {
        throw std::out_of_range("Invalid indices in keys vector.");
    }
    */
	if (Globals::verbose) std::cout << "ArrayArray::get() returning " << to_string(std::vector(values.begin() + keys[key_idx], values.begin() + keys[key_idx + 1] + 1)) << std::endl;
	V vec;
	if (key_idx > keys.size()) {
		vec.reserve(large_keys[key_idx + 1 - keys.size()] + 1 - large_keys[key_idx - keys.size()]);
		vec.assign(values.begin() + large_keys[key_idx - keys.size()], values.begin() + large_keys[key_idx + 1 - keys.size()] + 1);
	} else {
		vec.reserve(keys[key_idx + 1] + 1 - keys[key_idx]);
		vec.assign(values.begin() + keys[key_idx], values.begin() + keys[key_idx + 1] + 1);
	}
	return vec;
}

template<ValidIntegerType T, size_t NK, size_t NV>
vec384<vec24<T>> ArrayArray<T, NK, NV>::to_list() const {
    return to_array();
}

template<ValidIntegerType T, size_t NK, size_t NV>
vec384<vec24<T>> ArrayArray<T, NK, NV>::to_array() const {
    if (Globals::verbose) std::cout << "ArrayArray::to_array() size_ = " << size_ << ", keys_index_ = " << keys_index_ << ", values_index_ = " << values_index_ << ")" << std::endl;
    vec384<vec24<T>> result;
	result.reserve(keys_index_ / 2);
    for (size_t i = 0; i < keys_index_; i += 2) {
        if (keys[i] != -1) {
            result.push_back(get<vec24<T>>(i / 2));
        }
    }
    return result;
}


PYBIND11_MODULE(arrayarray, m) {
    m.doc() = "arrayarray made with pybind11";

#include <../src/ArrayArrayPy.cpp>

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}