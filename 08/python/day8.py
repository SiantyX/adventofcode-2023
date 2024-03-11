import os
from fractions import gcd

lines = list()
with open("day8.txt", "r") as f:
    lines = f.readlines()

map = lines[0].strip()
print(len(map))
print(map)
print("mapstop")
instr = lines[2:]
instr_dict = { r[:3]: (r[7:10], r[12:15]) for r in instr }

#pos = "AAA"
poses = [k for k in instr_dict.keys() if k[2] == "A"]
print(poses)
j = 0
answers = [0,0,0,0,0,0]
import time
start = time.time()
while True:
    """
    if j % 1000000 == 0 and j != 0:
        end = time.time()
        print(end - start)
        exit()
    """
    if j % len(map) == 0:
        for i in range(len(poses)):
            if poses[i][2] == "Z" and answers[i] == 0:
                print(f"i: {i} j: {j}")
                answers[i] = j
        if all(answers):
            break
    """
    if j % len(map) == 0:
        x = 0
        for i in range(len(poses)):
            if poses[i][2] == "Z":
                x = x + 1;
                #print(f"i: {i} j: {j}")
                #answers[i] = j
        if x == len(poses):
            break
    """
    for i in range(len(poses)):
        if answers[i] != 0:
            continue
        if map[j % len(map)] == "L":
            poses[i] = instr_dict[poses[i]][0]
        elif map[j % len(map)] == "R":
            poses[i] = instr_dict[poses[i]][1]
        else:
            print(f"num: j {j % len(map)}")
            print(f"char :{map[j % len(map)]}")
            exit()
                
    j = j + 1

print(poses)
print(j)
print(answers)
m = 1
for a in answers:
    m = m * a

import math
def lcm(a, b):
    return abs(a*b) // math.gcd(a, b)

result = answers[0]
for i in range(1, len(answers)):
   result = lcm(result, answers[i])

print(result)

#print(a)

