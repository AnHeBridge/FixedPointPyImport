# fixed_point and python extension
![avatar](http://e.hiphotos.baidu.com/baike/h%3D160/sign=46cdcd6d4a90f6031bb098410913b370/91529822720e0cf31b850fa70a46f21fbe09aa78.jpg)

## Description

This library base on (https://github.com/johnmcfarlane/fixed_point) library .
provide a header-only C++11 API for approximating real numbers using binary fixed-point arithmetic and build python extension.**There are still some constant efficiencies to be optimized。**
This project split number to low,high ,and do not need int128 support.
Support basic arithmetic operations(+-*/,sqrt,sin ....etc), view details from test.

## Download

The library is [hosted](https://github.com/johnmcfarlane/fixed_point) on GitHub:

```shell
$ git clone https://github.com/AnHeBridge/fixed_point.git
```

The API is exposed through headers in the [include](./include/) directory. Add this to your system header list and include, e.g.:

```c++
#include <sg14/fixed_point>
```
Use in python
```python
from fix32 import fix32
a = fix32(100.5)
```

## Build
```
mkdir build
cd build
cmake ..
make
```

## Run Tests
```
cd build
cmake .. -Dgtest_build_samples=ON
make
ctest -VV
cp fix32.so ../test/
cd ../test/
python test.py
```

## Further Reading 
	from http://johnmcfarlane.github.io/fixed_point/
	- [documentation](http://johnmcfarlane.github.io/fixed_point/)
	- [latest draft](doc/p0037.md) of C++ Standard Library proposal, [P0037](doc/p0037.md)
	- position paper illustrating the compositonal approach, [P0554](doc/p0554.md)
	- forum of [Study Group 14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)
	from others 
	 - div_functions :Knuth TAOCP Vol.2 shifted_div

