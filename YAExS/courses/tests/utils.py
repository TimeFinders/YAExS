"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
from collections import namedtuple

from django.test import TestCase

from courses import utils


class CapitalizationTest(TestCase):
    def test_capitalization_for_all_lowercase(self):
        self.assertEqual(utils.capitalized("foobar"), "Foobar")

    def test_capitalization_for_all_uppercase(self):
        self.assertEqual(utils.capitalized("FOOBAR"), "Foobar")

    def test_capitalization_for_mixed_case(self):
        self.assertEqual(utils.capitalized("fOoBaR"), "Foobar")

    def test_capitalization_for_letter(self):
        self.assertEqual(utils.capitalized("a"), "A")

    def test_capitalization_for_blank(self):
        self.assertEqual(utils.capitalized(""), "")

    def test_capitalization_spaces(self):
        self.assertEqual(utils.capitalized("   "), "   ")


class OptionsTest(TestCase):
    def test_returning_a_generator(self):
        g = utils.options()
        self.assertEqual(g.next(), 1)
        self.assertEqual(g.next(), 2)
        self.assertEqual(g.next(), 4)
        self.assertEqual(g.next(), 8)
        self.assertEqual(g.next(), 16)
        self.assertEqual(g.next(), 32)
        # this generator is infinite...

    def test_returning_a_list(self):
        g = utils.options(6)
        self.assertEqual(g, [1, 2, 4, 8, 16, 32])


class ExtendedGetAttrTest(TestCase):
    SampleObject = namedtuple('SampleObject', ('a', 'b'))
    NestedSampleObject = namedtuple('NestedSampleObject', ('c', 'd'))

    def test_single_attr(self):
        o = self.SampleObject(1, 2)
        self.assertEqual(utils.extended_getattr(o, 'a'), 1)
        self.assertEqual(utils.extended_getattr(o, 'b'), 2)

    def test_nested_attr(self):
        o = self.SampleObject(self.NestedSampleObject(1, 2), 3)
        self.assertEqual(utils.extended_getattr(o, 'a.c'), 1)
        self.assertEqual(utils.extended_getattr(o, 'a.d'), 2)
        self.assertEqual(utils.extended_getattr(o, 'b'), 3)

    def test_attribute_error_on_invalid_attr(self):
        o = self.SampleObject(1, 2)
        self.assertRaises(utils.ExtendedAttributeError, utils.extended_getattr, o, 'f')

    def test_attribute_error_on_invalid_nested_attr(self):
        o = self.SampleObject(self.NestedSampleObject(1, 2), 3)
        self.assertRaises(utils.ExtendedAttributeError, utils.extended_getattr, o, 'a.f')

    def test_defaults_on_invalid_attr(self):
        o = self.SampleObject(1, 2)
        self.assertEqual(utils.extended_getattr(o, 'f', 3), 3)

    def test_defaults_on_invalid_nested_attr(self):
        o = self.SampleObject(self.NestedSampleObject(1, 2), 3)
        self.assertEqual(utils.extended_getattr(o, 'a.f', 9), 9)


class DictByAttrTest(TestCase):
    SampleObject = namedtuple('SampleObject', ('a', 'b', 'c'))

    def test_mapping(self):
        items = [self.SampleObject(i, i + 1, i + 2) for i in range(10)]
        ordered = utils.dict_by_attr(items, 'a')
        self.assertEqual([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], ordered.keys())
        self.assertEqual([(0, 1, 2)], ordered[0])
        self.assertEqual([(1, 2, 3)], ordered[1])
        self.assertEqual([(2, 3, 4)], ordered[2])
        self.assertEqual([(3, 4, 5)], ordered[3])
        self.assertEqual([(4, 5, 6)], ordered[4])
        self.assertEqual([(5, 6, 7)], ordered[5])
        self.assertEqual([(6, 7, 8)], ordered[6])
        self.assertEqual([(7, 8, 9)], ordered[7])
        self.assertEqual([(8, 9, 10)], ordered[8])
        self.assertEqual([(9, 10, 11)], ordered[9])

    def test_mapping_in_groups(self):
        items = [self.SampleObject(i % 3, i + 1, i + 2) for i in range(10)]
        ordered = utils.dict_by_attr(items, 'a')
        self.assertEqual([0, 1, 2], ordered.keys())
        self.assertEqual([(0, 1, 2), (0, 4, 5), (0, 7, 8), (0, 10, 11)], ordered[0])
        self.assertEqual([(1, 2, 3), (1, 5, 6), (1, 8, 9), ], ordered[1])
        self.assertEqual([(2, 3, 4), (2, 6, 7), (2, 9, 10), ], ordered[2])

    def test_mapping_with_lambda(self):
        items = [self.SampleObject(i, i + 1, i + 2) for i in range(10)]
        ordered = utils.dict_by_attr(items, lambda o: o.a)
        self.assertEqual([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], ordered.keys())
        self.assertEqual([(0, 1, 2)], ordered[0])
        self.assertEqual([(1, 2, 3)], ordered[1])
        self.assertEqual([(2, 3, 4)], ordered[2])
        self.assertEqual([(3, 4, 5)], ordered[3])
        self.assertEqual([(4, 5, 6)], ordered[4])
        self.assertEqual([(5, 6, 7)], ordered[5])
        self.assertEqual([(6, 7, 8)], ordered[6])
        self.assertEqual([(7, 8, 9)], ordered[7])
        self.assertEqual([(8, 9, 10)], ordered[8])
        self.assertEqual([(9, 10, 11)], ordered[9])

    def test_mapping_with_value(self):
        items = [self.SampleObject(i, i + 1, i + 2) for i in range(10)]
        ordered = utils.dict_by_attr(items, 'a', 'b')
        self.assertEqual([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], ordered.keys())
        self.assertEqual([1], ordered[0])
        self.assertEqual([2], ordered[1])
        self.assertEqual([3], ordered[2])
        self.assertEqual([4], ordered[3])
        self.assertEqual([5], ordered[4])
        self.assertEqual([6], ordered[5])
        self.assertEqual([7], ordered[6])
        self.assertEqual([8], ordered[7])
        self.assertEqual([9], ordered[8])
        self.assertEqual([10], ordered[9])
