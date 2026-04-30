#include <../include/Combinations.h>
#include <../include/CombinationsIterator.h>
using namespace std;

namespace py = pybind11;

template<ValidIntegerType T>
Combinations<T>::Combinations(T n) {
    //none = ArrayArray<T>(1, false);
    //cmbinations = ArrayArray<T>(2048, true);
    this->n = n;
    //none = std::make_shared<ArrayArray<T, 1, 1>>(0, false);
    aryary = std::make_shared<ArrayArray<T, COMBINATIONS_KEYS_LEN, COMBINATIONS_VALUES_LEN>>(COMBINATIONS_KEYS_LEN, true);
}

template<ValidIntegerType T>
size_t Combinations<T>::size() const {
    return (aryary) ? aryary->size() : 0;
}

template<ValidIntegerType T>
size_t Combinations<T>::len() const {
    return (aryary) ? aryary->len() : 0;
}

template<ValidIntegerType T>
std::pair<size_t, size_t> Combinations<T>::aryary_size() const {
    if (aryary) {
		return { aryary->keys_size(), aryary->values_size() };
	} else {
		return { 0, 0 };
	}
}

template<ValidIntegerType T>
std::pair<size_t, size_t> Combinations<T>::aryary_len() const {
    if (aryary) {
		return { aryary->keys_size(), aryary->values_size() };
	} else {
		return { 0, 0 };
	}
}

template<ValidIntegerType T>
std::pair<size_t, size_t> Combinations<T>::aryary_capacity() const {
    if (aryary) {
		return { aryary->keys_capacity(), aryary->values_capacity() };
	} else {
		return { 0, 0 };
	}
}

template <ValidIntegerType T>
vec24<T> Combinations<T>::next() {
	if (get_verbose()) std::cout << "Combinations::next() index = " << index << ", aryary ? " << ((aryary) ? true : false) << ", aryary->size() = " << ((aryary) ? aryary->size() : 0) << std::endl;
    if (!aryary || index >= aryary->size()) {
        index = 0;
		if (get_verbose()) std::cout << "Combinations::next() stop_iteration()" << std::endl;
        throw pybind11::stop_iteration();
    }
    vec24<T> vec = aryary->get<vec24<T>>(index++);
    bool iter = (vec.size() == 0);
	while (vec.size() == 0 && index < aryary->size()) {
		vec = aryary->get<vec24<T>>(index++);
	}
	if (iter && vec.size() == 0 && index >= aryary->size()) {
        index = 0;
		if (get_verbose()) {
			std::cout << "Combinations::next() iter = " << iter << ", vec.size() = " << vec.size() << std::endl;
			std::cout << "Combinations::next() stop_iteration()" << std::endl;
		}
		throw pybind11::stop_iteration();
	}
    return vec;
}

template<ValidIntegerType T>
vec24<T> Combinations<T>::get(size_t idx) const {
    return (aryary) ? aryary->get<vec24<T>>(idx) : vec24<T>();
}

template<ValidIntegerType T>
void Combinations<T>::remove_at(size_t idx) {
    if (aryary) {
        aryary->remove_at(idx);
    }
}

template<ValidIntegerType T>
void Combinations<T>::removeAt(size_t idx) {
    if (aryary) {
        aryary->removeAt(idx);
    }
}

template<ValidIntegerType T>
vec384<vec24<T>> Combinations<T>::to_array() const {
    return (aryary) ? aryary->to_array() : vec384<vec24<T>>();
}


template<ValidIntegerType T>
vec384<vec24<T>> Combinations<T>::to_list() const {
    return (aryary) ? aryary->to_list() : vec384<vec24<T>>();
}

template<ValidIntegerType T>
std::shared_ptr<ArrayArray<T, COMBINATIONS_KEYS_LEN, COMBINATIONS_VALUES_LEN>> Combinations<T>::get_arrayarray() const {
    return aryary;
}

template<ValidIntegerType T>
bool Combinations<T>::get_verbose() const {
    return (verbose.has_value() && verbose.value()) || (!verbose.has_value() && Globals::verbose);
}

template<ValidIntegerType T>
void Combinations<T>::set_verbose(bool bln) {
    //Globals::verbose = bln;
	verbose = bln;
}

template<ValidIntegerType T>
void Combinations<T>::backtrack(T target) {
	vec_factors<T> factors;
	if (target == 1145760) {
		write_to_file = true;
	} else {
		write_to_file = false;
	}
	if (write_to_file) {
		file_stream.open("combinations.txt");
	}
	_backtrack(0, 1, target, factors);
	if (write_to_file) {
		file_stream.close();
	}
}

template<ValidIntegerType T>
bool Combinations<T>::is_divisible(vec_factors<T>& factors, size_t factors_len, T fact) {
	if (factors_len == 0) {
		return false;
	}
	//bool bprint = (factors_len >= 4 && factors[0] == 3 && factors[1] == 7 && factors[2] == 11 && factors[3] == 32); 
	for (size_t i = 0; i < factors_len; i++) {
		if (factors[i] > fact) {
			//if (bprint) std::cout << "is_divisible() factors_len = " << factors_len << ", factors[" << i << "], " << factors[i] << " % " << fact << " = " << (factors[i] % fact) << std::endl;
			if (factors[i] % fact == 0) {
				return true;
			}
		} else {
			//if (bprint) std::cout << "is_divisible() factors_len = " << factors_len << ", factors[" << i << "], " << fact << " % " << factors[i] << " = " << (fact % factors[i]) << std::endl;
			if (fact % factors[i] == 0) {
				return true;
			}
		}
	}
	return false;
}


/*

import divisors as div
div.set_verbose(False)
combinations = div.Combinations(1145760)
combinations.backtrack(1145760)
[3, 7, 11, 32, 155] in combinations

https://github.com/AlexWeslowski/Divisors/blob/main/src/Combinations.cpp#L95

*/
template<ValidIntegerType T>
void Combinations<T>::_backtrack(size_t idepth, T istart, T target, vec_factors<T>& factors) {
    //if (factors.size() >= 2 && factors[0] == 6 && factors[1] == 8) std::cout << "_backtrack(" << target << ", " << to_string<T, FACTORS_VEC_LEN>(factors) << ")" << std::endl;
	if (get_verbose()) {
		std::cout << "_backtrack(" << target << ", " << to_string<T, FACTORS_VEC_LEN>(factors) << ")" << std::endl;
		std::cout << "_backtrack(), factors.capacity() = " << factors.capacity() << ", factors.size() = " << factors.size() << std::endl;
	}
	
	idepth++;
	
	/*
	833,280		[3, 7, 10, 62, 64]
	954,240		[3, 7, 10, 64, 71]
	1,145,760	[3, 7, 11, 32, 155]
	1,317,120	[3, 5, 32, 49, 56]
	1,666,560	[3, 5, 31, 56, 64]
	1,908,480	[3, 5, 28, 64, 71]
	*/
	/*
	bool is_match;
	T fact0;
	T fact1;
	T fact2;
	switch (this->n) {
		case 833280:
			is_match = true;
			fact0 = 3;
			fact1 = 7;
			fact2 = 10;
			break;
		case 954240:
			is_match = true;
			fact0 = 3;
			fact1 = 7;
			fact2 = 10;
			break;
		case 1145760:
			is_match = true;
			fact0 = 3;
			fact1 = 7;
			fact2 = 11;
			break;
		case 1317120:
			is_match = true;
			fact0 = 3;
			fact1 = 5;
			fact2 = 32;
			break;
		case 1666560:
			is_match = true;
			fact0 = 3;
			fact1 = 5;
			fact2 = 31;
			break;
		case 1908480:
			is_match = true;
			fact0 = 3;
			fact1 = 5;
			fact2 = 28;
			break;
		default:
			is_match = false;
			break;
	}
	if (is_match) {
		std::stringstream ss1;
		ss1 << "n = " << this->n << ", istart = " << istart << ", target = " << target << ", idepth = " << idepth << ", factors = " << to_string<T, FACTORS_VEC_LEN>(factors) << std::endl;
		if (is_match && write_to_file && file_stream && file_stream.is_open()) {
			file_stream << ss1.rdbuf();
		} else if (is_match && factors.size() >= 4 && factors[0] == fact0 && factors[1] == fact1 && factors[2] == fact2 && (target == 1 || target > 32)) {
			std::cout << ss1.rdbuf();
		} else if (is_match && target >= 155) {
			std::cout << ss1.rdbuf();
		}
	}
	*/

    if (target == 1) {
        if (factors.size() >= min_factors && factors.size() <= max_factors && factors[0] != n) {
            bool bappend = true;
            if (get_verbose()) std::cout << "factors.size() = " << factors.size() << std::endl;
            for (size_t j = factors.size(); j-- > 1; ) {
                for (size_t k = j; k-- > 0; ) {
                    if (get_verbose()) std::cout << "factors[" << j << "] % factors[" << k << "] (" << factors[j] << " % " << factors[k] << ") == " << (factors[j] % factors[k]) << std::endl;
                    if (factors[j] % factors[k] == 0) {
                        bappend = false;
                        break;
                    }
                }
                if (!bappend) {
                    break;
                }
            }

            if (get_verbose()) std::cout << "_backtrack(), bappend = " << bappend << ", factors = " << to_string<T, FACTORS_VEC_LEN>(factors) << std::endl;
            if (bappend) {
                std::sort(factors.begin(), factors.end());
				//aryary->push_back<FACTORS_VEC_LEN>(factors);
                //aryary->push_back_small_vector<FACTORS_VEC_LEN>(factors);
				aryary->push_back(factors);
            }
        }
        //return none;
    
    } else {

		vec_divisors<T> vec = get_div().divisors(target);
		if (get_verbose()) std::cout << "_backtrack(), divisors(" << target << "), vec.size() = " << vec.size() << ", vec = " << to_string<T, DIVISORS_VEC_LEN>(vec) << std::endl;
		/*
		if (is_match) {
			std::stringstream ss2;
			ss2 << "n = " << this->n << ", istart = " << istart << ", target = " << target << ", idepth = " << idepth << ", vec_divisors = [";
			for (size_t i = 1; i < vec.size(); i++) {
				if (i < vec.size() - 1) { ss2 << vec[i] << ", "; }
				else { ss2 << vec[i] << "]" << std::endl; }
			}
			if (write_to_file && file_stream && file_stream.is_open()) {
				file_stream << ss2.rdbuf();
			} else if (factors.size() >= 3 && factors[0] == fact0 && factors[1] == fact1 && factors[2] == fact2) {
				std::cout << ss2.rdbuf();
			}
		}
		*/
		/*
		if (factors.size() + vec.size() > factors.capacity()) {
			factors.reserve(factors.capacity() + std::max(vec.size(), static_cast<size_t>(32)));
			if (get_verbose()) std::cout << "_backtrack(), factors.capacity() = " << factors.capacity() << ", factors.size() = " << factors.size() << std::endl;
		}
		*/
		size_t i = 1;
		if (!gt_half && vec[1] == 2) {
			i = 2;
		}
		T iback = (factors.empty()) ? 0 : factors.back();
        for (; i < vec.size(); i++) {
            //T current_divisor = vec[i];
			if (get_verbose()) {
				std::cout << "_backtrack(), factors.empty() = " << factors.empty() << ", factors.size() = " << factors.size() << std::endl;
				std::cout << "_backtrack(), max_factors = " << max_factors << ", vec[" << i << "] = " << vec[i] << ", factors.back() = " << factors.back() << std::endl;
			}
            //if ((gt_half || vec[i] != 2) && factors.size() < max_factors && (factors.size() == 0 || vec[i] != factors.back()) && !is_divisible(factors, factors.size(), vec[i])) {
			//if ((gt_half || vec[i] != 2) && factors.size() < max_factors && !is_divisible(factors, factors.size(), vec[i])) {
			//if ((gt_half || vec[i] != 2) && factors.size() < max_factors) {
			//if (factors.empty() || (factors.size() < max_factors && vec[i] >= factors.back())) {
			if (factors.empty() || (factors.size() < max_factors && vec[i] > iback && !is_divisible(factors, factors.size(), vec[i]))) {
                factors.push_back(vec[i]);
                _backtrack(idepth, vec[i], target / vec[i], factors);
                factors.pop_back();
            }
        }
    }

    //return { aryary.keys, aryary.values };
    //return aryary;
}

/*
template<ValidIntegerType T>
void Combinations<T>::backtrack(T target, std::vector<T>& factors) {
	vec_factors<T> _factors = vec_factors<T>(factors.begin(), factors.end());
	
    //if (factors.size() >= 2 && factors[0] == 6 && factors[1] == 8) std::cout << "backtrack(" << target << ", " << to_string<T, FACTORS_VEC_LEN>(factors) << ")" << std::endl;

    if (target == 1) {
        if (_factors.size() >= min_factors && _factors.size() <= max_factors && _factors[0] != n) {
            bool bappend = true;
            if (get_verbose()) std::cout << "factors.size() = " << _factors.size() << std::endl;
            for (size_t j = _factors.size(); j-- > 1; ) {
                for (size_t k = j; k-- > 0; ) {
                    if (get_verbose()) std::cout << "factors[" << j << "] % factors[" << k << "] (" << _factors[j] << " % " << _factors[k] << ") == " << (_factors[j] % _factors[k]) << std::endl;
                    if (_factors[j] % _factors[k] == 0) {
                        bappend = false;
                        break;
                    }
                }
                if (!bappend) {
                    break;
                }
            }

            if (get_verbose()) std::cout << "bappend == " << bappend << ", factors = " << to_string<T, FACTORS_VEC_LEN>(_factors) << std::endl;
            if (bappend) {
                std::sort(_factors.begin(), _factors.end());
                aryary->push_back<FACTORS_VEC_LEN>(_factors);
            }
        }
        //return none;
    
    } else {

		vec_divisors<T> vec = get_div().divisors(target);
        for (size_t i = 1; i < vec.size(); i++) {
            //T current_divisor = vec[i];
            if (_factors.empty() || (_factors.size() < max_factors && vec[i] >= _factors.back())) {
                _factors.push_back(vec[i]);
                _backtrack(target / vec[i], _factors);
                _factors.pop_back();
            }
        }
    }

    //return { aryary.keys, aryary.values };
    //return aryary;
}
*/


PYBIND11_MODULE(combinations, m) {
    m.doc() = "combinations made with pybind11";

#include <../src/CombinationsPy.cpp>

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
