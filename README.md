# Examination Timetabling
Repository for OMA assignment 2017-2018 for the degree course in **Computer Engineer** of *Polytechnic of Turin*.

## Assignment
Let us consider a set **E** of exams, to be scheduled during an examination period at the end of the semester, and a set **S** of students. Each student is enrolled in a subset of exams, at least one. The examination period is divided in t_max ordered time-slots.

By law, conﬂicting exams (i.e. having enrolled students in common) cannot take place during the same time-slot. Moreover, to incentive universities at creating timetables more sustainable for the students, the Ministry of Education assigns a penalty for each couple of conﬂicting exams scheduled up to 5 time-slots apart. More precisely, given the number **N_(e,e')** of students enrolled in both conﬂicting exams e and e', which are scheduled i time-slots apart, the penalty is calculated as
**2^(5-i)*N_(e,e)/abs(S)**.

The Examination Timetabling Problem (ETP) aims at assigning each exam to a speciﬁc time-slot ensuring that:
* Each exam is scheduled once during the period,
* Two conﬂicting exams are not scheduled in the same time-slot.

The objective is to minimize the total penalty resulting from the created timetable.

**Assumptions**:
* During each time-slot there is always a number of available rooms greater than the total number of exams;
* Rooms have enough capacity with respect to the number of enrolled students.

## Mathematical model
The model is described in a `OMA_ETT-Model.pdf` document stored in the Latex folder of the project.

## Project Report
The final report of the project is on a file named `ETPreport_OMAMZ_group03.pdf` in the report folder.

## Required tasks
* Provide a Linear Programming formulation for the ETP (deadline 1).
* Propose a solution approach for the ETP by exploiting one (or more) heuristic and meta-heuristic algorithms presented during the course.
* Develop/implement such a solution algorithm through a know programming language (preferably C/C++ or Java).
* Solve the benchmark instances through the implemented algorithm.
* Deliver the project code, the results, and a report of the work (deadline 2)
* Present the solution method adopted and the results obtained (*to be deﬁned within Jan 9–19, 2018, during the last lessons of the course*)

## Deadlines
* Deadline 1 (problem formulation): **26/11/2017**, 23:59.
* Deadline 2 (project code, results, and report): **08/01/2018**, 23:59.

## Check Solution
Using the provided software by professor, is possible to check the solution using this command:
```
ETPchecker.exe instance name -check solution file
```
everything is able and it must work on Windows.

The format of the solution must be like this:
```
INT1 INT2
```
where INT1 is the exam ID and INT2 is the ID of the assigned time-slot (IDs must correspond to those read in the instance ﬁles).

**The solution MUST BE FEASIBLE!**

## Built With

* [C](https://en.wikipedia.org/wiki/C_(programming_language)) - The development language

## Authors
* **Jacopo Maggio** - [JackMGit](https://github.com/JackMGit)
* **Stefano Munna** - [stemunna21](https://github.com/stemunna21)
* **Jacopo Nasi** - [Jacopx](https://github.com/Jacopx)
* **Andrea Santu** - [androsanta](https://github.com/androsanta)
* **Marco Torlaschi** - [Torla](https://github.com/Torla)

## License

The property of this project belongs to the Polytechnic of Turin since this project isn't delivered yet. After that it will be published under not-already defined license.

## MaderFader

* Vediamo di prendere tutti i punti...
* ... anche i due aggiuntivi.
* Pensaci due volte prima di pushare!
