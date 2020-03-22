#include <Python.h>
#include <sg14/fixed_point>
#include <string>

struct PyFixed_Object {
	PyObject_HEAD
	sg14::fixed_point<int64_t,-32> ob_ival = sg14::fixed_point<>{0};
};

sg14::fixed_point<int64_t,-32> get_fixed(PyObject* obj) {
	if (PyFloat_Check(obj)) 
		return std::move(sg14::fixed_point<int64_t,-32>{PyFloat_AsDouble(obj)});
	else 
		return std::move(sg14::fixed_point<int64_t,-32>{_PyInt_AsInt(obj)});
}

static PyObject* fixed_add(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	
	result->ob_ival = fst->ob_ival + sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_sub(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	
	result->ob_ival = fst->ob_ival - sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_mul(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	
	result->ob_ival = fst->ob_ival * sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_div(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;

	if (sec->ob_ival == 0) {                        
		PyErr_SetString(PyExc_ZeroDivisionError,"division by zero");    
		return NULL;                            
	}                                           
	
	result->ob_ival = fst->ob_ival / sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_mod(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;

	if (sec->ob_ival == 0) {                              
		PyErr_SetString(PyExc_ZeroDivisionError,"division by zero");    
		return NULL;                            
	}                                           
	
	result->ob_ival = fst->ob_ival % sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_neg(PyFixed_Object* fst) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	
	result->ob_ival = -fst->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_abs(PyFixed_Object* fst) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	
	result->ob_ival = abs(fst->ob_ival);
	return (PyObject*)result;
}

static int fixed_nonzero(PyFixed_Object* fst) {
	return fst->ob_ival != 0;
}

static PyObject* fixed_invert(PyFixed_Object* fst) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	
	result->ob_ival = ~fst->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_lshift(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	if (sec < 0) {                                  
		PyErr_SetString(PyExc_ValueError, "negative shift count");
		return NULL;  
	}                                     
	result->ob_ival = fst->ob_ival << sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_rshift(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	if (sec < 0) {                                                
		PyErr_SetString(PyExc_ValueError, "negative shift count");
		return NULL;                                            
	}                                                             
	result->ob_ival = fst->ob_ival >> sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_and(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;

	result->ob_ival = fst->ob_ival & sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_xor(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;

	result->ob_ival = fst->ob_ival ^ sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_or(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;

	result->ob_ival = fst->ob_ival | sec->ob_ival;
	return (PyObject*)result;
}

static PyObject* fixed_int(PyFixed_Object* fst) {
	return PyInt_FromLong(int(fst->ob_ival));
}

static PyObject* fixed_float(PyFixed_Object* fst) {
	return PyFloat_FromDouble(float(fst->ob_ival));
}

static int fixed_print(PyFixed_Object* v,FILE* fp,int flags) {
	double output_value = static_cast<double>(v->ob_ival);
	Py_BEGIN_ALLOW_THREADS
	fprintf(fp, "Fixed32 :%.8f", output_value);
	Py_END_ALLOW_THREADS
	return 0;
}

static PyObject* fixed_floor_div(PyFixed_Object* fst,PyFixed_Object* sec) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;

	if (sec->ob_ival == 0) {                              
		PyErr_SetString(PyExc_ZeroDivisionError,"division by zero");    
		return NULL;                            
	}                                           
	
	result->ob_ival = fst->ob_ival / sec->ob_ival;
	result->ob_ival.set_low_zero();
	return (PyObject*)result;
}

static long fixed_hash(PyFixed_Object *v) {
	return v->ob_ival.data();
}

static PyObject* fixed_to_decimal_string(PyFixed_Object* v) {
	int32_t high = v -> ob_ival.rep_data().get_high();
	uint32_t low = v -> ob_ival.rep_data().get_low();
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
		now->ob_ival = sg14::fixed_point<>{PyFloat_AsDouble(init_value)};
		return 0;
	}

	if (PyInt_Check(init_value)) {
		PyFixed_Object* now = (PyFixed_Object*)self;
		now->ob_ival = sg14::fixed_point<>{_PyInt_AsInt(init_value)};
		return 0;
	}

	PyErr_SetString(PyExc_TypeError,"fixed32 init need float or int value");
	return -1;
}

static PyObject * fixed_sin(PyFixed_Object *fst) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	
	result->ob_ival = sin(fst->ob_ival);
	return (PyObject*)result;
}


static PyObject * fixed_cos(PyFixed_Object *fst) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	
	result->ob_ival = cos(fst->ob_ival);
	return (PyObject*)result;
}

static PyObject * fixed_tan(PyFixed_Object *fst) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;
	
	result->ob_ival = tan(fst->ob_ival);
	return (PyObject*)result;
}

static PyObject * fixed_sqrt(PyFixed_Object *fst) {
	PyTypeObject* type = Py_TYPE(fst);
	PyFixed_Object * result = (PyFixed_Object *)(type->tp_alloc(type, 0));
	if (!result)
		return NULL;

	if (fst->ob_ival < 0) {
		PyErr_SetString(PyExc_ValueError, "negative sqrt value");
		return NULL;
	}

	result->ob_ival = sqrt(fst->ob_ival);
	return (PyObject*)result;
}

static int fixed_compare(PyFixed_Object *v, PyFixed_Object *w) {
	const sg14::fixed_point<int64_t,-32>& fst = v->ob_ival;
	const sg14::fixed_point<int64_t,-32>& sec = w->ob_ival;
	return (fst < sec) ? -1 : (fst > sec) ? 1 : 0;
}

PyObject * fixed_richcompare(PyObject *fst, PyObject *w, int op) {
	if (!PyFloat_Check(w) && !PyInt_Check(w)) {
		PyErr_SetString(PyExc_RuntimeError,"fixed32 not support this compare");
		return NULL;
	}
	
	PyFixed_Object* v = (PyFixed_Object*)fst;

	const sg14::fixed_point<int64_t,-32>& value = get_fixed(w);
	int cmp;
	PyObject *res;
	switch (op) {
		case Py_LT: cmp = v->ob_ival <  value; break;
		case Py_LE: cmp = v->ob_ival <= value; break;
		case Py_EQ: cmp = v->ob_ival == value; break;
		case Py_NE: cmp = v->ob_ival != value; break;
		case Py_GT: cmp = v->ob_ival >  value; break;
		case Py_GE: cmp = v->ob_ival >= value; break;
		default: return NULL; /* cannot happen */
	}
	if (cmp)
		res = Py_True;
	else
		res = Py_False;
	Py_INCREF(res);
	return res;
}

static PyNumberMethods fixed_as_number = {
    (binaryfunc)fixed_add,			/*nb_add*/
    (binaryfunc)fixed_sub,			/*nb_subtract*/
    (binaryfunc)fixed_mul,          /*nb_multiply*/
    (binaryfunc)fixed_div,			/*nb_divide*/
	(binaryfunc)fixed_mod,          /*nb_remainder*/
	(binaryfunc)fixed_mod,			/*nb_divmod*/
	0,								/*nb_power*/ 
	(unaryfunc)fixed_neg,			/*nb_negative*/
	0,								/*nb_positive*/
	(unaryfunc)fixed_abs,			/*nb_absolute*/
	(inquiry)fixed_nonzero,			/*nb_nonzero*/
	(unaryfunc)fixed_invert,		/*nb_invert*/
	(binaryfunc)fixed_lshift,		/*nb_lshift*/
	(binaryfunc)fixed_rshift,		/*nb_rshift*/
	(binaryfunc)fixed_and,			/*nb_and*/
	(binaryfunc)fixed_xor,			/*nb_xor*/
	(binaryfunc)fixed_or,			/*nb_or*/
	0,								/*nb_coerce*/
	(unaryfunc)fixed_int,			/*nb_int*/
	(unaryfunc)fixed_int,			/*nb_long*/
	(unaryfunc)fixed_float,			/*nb_float*/
	0,								/*nb_oct*/
	0,								/*nb_hex*/
	0,								/*nb_inplace_add*/
	0,								/*nb_inplace_subtract*/
	0,								/*nb_inplace_multiply*/
	0,								/*nb_inplace_divide*/
	0,								/*nb_inplace_remainder*/
	0,								/*nb_inplace_power*/
	0,								/*nb_inplace_lshift*/
	0,								/*nb_inplace_rshift*/	
	0,								/*nb_inplace_and*/
	0,								/*nb_inplace_xor*/
	0,								/*nb_inplace_or*/
	(binaryfunc)fixed_floor_div,	/* nb_floor_divide */
	(binaryfunc)fixed_div,			/* nb_true_divide */
	0,								/* nb_inplace_floor_divide */
	0,								/* nb_inplace_true_divide */
	0,								/* nb_index */
};

static PyMethodDef fixed_methods[] = {
	{"sin", (PyCFunction)fixed_sin,   METH_NOARGS,"Return sin value of fix32."},
	{"cos", (PyCFunction)fixed_cos,   METH_NOARGS,"Return cos value of fix32."},
	{"sqrt", (PyCFunction)fixed_sqrt,   METH_NOARGS,"Return sqrt value of fix32."},
	{"tan", (PyCFunction)fixed_tan,   METH_NOARGS,"Return tan value of fix32."},
	{NULL,              NULL}           /* sentinel */
};

PyTypeObject PyFixed_Point_Type = {
	PyVarObject_HEAD_INIT(&PyType_Type, 0)
	"fixed32",									/* tp_name */
	sizeof(PyFixed_Object),						/* tp_basicsize */
	0,											/* tp_itemsize */
	0,											/* tp_dealloc */
	(printfunc)fixed_print,						/* tp_print */
	0,											/* tp_getattr */
	0,											/* tp_setattr */
    (cmpfunc)fixed_compare,						/* tp_compare */
    (reprfunc)fixed_to_decimal_string,          /* tp_repr */
    &fixed_as_number,							/* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    (hashfunc)fixed_hash,						/* tp_hash */
    0,                                          /* tp_call */
    (reprfunc)fixed_to_decimal_string,          /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,							/* tp_flags */
    "fixed_point_32",                           /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    fixed_richcompare,                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    fixed_methods,	                            /* tp_methods */
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

