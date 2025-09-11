	
	
	/*
	py::class_<CombinationsIterator<int64_t>>(m, "CombinationsIterator")
	.def(py::init<ArrayArray<int64_t>>(), py::arg("aryary"))
	//.def("__iter__", [](CombinationsIterator<int64_t>& it) -> CombinationsIterator<int64_t>& { return it; })
	.def("__next__", &CombinationsIterator<int64_t>::next);
	*/
	
	py::class_<CombinationsIterator<int64_t>, std::shared_ptr<CombinationsIterator<int64_t>>>(m, "CombinationsIterator")
	//py::class_<CombinationsIterator<int64_t>>(m, "CombinationsIterator")
	.def(py::init())
	.def(py::init<std::shared_ptr<ArrayArray<int64_t>>>(), py::arg("aryary"))
	.def("__iter__", [](CombinationsIterator<int64_t>& it) -> CombinationsIterator<int64_t>& { return it; })
	.def("__next__", &CombinationsIterator<int64_t>::next, py::keep_alive<0, 1>());


