# TODO
import csv
import sys
import cs50

if(len(sys.argv)!=2):
    print("Please input a CSV")
    exit(1)

print(sys.argv)

db = cs50.SQL("sqlite:///students.db")

with open(sys.argv[1],"r") as characters:
    reader = csv.DictReader(characters, delimiter=",");
    for row in reader:
        nameList = row['name'].split()
        firstName = nameList[0]
        middleName = None
        if len(nameList) == 2:
            lastName = nameList[1]
        elif len(nameList) == 3:
            lastName = nameList[2]
            middleName=nameList[1]

        db.execute(f"INSERT into students (first,middle,last,house,birth) VALUES(?,?,?,?,?)",firstName,middleName,lastName,row['house'],row['birth'])
