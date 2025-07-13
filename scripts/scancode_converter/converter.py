import csv

ps2_out = open("ps2_out.h", "w")
keycode_template = open("keycode_template.h", "w")

extended = []

with open('scan_code_set2.csv', mode='r') as file:
    csvFile = csv.reader(file)

    for lines in csvFile:
        name = f"#define PS2_SCANCODE_{lines[0].replace(" ", "")}"

        if("E0," in lines[1]):
            extended.append(lines[0].replace(" ", ""))

        scancode = f"0x{lines[1].replace("E0,", "")}"
        ps2_out.write(f"{name:<40} {scancode}\n")

        template_name = f"#define KEYCODE_{lines[0].replace(" ", "")}"
        keycode_template.write(f"{template_name:<40} 0x\n")
    
    print (extended)