#define PY_SSIZE_T_CLEAN
#include <Python.h>

#undef PREALLOCATE

static PyObject *partition(PyObject *self, PyObject *args)
{
	PyObject *pred_obj, *it_obj;
	PyObject *iter, *item;
	PyObject *ts, *fs;
	PyObject *res;
	Py_ssize_t in_len;
#if PREALLOCATE
	size_t nr_true, nr_false;
#endif

	if (!PyArg_ParseTuple(args, "OO", &pred_obj, &it_obj))
		goto out;

	iter = PyObject_GetIter(it_obj);
	if (iter == NULL) {
		PyErr_SetString(PyExc_TypeError, "it must be iterable");
		goto out;
	}

	/* True to pre-allocate the output lists if we can */
#if PREALLOCATE
	in_len = PyObject_LengthHint(it_obj, 0);
	if (in_len < 0)
		in_len = 0;
	in_len >>= 1;
#else
	in_len = 0;
#endif

	fs = PyList_New(in_len);
	if (fs == NULL)
		goto out;

	ts = PyList_New(in_len);
	if (ts == NULL)
		goto out_free_fs;

#if PREALLOCATE
	nr_true = nr_false = 0;
#endif
	while ((item = PyIter_Next(iter))) {
		PyObject *ret;
		int ret_val;

		ret = PyObject_CallOneArg(pred_obj, item);
		if (ret == NULL) {
			goto out_free_ts;
		}

		ret_val = PyObject_IsTrue(ret);
		Py_DECREF(ret);

		if (ret_val) {
			PyList_Append(ts, item);
#if PREALLOCATE
			nr_true++;
#endif
		}else{
			PyList_Append(fs, item);
#if PREALLOCATE
			nr_false++;
#endif
		}

		Py_DECREF(item);
	}

#if PREALLOCATE
	/* If we pre-allocated the output lists then trim then down because the
	 * excess space will be filled with NULL's and that will crash the
	 * interpreter
	 */
	if (in_len) {
		PySequence_DelSlice(ts, nr_true, -1);
		PySequence_DelSlice(fs, nr_false, -1);
	}
#endif

	res = PyTuple_Pack(2, fs, ts);
	if (res == NULL)
		goto out_free_ts;

	return res;

out_free_ts:
	Py_DECREF(ts);
out_free_fs:
	Py_DECREF(fs);
out:
	return NULL;
}

static PyMethodDef _partition_methods[] = {
	{
		"partition",
		partition,
		METH_VARARGS,
		"Partition an iterable."
	},
	{
		NULL,
	},
};

static struct PyModuleDef _partition_module = {
	PyModuleDef_HEAD_INIT,
	"_partition",
	NULL,
	-1,
	_partition_methods,
};

PyMODINIT_FUNC
PyInit__partition(void)
{
	PyObject *m;

	m = PyModule_Create(&_partition_module);
	if (m == NULL)
		return NULL;

	return m;
}
