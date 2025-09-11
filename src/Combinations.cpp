#include <../include/Combinations.h>
#include <../include/CombinationsIterator.h>
using namespace std;

namespace py = pybind11;

template<ValidIntegerType T>
Combinations<T>::Combinations(T n) {
    //none = ArrayArray<T>(1, false);
    //cmbinations = ArrayArray<T>(2048, true);
    this->n = n;
    none = std::make_shared<ArrayArray<T>>(0, false);
    aryary = std::make_shared<ArrayArray<T>>(2048, true);
}

template<ValidIntegerType T>
size_t Combinations<T>::size() const {
    return (aryary) ? aryary->size() : 0;
}

template<ValidIntegerType T>
size_t Combinations<T>::len() const {
    return (aryary) ? aryary->len() : 0;
}

template <ValidIntegerType T>
std::vector<T> Combinations<T>::next() {
	if (get_verbose()) std::cout << "Combinations::next() index = " << index << ", aryary->size() = " << ((aryary) ? aryary->size() : 0) << std::endl;
    if (!aryary || index >= aryary->size()) {
        index = 0;
        throw pybind11::stop_iteration();
    }
    std::vector<T> vec = aryary->get(index++);
    bool iter = (vec.size() == 0);
	while (vec.size() == 0 && index < aryary->size()) {
		vec = aryary->get(index++);
	}
	if (iter && vec.size() == 0 && index >= aryary->size()) {
        index = 0;
		throw pybind11::stop_iteration();
	}
    return vec;
}

template<ValidIntegerType T>
std::vector<T> Combinations<T>::get(size_t idx) const {
    return (aryary) ? aryary->get(idx) : std::vector<T>();
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
std::vector<std::vector<T>> Combinations<T>::to_array() const {
    return (aryary) ? aryary->to_array() : std::vector<std::vector<T>>();
}


template<ValidIntegerType T>
std::vector<std::vector<T>> Combinations<T>::to_list() const {
    return (aryary) ? aryary->to_list() : std::vector<std::vector<T>>();
}

template<ValidIntegerType T>
std::shared_ptr<ArrayArray<T>> Combinations<T>::get_arrayarray() const {
    return aryary;
}

template<ValidIntegerType T>
bool Combinations<T>::get_verbose() const {
    return (verbose.has_value() && verbose.value()) || (!verbose.has_value() && Globals::verbose);
}

template<ValidIntegerType T>
void Combinations<T>::set_verbose(bool bln) {
    Globals::verbose = bln;
}

template<ValidIntegerType T>
//std::pair<std::vector<T>, std::vector<T>> Combinations<T>::backtrack(T n, T target, std::vector<T>& factors) {
void Combinations<T>::backtrack(T target, std::vector<T>& factors) {
    //if (factors.size() >= 2 && factors[0] == 6 && factors[1] == 8) std::cout << "backtrack(" << target << ", " << to_string(factors) << ")" << std::endl;

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

            if (get_verbose()) std::cout << "bappend == " << bappend << ", factors = " << to_string(factors) << std::endl;
            if (bappend) {
                //std::vector<T> sorted_factors(factors.begin() + 1, factors.end());
                std::sort(factors.begin(), factors.end());
                aryary->push_back(factors);
            }
        }
        //return none;
    
    } else {

        //std::vector<T> vec;
        std::vector<T> vec = get_div().divisors(target);
        for (size_t i = 1; i < vec.size(); i++) {
            //T current_divisor = vec[i];
            if (factors.empty() || (factors.size() < max_factors && vec[i] >= factors.back())) {
                factors.push_back(vec[i]);
                backtrack(target / vec[i], factors);
                factors.pop_back();
            }
        }
    }

    //return { aryary.keys, aryary.values };
    //return aryary;
}



PYBIND11_MODULE(combinations, m) {
    m.doc() = "combinations made with pybind11";

#include <../src/CombinationsPy.cpp>

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
