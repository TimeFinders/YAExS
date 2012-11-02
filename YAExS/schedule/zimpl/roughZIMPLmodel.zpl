# First go at a ZIMPL Model.
# Next steps:
#   Read in data from a file
#   Exam Preferences
#
# 
# To use in SCIP
# SCIP > read roughZIMPLmodel.zpl
# SCIP > optimize

############
#   SETS   #
############

#set PEOPLE := {"Andrew", "Auston", "Jeff", "Vera", "John", "Fengyan", "Sikdar"};
set PEOPLE := { read "people.dat" as "<1s>" comment "#" };
# List of students AND INSTRUCTORS. String identifiers.

#set INSTRUCTORS := {"John", "Fengyan", "Sikdar"};
set INSTRUCTORS := { read "instructors.dat" as "<1s>" comment "#"};
# List of instructors, a subset of PEOPLE
# this may be unnecessary if we do some sort of logical indexing on the set of all people,
# if the instructor string identifiers are somehow different.

set EXAMS[PEOPLE] := <"Andrew"> {read "exams.dat" as "<s+>" use 1 comment "#"},
	<"Auston"> 	{read "exams.dat" as "<s+>" skip 1 use 1 comment "#"},
	<"Jeff"> 	{read "exams.dat" as "<s+>" skip 2 use 1 comment "#"},
	<"Vera"> 	{read "exams.dat" as "<s+>" skip 3 use 1 comment "#"},
	<"John"> 	{read "exams.dat" as "<s+>" skip 4 use 1 comment "#"},
	<"Fengyan"> {read "exams.dat" as "<s+>" skip 5 use 1 comment "#"},
	<"Sikdar">	{read "exams.dat" as "<s+>" skip 6 use 1 comment "#"};

#set EXAMS[PEOPLE] :=  <"Andrew"> {"SDD", "CCN"}, 
#	 <"Auston"> {"SDD"},
#	 <"Jeff"> {"SDD", "CCN", "Bio"}, 
#	 <"Vera"> {"SDD", "CLA", "Bio"},
#	 <"John"> {"SDD", "Bio"}, 
#	 <"Fengyan"> {"CLA"},
#	 <"Sikdar"> {"CCN"} ;
# For each member p of PEOPLE, there is a set of exams the person has (gives/takes)
# I'm not sure how to read this in from a file in a better way

set ALL_EXAMS := union <p> in PEOPLE : EXAMS[p];
# All exams offered. Dont need to have an explicit list, just union over peoples exams.

param numSlots := read "parameters.dat" as "1n" use 1 comment "#";
set TSLOT := { 1..numSlots };         		
# All timeslots available for exams (e.g. 5 days at 4 per day = 20 tslots)

param numDays := read "parameters.dat" as "1n" skip 1 use 1 comment "#";
set DAYS := { 1..numDays };          		 	 	
# All days available to schedule exams (e.g. 1 to 5)

#set DAYSLOT[DAYS] := { read "dayslots.dat" as "<1n> 2n" comment "#"};
set DAYSLOT [DAYS] := <1> {1..2};
#					 <2> {5..8},
#					 <3> {9..12}, <4> {13..16}, <5> {17..20};
# For each day, lists the timeslots. (e.g. 4 per day)
# we should actually read these in from a file or do some logic.
# but I'm not sure how to do this.

#time requirements
set dontMeetAt[ALL_EXAMS] :=  <"SDD"> {1,2}, <"CLA"> {1}, <"CCN"> {}, <"Bio"> {};
#I'm not sure how to read this in from a file.

###############
#  VARIABLES  #
###############

var examIsAt [<e,t> in ALL_EXAMS cross TSLOT] binary; 
# cross is the cartesian product, so e is in ALL_EXAMS, t is in TSLOT
# Binary variable, equals 1 if exam e is at time t, 0 otherwise.

var twoPlus[<p> in PEOPLE] binary;
# Binary variable, equals 1 if person p has TWO OR MORE exams on any day, 0 otherwise.

var threePlus[<p> in PEOPLE] binary;
# Binary variable, equals 1 if person p THREE OR MORE exams on any, 0 otherwise.

var conflict [<t,p> in TSLOT cross PEOPLE] >= 0;  # conflict[t,p]=0 if person p has no conflicts at time t (zero or one exams)
# and conflict[t,p] will be set to - 1 + (number of exams person p has at time t) otherwise when optimizing.
# cross is the cartesian product, so t is in TSLOT, p in PEOPLE

###############
#  OBJECTIVE  #
###############

minimize clashes: 
    sum <p> in PEOPLE : threePlus[p]
 +  sum <t> in TSLOT : (sum<p> in PEOPLE : conflict[t,p])
 +  (1/5) * sum <e> in ALL_EXAMS : ( sum <t> in dontMeetAt[e] : examIsAt[e,t] ) 
 + (1/10) * sum<p> in PEOPLE : twoPlus[p];

#  time preferences (dontMeetAt) are weighted 1/5 as much as conflicts and 
# three exams in one day


#################
#  CONSTRAINTS  #
#################

subto time: 
	forall <e> in ALL_EXAMS do
		sum<t> in TSLOT : examIsAt [e,t] == 1;     

# Each exam must use exactly one time slot.
# Note the double equal sign needed for equality constraint

subto penalizeConflicts:
	forall <t,p> in TSLOT cross PEOPLE do
		 -conflict[t,p] + (sum <e> in EXAMS[p]: examIsAt[e,t]) <= 1;

# If person p is scheduled for one or fewer exams at time t, then conflict[t,p] = 0
# If person p is scheduled for more than one exam at time t, then
#		 when optimizing conflict[t,p] = - 1 + (that number of exams)


subto overload: 
	forall <p,d> in PEOPLE cross DAYS do
		sum <e> in EXAMS[p]: 
			(sum <t> in DAYSLOT[d]: examIsAt [e,t] - twoPlus[p] - threePlus[p]) <= 1;

# Set the twoPlus and threePlus variables when a person has multiple exams on one day
# If person p has one exam each day, twoPlus[p] and threePlus[p] will be zero.
# If person p has a day with two exams, optimizing will set twoPlus[p] = 1 and threePlus[p] = 0. 
# If person p has a day with three exams, twoPlus[p] and threePlus[p] will be 1.
# No person is allowed to have four exams on one day.


#subto instructorTimeConstraints:
#	forall <i,e> in INSTRUCTORS cross EXAMS do
#		examIsAt[e,t] #something 
