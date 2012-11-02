#
#  main.c
#  Final Exams Schedule OBJECTIVE COMPLETELY CHANGED
#
#  Created by Michael Maxwell on 1/24/12.
#  Revised by Vera Axelrod and Katie Sebastian, starting 3/31/12
#  Copyright (c) 2012 Rensselaer Polytechnic Institute. All rights reserved.
#

set STDNT;         # List of students
set EXAMS {STDNT} ordered;  # For each member s of STDNT, there is a set of exams the STDNT takes
set TOTALEX=union {i in STDNT} EXAMS[i];          # All exams offered
set TSLOT ordered;          # All timeslots available for exams
set DAYS;           # All days available to schedule exams
set DAYSLOT {DAYS};    #For each day, lists the timeslots.

param numStudents {TOTALEX};      #Number of students per exam, taking that exam.

var X {i in TOTALEX,t in TSLOT} binary;      #Binary variable, equals 1 if exam i is at time t, 0 otherwise.

var Y {s in STDNT} binary;  # Binary variable, equals 1 if student s takes more than ONE exam on any day, 0 otherwise.

var Z {s in STDNT} binary;  # Binary variable, equals 1 if student s takes more than TWO exams on any, 0 otherwise.

var w{t in TSLOT, s in STDNT} >= 0;  #w[t,s]=0 if student s has no conflicts at time t (zero or one exams)
	# and w[t,s] will be set to - 1 + (number of exams student s takes at time t) otherwise when optimizing.

 
minimize Tests_Taken:
sum {s in STDNT} Z[s]  +  sum{t in TSLOT,s in STDNT} w[t,s]; 


subject to Time{i in TOTALEX}: sum {t in TSLOT} X [i,t] = 1;     
#Each exam must use exactly one time slot.


subject to PenalizeConflicts{t in TSLOT, s in STDNT}: -w [t,s]+sum {i in EXAMS[s]} X[i,t] <= 1;
#If student s is scheduled one or fewer exams at time t, then w[t,s] = 0
#If student s is scheduled for more than one exam at time t, then when optimizing w[t,s] = - 1 + (that number of exams)


subject to Overload {s in STDNT, d in DAYS}: sum {i in EXAMS[s], t in DAYSLOT[d]} X [i,t] - Y[s] - Z[s] <= 1;
#Constraint to set the Y and Z variables when a student has multiple exams on one day
#If student s has one exam each day, Y[s] and Z[s] will be zero.
#If student s has a day with two exams, optimizing will set Y[s] = 1 and Z[s] = 0. 
#If student s has a day with three exams, Y[s] and Z[s] will be 1.
#No student is allowed to have four exams on one day.