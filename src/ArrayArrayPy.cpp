

	/*
	using ArrayArrayT = ArrayArray<int64_t>;
	std::string type_name = "";
	py::class_<ArrayArrayT>(m, ("ArrayArray" + type_name).c_str())
		.def(py::init<size_t, bool>(), py::arg("capacity"), py::arg("resizeable"))
		.def("append", &ArrayArrayT::append, py::arg("ary"))
		.def("removeAt", &ArrayArrayT::removeAt, py::arg("idx"))
		.def("__len__", &ArrayArrayT::size)
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
.def("to_list", &ArrayArray<int64_t>::to_list)
.def("values_len", &ArrayArray<int64_t>::values_len)
.def("values_size", &ArrayArray<int64_t>::values_size)
//.def("get_size", &ArrayArray<int64_t>::size)
//.def("get_item", &ArrayArray<int64_t>::getItem)
.def("__len__", &ArrayArray<int64_t>::size)
.def("__getitem__", &ArrayArray<int64_t>::get);

