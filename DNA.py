from sys import argv, exit

def repeatCount(STR,inputSeq):
    if(STR==inputSeq[0:len(STR)]):
        count = 1 + repeatCount(STR,inputSeq[len(STR):])
    else:
        count = 0
    return count;


if(len(argv)!=3):
    print("Please input a database and a sequence")
    exit(1)

#load dictionary
database = open(argv[1])
sequence = open(argv[2])
dna = sequence.read()

firstLine = database.readline()

strList = firstLine.split(",")
strList[-1] = strList[-1].rstrip()
count_dict={}

for x in strList[1::]:
    maxRepeats=0;
    for c in range(0,len(dna)):
        count = repeatCount(x,dna[c:])
        if count > maxRepeats:
            maxRepeats = count
   # print(f"Repeats: {maxRepeats}, STR: {x}, C:{c}")
    count_dict[x]=maxRepeats

for line in database:
    valList = line.split(",")
    output = valList[0]
    for strIndex in range(1,len(valList)):
#        print(f"ValList: {valList[strIndex]}, Dict: {count_dict[strList[strIndex]]}, STR: {strList[strIndex]}")
        if(int(valList[strIndex])!=count_dict[strList[strIndex]]):
            output="No Match"
    if (output == valList[0]):
        print(output)
        exit(0)

print(output)



