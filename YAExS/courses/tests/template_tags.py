"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
from datetime import time

from django.test import TestCase
from mock import Mock

from courses.templatetags.course_tags import remove_zero_prefix, get, display_period, dow_short
from courses.tests.factories import PeriodFactory


__all__ = ['RemoveZeroPrefixTest', 'GetTest', 'DowShortTest']


class RemoveZeroPrefixTest(TestCase):
    def test_remove_zero_prefix_with_zero_prefix(self):
        self.assertEqual(remove_zero_prefix('01'), '1')

    def test_remove_zero_prefix_with_no_zero_prefix(self):
        self.assertEqual(remove_zero_prefix('21'), '21')


class GetTest(TestCase):
    def test_get_with_object(self):
        obj = Mock()
        obj.lol = 342
        self.assertEqual(get(obj, 'lol'), 342)

    def test_get_with_index(self):
        obj = [1, 2, 3]
        self.assertEqual(get(obj, 1), 2)

    def test_get_with_key_index(self):
        obj = {'lol': 'rofl'}
        self.assertEqual(get(obj, 'lol'), 'rofl')

    def test_get_should_return_None_on_invalid_access(self):
        obj = []
        self.assertEqual(get(obj, 4), None)


class DisplayPeriodTest(TestCase):
    def test_display_period_of_4_to_5(self):
        period = PeriodFactory.build(start=time(hour=4), end=time(hour=5))
        self.assertEqual(display_period(period), '4:00-5:00 am')

    def test_display_period_with_minutes(self):
        period = PeriodFactory.build(start=time(hour=4), end=time(hour=4, minute=50))
        self.assertEqual(display_period(period), '4:00-4:50 am')

    def test_display_period_in_the_afternoon(self):
        period = PeriodFactory.build(start=time(hour=14), end=time(hour=14, minute=50))
        self.assertEqual(display_period(period), '2:00-2:50 pm')

    def test_display_period_from_morning_to_afternoon(self):
        period = PeriodFactory.build(start=time(hour=10), end=time(hour=14, minute=50))
        self.assertEqual(display_period(period), '10:00 am-2:50 am')


class DowShortTest(TestCase):
    def test_monday(self):
        self.assertEqual(dow_short('Monday'), 'Mo')

    def test_tuesday(self):
        self.assertEqual(dow_short('Tuesday'), 'Tu')

    def test_wednesday(self):
        self.assertEqual(dow_short('Wednesday'), 'We')

    def test_thursday(self):
        self.assertEqual(dow_short('Thursday'), 'Th')

    def test_friday(self):
        self.assertEqual(dow_short('Friday'), 'Fr')

    def test_saturday(self):
        self.assertEqual(dow_short('Saturday'), 'Sa')

    def test_sunday(self):
        self.assertEqual(dow_short('Sunday'), 'Su')

    def test_return_None_otherwise(self):
        self.assertEqual(dow_short('foobar'), None)

    def test_convert_list(self):
        collection = ['Monday', 'Wednesday', 'Thursday']
        self.assertEqual(dow_short(collection), ('Mo', 'We', 'Th'))

    def test_convert_tuple(self):
        collection = ('Monday', 'Wednesday', 'Thursday')
        self.assertEqual(dow_short(collection), ('Mo', 'We', 'Th'))
