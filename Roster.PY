# TODO
import sys
import cs50

hp_houses = {'Ravenclaw','Gryffindor','Hufflepuff','Slytherin'}

if len(sys.argv) != 2:
    print("Please enter 1 command-line argument")
    sys.exit(1)
    
house = (sys.argv[1]).title()

if house not in hp_houses:
    print("Not a Harry Potter House")
    exit(1)

    
db = cs50.SQL("sqlite:///students.db")

output = db.execute(f"Select first,middle,last,Birth from students where house = ? order by last,first",house)

for row in output:
    if row['middle'] is None:
        print(f"{row['first']} {row['last']}, born {row['birth']}") 
    else:
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}") 
