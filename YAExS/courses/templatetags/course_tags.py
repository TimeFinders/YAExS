"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
from django import template

from courses.utils import DAYS, ObjectJSONEncoder
from courses.encoder import default_encoder


register = template.Library()


def remove_zero_prefix(timestr):
    if timestr[0] == '0':
        return timestr[1:]
    return timestr


@register.filter
def bold_topics_include(string):
    return string.replace('Topics include', '<strong>Topics include</strong>')


@register.filter
def requires_truncation(string, summary_size):
    tstr = string.lower().strip()
    return len(tstr[:summary_size].strip()) != len(tstr.strip())


@register.filter
def reverse_truncatechars(string, start):
    """Inverse of truncate chars. Instead of the first x characters,
    excludes the first nth characters.
    """
    return string[start:]


@register.filter
def toJSON(obj):
    return ObjectJSONEncoder().encode(default_encoder.encode(obj))


@register.filter
def get(obj, key):
    if obj:
        try:
            return getattr(obj, key)
        except (AttributeError, TypeError):
            pass
        try:
            return obj[key]
        except (KeyError, IndexError):
            pass
    return None


@register.filter
def display_period(period):
    fmt = "%s-%s"
    start_format, end_format = "%I", "%I"
    if period.start.minute != 0:
        start_format += ':%M'
    if period.end.minute != 0:
        end_format += ':%M'
    end_format += " %p"
    return fmt % (
        remove_zero_prefix(period.start.strftime(start_format)).lower(),
        remove_zero_prefix(period.end.strftime(end_format)).lower(),
    )


@register.filter
def sort(iterable):
    return sorted(iterable)


@register.filter
def dow_short(dow):
    if isinstance(dow, list) or isinstance(dow, tuple):
        return tuple(map(dow_short, dow))
    return {
        'Monday': 'Mo',
        'Tuesday': 'Tu',
        'Wednesday': 'We',
        'Thursday': 'Th',
        'Friday': 'Fr',
        'Saturday': 'Sa',
        'Sunday': 'Su',
    }.get(dow)


@register.filter
def period_dow_buckets(periods):
    """Prepares periods by day of the week.
    """
    slots = {}
    for period in periods:
        for dow in period.days_of_week:
            slots.setdefault(dow, []).append(period)
    return slots


@register.filter
def period_type_buckets(periods):
    """Converts periods into buckets of days of the week.

    The periods in each bucket is sorted by start time.
    """
    slots = {}
    for period in periods:
        dows = tuple(period.days_of_week)
        slots[dows[0]] = slots.get(dows[0], {})
        slots[dows[0]][dows] = slots[dows[0]].get(dows, []) + [period]
        #for dow in period.days_of_week:
        #   slots[dow] = slots.get(dow, []) + [period]
    for slot in slots.values():
        for periods in slot.values():
            periods.sort(key=lambda p: p.start)
    return slots


@register.filter
def sections_for(sections, course):
    return [s for s in sections if s.course_id == course.id]


@register.filter
def sum_credits(courses):
    min_creds = max_creds = 0
    for course in courses:
        min_creds += course.min_credits
        max_creds += course.max_credits
    if min_creds == max_creds:
        return "%d credit%s" % (min_creds, '' if min_creds == 1 else 's')
    return "%d - %d credits" % (min_creds, max_creds)


@register.filter
def seats_left(sections):
    return sum(s.seats_left for s in sections)


@register.filter
def join(collection, sep=', '):
    return unicode(sep).join(map(unicode, collection))
