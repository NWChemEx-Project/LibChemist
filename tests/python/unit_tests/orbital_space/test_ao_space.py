import unittest
import chemist

class TestAOSpace(unittest.TestCase):
    def test_ao_space(self):
        # Test initialization
        ao_space_d = chemist.AOSpaceD()
        ao_space_f = chemist.AOSpaceF()
        self.assertIsNotNone(ao_space_d)
        self.assertIsNotNone(ao_space_f)

        # Test basis set
        basis_set_d = chemist.basis_set.AOBasisSetD()
        basis_set_f = chemist.basis_set.AOBasisSetF()
        ao_space_d = chemist.AOSpaceD(basis_set_d)
        ao_space_f = chemist.AOSpaceF(basis_set_f)
        self.assertEqual(ao_space_d.basis_set, basis_set_d)
        self.assertEqual(ao_space_f.basis_set, basis_set_f)

        # Test size
        size_d = ao_space_d.size()
        size_f = ao_space_f.size()
        self.assertEqual(size_d, 0)
        self.assertEqual(size_f, 0)

        # Test equality
        ao_space_d2 = chemist.AOSpaceD()
        ao_space_f2 = chemist.AOSpaceF()
        self.assertEqual(ao_space_d, ao_space_d2)
        self.assertEqual(ao_space_f, ao_space_f2)

        # Test inequality
        center = chemist.basis_set.AtomicBasisSetD("", 0, 1.0, 2.0, 3.0)
        basis_set_d.add_center(center)
        ao_space_d2.basis_set = basis_set_d
        self.assertEqual(ao_space_d2.size(), 1)
        self.assertNotEqual(ao_space_d, ao_space_d2)