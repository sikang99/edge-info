package main

// #cgo pkg-config: python3
// #include <Python.h>
import "C"

import (
	"unsafe"
)

func main() {

	pycodeGo := `
import sys
for path in sys.path:
	print(path)
` 

	defer C.Py_Finalize()
	C.Py_Initialize()
	pycodeC := C.CString(pycodeGo)
	defer C.free(unsafe.Pointer(pycodeC))
	C.PyRun_SimpleString(pycodeC)
}
