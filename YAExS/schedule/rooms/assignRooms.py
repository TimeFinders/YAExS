# A old python parser to take in ampl results, assign rooms, and print create a human readable schedule.


# Usage: python assignRooms.py <exams file> <AMPL results X file> <students file> <rooms file> <grouped rooms file> <number of exam slots> <output file>
# author: Vera Axelrod

import sys
import re


exams = {}
sections = {}
crns = {}
counts = {}
rooms = {}
assignments = {}
names = {}

#parses the results output by AMPL into exams dictionary
def parseResults(results):

    timeslots = None
    for line in open(results, 'r'):
        if re.search(":\s.*:=", line) is not None:
            timeslots = line.split()[1:-1]
        elif re.search("\w{8}.*", line) is not None:
            line = line.split()
            i = 0
            for j in line[1:]:
                if int(j) == 1:
                    exams[line[0]] = timeslots[i]
                    break
                else:
                    i += 1


#parses the exams input file into exams.
#file format should be: <department code (CSCI)>,<course number (2010)>,[list of crn's for this exam separated by whitespace], [whitespace separated list of sections].
#please note that the file MUST be SORTED by department code THEN course number.
def parseExams(examInfo):
    global sections, counts

    #list used to check for courses with the same code/number but with different exams
    similar = []

    #loop over all exams
    for line in open(examInfo, 'r'):

         #split up csv tuple
        i = line.split(',')

        #select first code/number from file (used to select unique name for cross listed courses)
        # example tmp: CSCI4800
        tmp = i[0].split('-')[0] + i[1].split('-')[0].split('/')[0]

        #collect various course name combinations to output all names for an exam
        temp = []
        for name in zip(i[0].split('-'), i[1].split('-')):
            temp.append(name[0] + name[1])

        names[tmp] = ' '.join(temp)

        #if similar is empty add this course
        if len(similar) == 0:
            similar.append(tmp)
        # check if same course name has been seen before
        elif similar[0][0:8] == tmp:

            #counter to append letter to course name to identify sections with separate exams
            #initialized to @ because next ASCII character is 'A' (first used add-on)
            counter = '@'

            #incriment the counter for all previous sections
            for j in similar:
                counter = chr(ord(counter) + 1)
            tmp += counter

            #add this course to the list
            similar.append(tmp)

        #if this course does not match - empty the list and add the current one
        else:
            del similar[:]
            similar.append(tmp)

        for s in i[2].split():
            crns[s] = tmp

        #create an entry in sections correlating a unique exam/class code with the sections for that class
        sections[tmp] = i[3].strip()
        counts[tmp] = 0

#parse student file to set up counts (number of students per exam)
def parseStudents(studentInfo):

    #loop over all tuples
    for line in open(studentInfo, 'r'):

        #split csv tuple into list
        i = line.strip().split(',')

        #if the course (CRN) has an exam add it.  We only care about courses with exams
        if i[1] in crns:
            name = crns[i[1]]
            counts[name] += 1

#assigns rooms based on the number of students taking a given exam
#room file contains info about each room in the format: <building>.<room number>.<capacity>
#groupRoomFile contains info about the room groupings.  This should be in the form <building room>,<building room> etc.
# slots is the number of time slots for this exam period
def assignRooms(roomFile, groupRoomFile, slots):

    #temporary variables used in calculation
    capacity = {}
    groups = {}
    tempRooms = []

    #read in the rooms
    for line in open(roomFile, 'r'):
        i = line.strip().split(',')
        tempRooms.append((i[0] + " " + i[1], int(i[2])))
        capacity[i[0] + " " + i[1]] = int(i[2])

    #read in the rooms and grouped rooms and sort both lists by size
    tempGroups = sorted(group(groupRoomFile, capacity), key=lambda t: t[1], reverse=True)
    tempRooms = sorted(tempRooms, key=lambda t: t[1], reverse=True)

    #construct dictionaries to keep track of the rooms not used in a timeslot
    for i in range(1, slots + 1):
        rooms['T' + str(i)] = list(tempRooms)
        groups['T' + str(i)] = list(tempGroups)

    #for each exam (biggest first)
    for exam in sorted(counts.items(), key=lambda t: t[1], reverse=True):
        
        #if there are no students in the exam do nothing
        if exam[1] <= 0:
            continue

        #get the groups  and rooms left for this slot
        tempRooms = rooms[exams[exam[0]]]
        tempGroups = groups[exams[exam[0]]]

        #first match the smallest possible room
        i = 0
        while i < len(tempRooms):
            if exam[1] > tempRooms[i][1]:
                break
            i += 1

        #if not possible match the smallest group
        if i == 0:
            while i < len(tempGroups):
                if exam[1] > tempGroups[i][1]:
                    break
                i += 1

            #remember the assigned group
            assignments[exam[0]] = tempGroups[i - 1][0]

            #we can no longer use the groups
            del tempGroups[i - 1]

        else:

            #remember the assigned room
            assignments[exam[0]] = tempRooms[i - 1][0]

            #can no longer use that room
            del tempRooms[i - 1]

        #ensure that no rooms/groups are left that use the room we just assigned
        removeDups(assignments[exam[0]], tempRooms, tempGroups)


#reads in the group rooms file from a file.  See assignRooms for file format
def group(groupFile, capacity):

    groups = []

    #read in the groups and calculate capacity
    for line in open(groupFile, 'r'):
        group = line.strip().split(',')
        cap = 0

        for room in group:
            cap += capacity[room]

        #create tuples for the groups
        groups.append((' '.join(group), cap))

    return groups


#removes used rooms/groups from a timeslot
def removeDups(room, tempRooms, tempGroups):

    for r in room.split():
        for p in tempRooms:
            if r == p[0]:
                tempRooms.remove(p)

        for p in tempGroups:
            if p[0].find(r) > 0:
                tempGroups.remove(p)


#outputs all relevant information about the exam schedule
def output(outputfile):

    with open(outputfile, 'w') as f:

        f.write("Course, Section(s), Timeslot, Room(s)\n")
        #print out all exams/sections/times
        for key, val in sorted(exams.iteritems()):
            f.write(names[key[:8]] + "," + sections.pop(key) + "," + val + "," + assignments[key] + "\n")

        #exams with no students are output for informational purposes
        f.write("\n\nEXAMS WITH NO STUDENTS WERE NOT ASSIGNED TIMES. THEY ARE:\n")

        for key, val in sorted(sections.iteritems()):
            f.write(names[key[:8]] + "," + val + "\n")

parseExams(sys.argv[1])
parseResults(sys.argv[2])
parseStudents(sys.argv[3])
assignRooms(sys.argv[4], sys.argv[5], int(sys.argv[6]))
output(sys.argv[7])
