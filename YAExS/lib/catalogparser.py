"""
Copyright (c) 2012, Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz, Auston Sterling
All rights reserved.
  
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
  
The names Vera Axelrod, Andrew Karnani, Jeffrey Rodowicz and Auston Sterling may not be used to endorse or promote products derived from this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  
"""
import urllib2
import re
from BeautifulSoup import BeautifulSoup
from rpi_courses.config import DEPARTMENTS


def load_page(url, data=None):
    # load a page given the url, do not include the "http://" as it is already included.
    try:
        website = urllib2.urlopen("http://" + url, data)
        return website.read()
    except urllib2.URLError, e:
        print "Could not retrieve catalog url because:", e.reason[1]


def get_catalogs(frontpage):
    # get the list of catalogs of all the available years from the front page
    ids = re.findall('<option value="\d+"\s*\w*>Rensselaer Catalog', frontpage)
    out = []
    for i in ids:
        out.append(re.search("\d+", i).group(0))
    return out


def get_courses_link_id(page):
    # get the link id to all the courses for a catalog page
    link = re.search('\d+" class="navbar" \w+="\d+">Courses</a>', page)
    return re.search("\d+", link.group(0)).group(0)


def get_course_ids(department_page):
    ids = re.findall('coid=\d+"', department_page)
    out = []
    for i in ids:
        out.append(re.search('\d+', i).group(0))
    return out


def get_course_detail(course_page):
    course_page = re.sub('<br */?>', '\n', course_page)
    soup = BeautifulSoup(course_page, convertEntities=BeautifulSoup.HTML_ENTITIES)
    title_text = soup.findAll('h1 h2 h3 h4 h5 h6'.split(' '))[0].text
    title = re.search('([\w+\s]+) (\d+\w+) \- (.*)', title_text)
    course = {
        'department': title.group(1),
        'num': title.group(2),
        'title': title.group(3),
        'description': get_course_description(soup.findAll('hr')[0].nextSibling),
        'prereqs': get_course_reqs(soup)
    }
    #print course['title']
    #print course['description']
    return course


def get_course_description(tag):
    current = tag
    contents = []
    while current and getattr(current, 'name', 'text') not in ('em', 'strong', 'p'):
        contents.append(getattr(current, 'text', current.string))
        current = current.nextSibling
    return ''.join(contents).strip()


def get_course_reqs(tag):
    if (tag.findAll('strong')):
        current = tag.findAll('strong')[0]
        if getattr(current, "text", current.string) == 'Prerequisites/Corequisites:':
            return ''.join(current.nextSibling).strip()
    return 'None'


def special(tags):
    contents = re.findall('>??(.*?)<.*?>', tags)
    return "".join(contents)


def parse_catalog(a=False):
    courses = {}
    url = "catalog.rpi.edu"
    ids = get_catalogs(load_page(url))
    if a:
        catalogs = len(ids)
    else:
        catalogs = 1
    for i in range(catalogs):
        catalog_url = url + "/index.php?catoid=" + ids[i]
        link_id = get_courses_link_id(load_page(catalog_url))
        courses_url = url + "/content.php?catoid=" + ids[i] + "&navoid=" + link_id

        # parse need to parse out the coid (course id) from each department list of courses
        # then use it in the url: http://catalog.rpi.edu/preview_course.php?catoid=<id>&navoid<link_id>&coid=<course>
        # this will bring up the course descriptions and info and only the info for that course.
        for e in DEPARTMENTS.keys():
            print "parsing", e
            course_id = get_course_ids(load_page(courses_url, "filter[27]=" + e))
            for c in range(0, len(course_id)):
                detail_url = url + "/preview_course.php?catoid=" + ids[i] + "&coid=" + course_id[c]
                temp = get_course_detail(load_page(detail_url))
                key = temp['department'] + temp['num']
                if (key not in courses or temp['description'].strip() != '') and re.search('Topics in', temp['title']) == None:
                    courses[key] = temp
    return courses
