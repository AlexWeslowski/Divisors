#include <../include/CombinationsIterator.h>

using namespace std;

namespace py = pybind11;

template <ValidIntegerType T>
CombinationsIterator<T>::CombinationsIterator() {}

template <ValidIntegerType T>
CombinationsIterator<T>::CombinationsIterator(const std::shared_ptr<ArrayArray<T>> aryary) : aryary_(aryary), index(0) {}

template <ValidIntegerType T>
std::vector<T> CombinationsIterator<T>::next() {
    if (index >= aryary_->size()) {
        throw pybind11::stop_iteration();
    }
    return aryary_->get(index++);
}



PYBIND11_MODULE(combinationsiterator, m) {
    m.doc() = "combinationsiterator made with pybind11";

#include "../src/CombinationsIteratorPy.cpp"

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}