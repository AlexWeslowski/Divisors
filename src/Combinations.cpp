#include <../include/Combinations.h>
using namespace std;

namespace py = pybind11;

template<ValidIntegerType T>
Combinations<T>::Combinations() {
    //none = ArrayArray<T>(1, false);
    //cmbinations = ArrayArray<T>(2048, true);
    none = std::make_shared<ArrayArray<T>>(1, false);
    combinations = std::make_shared<ArrayArray<T>>(2048, true);
}


template<ValidIntegerType T>
//std::pair<std::vector<T>, std::vector<T>> Combinations<T>::backtrack(T n, T target, std::vector<T>& factors) {
std::shared_ptr<ArrayArray<T>> Combinations<T>::backtrack(T n, T target, std::vector<T>& factors) {
    if (target == 1) {
        if (factors.size() >= 1 && factors.size() <= max_factors && factors[0] != n) {
            bool bappend = true;
            if (Globals::verbose) std::cout << "factors.size() = " << factors.size() << std::endl;
            for (size_t j = factors.size(); j-- > 1; ) {
                for (size_t k = j; k-- > 0; ) {
                    if (Globals::verbose) std::cout << "factors[" << j << "] % factors[" << k << "] (" << factors[j] << " % " << factors[k] << ") == " << (factors[j] % factors[k]) << std::endl;
                    if (factors[j] % factors[k] == 0) {
                        bappend = false;
                        break;
                    }
                }
                if (!bappend) {
                    break;
                }
            }

            if (bappend) {
                //std::vector<T> sorted_factors(factors.begin() + 1, factors.end());
                std::sort(factors.begin(), factors.end());
                combinations->push_back(factors);
            }
        }
        return none;
    }

    //std::vector<T> vec;
    std::vector<T> vec = get_div().divisors(target);
    for (size_t i = 1; i < vec.size(); i++) {
        //T current_divisor = vec[i];
        if (factors.empty() || ((factors.size() < max_factors) && (vec[i] >= factors.back()))) {
            factors.push_back(vec[i]);
            backtrack(n, target / vec[i], factors);
            factors.pop_back();
        }
    }

    //return { combinations.keys, combinations.values };
    return combinations;
}

template<ValidIntegerType T>
void Combinations<T>::set_verbose(bool bln) {
    Globals::verbose = bln;
}



PYBIND11_MODULE(combinations, m) {
    m.doc() = "combinations made with pybind11";

    py::class_<Combinations<int64_t>>(m, "Combinations")
        .def(py::init())
        .def("backtrack", &Combinations<int64_t>::backtrack)
        .def("set_verbose", &Combinations<int64_t>::set_verbose);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}