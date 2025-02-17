// Python extension module (hashset_module.c)
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "hashset.h"

typedef struct {
    PyObject_HEAD
    HashSet* set;
} PyHashSet;

static void PyHashSet_dealloc(PyHashSet* self) {
    hashset_destroy(self->set);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyHashSet_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyHashSet* self = (PyHashSet*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->set = hashset_create(16);  // Default initial capacity
        if (self->set == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*)self;
}

static PyObject* PyHashSet_add(PyHashSet* self, PyObject* args) {
    int value;
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
    
    bool result = hashset_add(self->set, value);
    return PyBool_FromLong(result);
}

static PyObject* PyHashSet_contains(PyHashSet* self, PyObject* args) {
    int value;
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
    
    bool result = hashset_contains(self->set, value);
    return PyBool_FromLong(result);
}

static PyObject* PyHashSet_remove(PyHashSet* self, PyObject* args) {
    int value;
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
    
    bool result = hashset_remove(self->set, value);
    return PyBool_FromLong(result);
}

static PyMethodDef PyHashSet_methods[] = {
    {"add", (PyCFunction)PyHashSet_add, METH_VARARGS, "Add a value to the set"},
    {"contains", (PyCFunction)PyHashSet_contains, METH_VARARGS, "Check if value exists in set"},
    {"remove", (PyCFunction)PyHashSet_remove, METH_VARARGS, "Remove a value from the set"},
    {NULL}
};

static PyTypeObject PyHashSetType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "alexhashset.HashSet",
    .tp_doc = "HashSet object",
    .tp_basicsize = sizeof(PyHashSet),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyHashSet_new,
    .tp_dealloc = (destructor)PyHashSet_dealloc,
    .tp_methods = PyHashSet_methods,
};

static PyModuleDef hashset_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "alexhashset",  // Changed from "hashset"
    .m_doc = "Alex's custom HashSet implementation.",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_alexhashset(void) {
    PyObject* m;
    if (PyType_Ready(&PyHashSetType) < 0)
        return NULL;

    m = PyModule_Create(&hashset_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyHashSetType);
    if (PyModule_AddObject(m, "HashSet", (PyObject*)&PyHashSetType) < 0) {
        Py_DECREF(&PyHashSetType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}