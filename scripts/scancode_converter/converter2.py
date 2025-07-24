import csv

output = open("output", "w")

with open('keycode_scancode.csv', mode='r') as file:
    csvFile = csv.reader(file)

    for lines in csvFile:
        output.write(f"{{ {lines[0]}, {lines[1]} }},\n")