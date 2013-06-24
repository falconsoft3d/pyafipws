/* 
 *  This file is part of PyAfipWs dynamical-link shared library
 *  Copyright (C) 2013 Mariano Reingart <reingart@gmail.com>
 *
 *  PyAfipWs is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PyAfipWs is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PyAfipWs.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Python.h>
#include "libpyafipws.h"

#define MODULE "wsaa"

EXPORT char * WSAA_CreateTRA(const char *service, long ttl) {

    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    char *ret;

    pName = PyString_FromString("wsaa");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    fprintf(stderr, "imported!\n");

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "create_tra");

        if (pFunc && PyCallable_Check(pFunc)) {
            fprintf(stderr, "pfunc!!!\n");
            pArgs = PyTuple_New(2);

            pValue = PyString_FromString(service);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return NULL;
            }
            PyTuple_SetItem(pArgs, 0, pValue);
            pValue = PyInt_FromLong(ttl);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return NULL;
            }
            PyTuple_SetItem(pArgs, 1, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);
            fprintf(stderr, "call!!!\n");
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                ret = PyString_AsString(pValue);
                printf("Result of call: %sd\n", ret);               
                Py_DECREF(pValue);
                return ret;
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return NULL;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load module\n");
        return NULL;
    }
}

