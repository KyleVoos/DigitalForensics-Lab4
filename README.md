# DigitalForensics-Lab3
A program for a lab in CS425 Digital Forensics. The program allows the user to modify a binary
file with new data at a specific offset within the file.

## How To Run
./lab3 [offset] [byte(s) to write] [input file]

[offset] and [byte(s) to write] are hex values and work with either a leading 0x
or without it.

## Example of Program Being Run
This is the original binary viewed in a hexeditor.

![](https://github.com/KyleVoos/DigitalForensics-Lab4/blob/master/Images/original_binary.PNG)

$ ./lab3 0x20 0x1122334455 backup  
0x1122334455 successfully written at offset 0x20  

![](https://github.com/KyleVoos/DigitalForensics-Lab4/blob/master/Images/modified_binary.PNG)

$ ./lab3 20 0x22 backup  
0x22 successfully written at offset 0x20  

![](https://github.com/KyleVoos/DigitalForensics-Lab4/blob/master/Images/no0x.PNG)
