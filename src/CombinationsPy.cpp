    
    
py::class_<Combinations<int64_t>, std::shared_ptr<Combinations<int64_t>>>(m, "Combinations")
.def(py::init<int64_t>(), py::arg("n"))
.def_readwrite("min_factors", &Combinations<int64_t>::min_factors)
.def_readwrite("max_factors", &Combinations<int64_t>::max_factors)
.def("backtrack", &Combinations<int64_t>::backtrack)
.def("to_array", &Combinations<int64_t>::to_array)
.def("to_list", &Combinations<int64_t>::to_list)
.def("get_arrayarray", &Combinations<int64_t>::get_arrayarray)
.def("set_verbose", &Combinations<int64_t>::set_verbose)
//.def("__iter__", [](Combinations<int64_t>& c) { return CombinationsIterator<int64_t>(c.get_arrayarray()); }, py::keep_alive<0, 1>())
//.def("__iter__", [](py::object self) { return py::cast(CombinationsIterator<int64_t>(self.get_arrayarray()), py::return_value_policy::take_ownership, py::keep_alive<0, 1>()); })
//.def("__iter__", [](py::object self) { auto& c = py::cast<Combinations<int64_t>&>(self); return CombinationsIterator<int64_t>(c.get_arrayarray()); }, py::keep_alive<0, 1>())
.def("__iter__", [](Combinations<int64_t>& self) -> Combinations<int64_t>& { return self; })
.def("__next__", &Combinations<int64_t>::next)
.def("__len__", &Combinations<int64_t>::size)
.def("__getitem__", &Combinations<int64_t>::get);

