#include <Python.h>
#include <sg14/fixed_point>
#include <string>

struct PyFixed_Object {
	PyObject_HEAD
	sg14::fixed_point<int64_t,-32> _value = sg14::fixed_point<>{0};
};

static int fixed_print(PyFixed_Object* v,FILE* fp,int flags) {
	double output_value = static_cast<double>(v->_value);
	Py_BEGIN_ALLOW_THREADS
	fprintf(fp, "%.8f", output_value);
	Py_END_ALLOW_THREADS
	return 0;
}

static PyObject* fixed_to_decimal_string(PyFixed_Object* v) {
	int32_t high = v -> _value.rep_data().get_high();
	uint32_t low = v -> _value.rep_data().get_low();
	std::string display = "(" + std::to_string(high) + "," + std::to_string(low) + ")";
	return PyString_FromString(display.c_str());
}

static int fixed_init(PyObject* self,PyObject* args,PyObject* kwds) {
	Py_ssize_t size = PyTuple_Size(args);
	if (size != 1 or kwds != NULL) {
		PyErr_SetString(PyExc_RuntimeError,"fixed32 init need only one param");
		return -1;
	}

	PyObject* init_value = PyTuple_GetItem(args,0);

	if (PyFloat_Check(init_value)) {
		PyFixed_Object* now = (PyFixed_Object*)self;
		now->_value = sg14::fixed_point<>{PyFloat_AsDouble(init_value)};
		return 0;
	}

	if (PyInt_Check(init_value)) {
		PyFixed_Object* now = (PyFixed_Object*)self;
		now->_value = sg14::fixed_point<>{_PyInt_AsInt(init_value)};
		return 0;
	}

	PyErr_SetString(PyExc_TypeError,"fixed32 init need float or int value");
	return -1;
}

PyTypeObject PyFixed_Point_Type = {
	PyVarObject_HEAD_INIT(&PyType_Type, 0)
	"fixed32",								/* tp_name */
	sizeof(PyFixed_Object),					/* tp_basicsize */
	0,										/* tp_itemsize */
	0,										/* tp_dealloc */
	(printfunc)fixed_print,					/* tp_print */
	0,										/* tp_getattr */
	0,										/* tp_setattr */
    0,										/* tp_compare */
    (reprfunc)fixed_to_decimal_string,            /* tp_repr */
    0,											/* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,											/* tp_hash */
    0,                                          /* tp_call */
    (reprfunc)fixed_to_decimal_string,          /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,							/* tp_flags */
    "fixed_point_32",                            /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,			                                /* tp_methods */
    0,	                                        /* tp_members */
    0,					                        /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    fixed_init,                                 /* tp_init */
    0,                                          /* tp_alloc */
    PyType_GenericNew,							/* tp_new */
    PyObject_Del,								/* tp_free */
};

PyMODINIT_FUNC initfix32(void) {
	PyObject* m;
	m = Py_InitModule3("fix32",NULL,"fixed_point32 support");
	if (m == NULL)
		return;

	if (PyType_Ready(&PyFixed_Point_Type) < 0)
		return;

	Py_INCREF(&PyFixed_Point_Type);
	PyModule_AddObject(m, "fix32", (PyObject *)&PyFixed_Point_Type);
	PyModule_AddIntConstant(m, "version", 1);
}

