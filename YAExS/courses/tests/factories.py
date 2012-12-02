"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
import datetime

import factory

from courses import models


class SemesterFactory(factory.Factory):
    FACTORY_FOR = models.Semester

    year = factory.Sequence(lambda n: 2009 + int(n))
    month = 1
    name = factory.Sequence(lambda n: u'Semester %03d' % int(n))
    ref = factory.LazyAttribute(lambda s: u'Semester%s-%s.xml' % (s.year, s.month))
    date_updated = factory.LazyAttribute(lambda s: datetime.datetime.now())
    visible = True


class DepartmentFactory(factory.Factory):
    FACTORY_FOR = models.Department

    name = factory.Sequence(lambda n: u'Department %03d' % int(n))
    code = factory.Sequence(lambda n: u'DEPT%03d' % int(n))


class PeriodFactory(factory.Factory):
    FACTORY_FOR = models.Period

    start = factory.Sequence(lambda n: datetime.time(hour=int(n) % 24))
    end = factory.Sequence(lambda n: datetime.time(hour=int(n) % 24, minute=50))
    days_of_week_flag = models.Period.MONDAY & models.Period.THURSDAY


class SectionCrosslistingFactory(factory.Factory):
    FACTORY_FOR = models.SectionCrosslisting

    semester = factory.LazyAttribute(lambda s: SemesterFactory())
    ref = factory.Sequence(lambda n: u'ref-%s' % n)


class SectionFactory(factory.Factory):
    FACTORY_FOR = models.Section

    number = factory.Sequence(unicode)
    crn = factory.Sequence(int)
    course = factory.LazyAttribute(lambda s: CourseFactory())
    semester = factory.LazyAttribute(lambda s: SemesterFactory())

    seats_taken = 10
    seats_total = 100
    notes = ''


class CourseFactory(factory.Factory):
    FACTORY_FOR = models.Course

    name = u'Course'
    number = factory.Sequence(int)
    department = factory.LazyAttribute(lambda s: DepartmentFactory())
    description = u'Just another description'
    #semesters =

    min_credits = 4
    max_credits = 4
    grade_type = ''

    prereqs = ''
    is_comm_intense = False


class OfferedForFactory(factory.Factory):
    FACTORY_FOR = models.OfferedFor

    course = factory.LazyAttribute(lambda s: CourseFactory())
    semester = factory.LazyAttribute(lambda s: SemesterFactory())


class SectionPeriodFactory(factory.Factory):
    FACTORY_FOR = models.SectionPeriod

    period = factory.LazyAttribute(lambda s: PeriodFactory())
    section = factory.LazyAttribute(lambda s: SectionFactory())
    semester = factory.LazyAttribute(lambda s: SemesterFactory())
    instructor = u'Goldschmit'
    location = u'DCC 1337'
    kind = u'LEC'


class SemesterDepartmentFactory(factory.Factory):
    FACTORY_FOR = models.SemesterDepartment

    department = factory.LazyAttribute(lambda s: DepartmentFactory())
    semester = factory.LazyAttribute(lambda s: SemesterFactory())
