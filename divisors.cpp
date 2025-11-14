#include <pybind11/pybind11.h>
#include <iostream>
#include <msieve.h>

namespace py = pybind11;

void factor(int n) {
	msieve_obj *obj = msieve_obj_new(n, MSIEVE_FLAG_QUIET, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 1, 0, NULL); 
	msieve_run(obj);
	msieve_factor *fact = obj->factors;
        while (fact != NULL) {
            printf("Factor: %s (%s)\n", fact->s, (fact->factor_type == MSIEVE_PRP) ? "prp" : (fact->factor_type == MSIEVE_PRIME) ? "prime" : "composite");
            fact = fact->next;
	}
	msieve_obj_free(obj);
}

PYBIND11_MODULE(divisors, m) {
    m.doc() = "pybind11 example plugin";
    m.def("factor", &factor);
#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}