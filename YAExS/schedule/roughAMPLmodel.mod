# This is an AMPL-format .mod file. In AMPL it would be used in conjunction  
# with a .dat file holding the data (the entries in the sets)
#
# I THINK WE SHOULD SWITCH TO ZIMPL

# I havent added a framework for exam preferences yet



# PSEUDO-MODEL:

# MINIMIZE
# students with 2 or 3 exams in one day
# instructors with 2 or 3 in one day (?)
# penalty for instructor preferences

# SUBJECT TO 
# no student with 2 exams at same time slot
# no instructor with 2 exams at same time slot
# no student with 4 exams in same day
# no instructor with 4 exams in one day (?)
# each exam meets at exactly one time slot
# every student attends all their exams
# every instructor attends all their exams
# time slots set up 4 per day, over 5 days (maybe make this variable)


# thoughts about instructors:

#can just treat instructors exactly the same as students. except perhaps they do not want finals back
#to back incase they have students that need extra time.
#instructor stuff could get tricky because what if Goldschmidt, e.g. is listed for 6 classes even though only teaching 2.
#what about when instructor is listed as Staff?
#prof preferences: we can probably just make everyhing and then do rotations to fulfill these....maybe. 

# preferences go in objective, requirements are constraints:

# instructors may have room preferences / room spacing preferences (dont care about that here, probably)
# instructors may have date preferences OR requirements
# instructors may have time of day preferences OR requirements
# instructors may have back-to-back preferences OR requirements (e.g. want back-to-back, cant have back-to-back)


# AMPL-STYLE MODEL: 

# SETS
set PEOPLE;         			# List of students AND INSTRUCTORS
# set INSTRUCTORS within PEOPLE; # List of instructors, a subset of PEOPLE
set EXAMS {PEOPLE} ordered;  	# For each member p of PEOPLE, there is a set of exams the person has (gives/takes)
set TOTALEX=union {p in PEOPLE} EXAMS[p];          # All exams offered
set TSLOT ordered;         		# All timeslots available for exams (e.g. 5 days at 4 per day = 20 tslots)
set DAYS;          		 	 	# All days available to schedule exams (e.g. 5)
set DAYSLOT {DAYS};   			# For each day, lists the timeslots. (e.g. 4 per day)

# VARIABLES

var examIsAt {e in TOTALEX,t in TSLOT} binary;      # Binary variable, equals 1 if exam e is at time t, 0 otherwise.

var twoPlus {p in PEOPLE} binary; 			  # Binary variable, equals 1 if person p has TWO OR MORE exams on any day, 0 otherwise.

var threePlus {p in PEOPLE} binary;  		  # Binary variable, equals 1 if person p THREE OR MORE exams on any, 0 otherwise.

var conflict {t in TSLOT, p in PEOPLE} >= 0;  # conflict[t,p]=0 if person p has no conflicts at time t (zero or one exams)
	# and conflict[t,p] will be set to - 1 + (number of exams person p has at time t) otherwise when optimizing.

 
# OBJECTIVE
minimize badConflicts: 
sum {p in PEOPLE} threePlus[p]  +  sum{t in TSLOT,p in PEOPLE} conflict[t,p]; 
# need to add in instructor PREFERENCES


# CONSTRAINTS
subject to time{i in TOTALEX}: sum {t in TSLOT} examIsAt [e,t] = 1;     
# Each exam must use exactly one time slot.


subject to penalizeConflicts{t in TSLOT, p in PEOPLE}: -conflict[t,p] + sum {e in EXAMS[p]} examIsAt[e,t] <= 1;
# If person p is scheduled for one or fewer exams at time t, then conflict[t,p] = 0
# If person p is scheduled for more than one exam at time t, then when optimizing conflict[t,p] = - 1 + (that number of exams)


subject to overload {p in PEOPLE, d in DAYS}: sum {e in EXAMS[p], t in DAYSLOT[d]} examIsAt [e,t] - twoPlus[p] - threePlus[p] <= 1;
# Constraint to set the twoPlus and threePlus variables when a person has multiple exams on one day
# If person p has one exam each day, twoPlus[p] and threePlus[p] will be zero.
# If person p has a day with two exams, optimizing will set twoPlus[p] = 1 and threePlus[p] = 0. 
# If person p has a day with three exams, twoPlus[p] and threePlus[p] will be 1.
# No person is allowed to have four exams on one day.


#subject to instructorTimeConstraints {i in INSTRUCTORS, e in EXAMS[i]}  examIsAt[e,t] #something 
