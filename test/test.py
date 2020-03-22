import unittest
from fix32 import fix32
import math

class TestFix32(unittest.TestCase) :
    def setUp(self) :
        self.a = fix32(100.5)
        self.b = fix32(50.125)
        self.c = fix32(-50.125)
        self.d = fix32(50.125 * 50.125)
        self.e = fix32(100)
        self.f = fix32(5)

    def tearDown(self):
        pass

    def test_add(self) :
        self.assertEqual(150.625,self.a + self.b)

    def test_sub(self) :
        self.assertEqual(100.5-50.125,self.a - self.b)

    def test_mul(self) :
        self.assertEqual(100.5 * 50.125,self.a * self.b)

    def test_div(self) :
        self.assertEqual(100.5 / 50.125,self.a / self.b)

    def test_divv(self) :
        self.assertEqual(100.5 // 50.125,self.a // self.b)

    def test_mod(self) :
        self.assertEqual(100.5 % 50.125,self.a % self.b)

    def test_abs(self) :
        self.assertEqual(50.125,abs(self.c))

    def test_neg(self) :
        self.assertEqual(50.125,-self.c)

    def test_and(self) :
        self.assertEqual(5 & 100,self.e & self.f)

    def test_or(self) :
        self.assertEqual(5 | 100,self.e | self.f)

    def test_xor(self) :
        self.assertEqual(5 ^ 100,self.e ^ self.f)

    def test_rft(self) :
        self.assertEqual(5 >> 2,int(self.f >> fix32(2)))

    def test_lft(self) :
        self.assertEqual(5 << 2,self.f << fix32(2))

    def test_int(self) :
        self.assertEqual(50,int(self.b))

    def test_float(self) :
        self.assertEqual(50.125,float(self.b))

if __name__ == '__main__':
    unittest.main()
