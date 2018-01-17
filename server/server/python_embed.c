//
//  python_embed.c
//  server
//
//  Created by working on 2018/1/17.
//  Copyright © 2018年 working. All rights reserved.
//

#include "python_embed.h"


void
initPythonEnv() {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
}

char *
callPythonFun(char *request)
{
    
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;
    
    char *modelName = "demo";
    const char *argv0 = "main";
    int numOfArgs = 1;
    
    pName = PyUnicode_DecodeFSDefault(modelName);
    /* Error checking of pName left out */
    
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    
    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv0);
        /* pFunc is a new reference */
        
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(numOfArgs);
            
            Py_ssize_t size = strlen(request);
            char *errors = "e";
            
            pValue = PyUnicode_DecodeUTF8(request, size, errors);
            PyTuple_SetItem(pArgs, 0, pValue);
            
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                Py_DECREF(pValue);
                
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                char *res = "error111";
                return res;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        
        // char *res = PyUnicode_AsUTF8(pValue);
        char *res = PyBytes_AsString(pValue);
        return res;
    }
    else {
        char *res = "error";
        return res;
    }
}



