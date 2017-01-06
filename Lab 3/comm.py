#!/usr/bin/python

"""
Name: Nathan Kong
UID:  204 401 093
TA:   Aishawarya Kamal
"""

import random, sys, locale, string
from optparse import OptionParser

class comm:
    def __init__(self, file1, file2):
        if file1 != "-":
            f1 = open(file1, 'r');
        else:
            f1 = sys.stdin
        self.content1 = f1.readlines()
        f1.close()

        if file2 != "-":
            f2 = open(file2, 'r');
        else:
            f2 = sys.stdin
        self.content2 = f2.readlines()
        f2.close()

    def getColumn1(self):
        return self.content1

    def getColumn2(self):
        return self.content2

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION] ... FILE1 FILE2

Compares files FILE1 and FILE2.
Display unique lines seen in FILE1, FILE2 and both in columns
1, 2, 3 respectively.
"""

    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", "--suppress1",
                      action="store_true", 
                      dest="suppress1", 
                      default=0,
                      help="Suppress column 1")
    parser.add_option("-2", "--suppress2",
                      action="store_true", 
                      dest="suppress2", 
                      default=0,
                      help="Suppress column 2")
    parser.add_option("-3", "--suppress3_true",
                      action="store_true", 
                      dest="suppress3", 
                      default=0,
                      help="Suppress column 3")
    parser.add_option("-u", "--unsorted_true",
                      action="store_true", 
                      dest="unsorted", 
                      default=0,
                      help="Unsorted files, FILE1 will take precedence.")
    (options, args) = parser.parse_args()

    option1 = options.suppress1
    option2 = options.suppress2
    option3 = options.suppress3
    option4 = options.unsorted

    if len(args) != 2:
        if len(args) == 0:
            parser.error("Missing operand")
        elif len(args) == 1:
            parser.error("Missing operand after {0}".format(args[0]))
        elif len(args) > 2:
            parser.error("Too many operands.")

    input_file1 = args[0]
    input_file2 = args[1]

    try:
        generator = comm(input_file1, input_file2)
        column1 = generator.getColumn1()
        column2 = generator.getColumn2()
    except IOError as err:
        parser.error("I/O error({0}): (1)".
                     format(errno, strerror))

    column1[len(column1) - 1] += "\n"
    column2[len(column2) - 1] += "\n"
    final_result = ""

    #When -u is not chosen
    if not options.unsorted:
        i, j = 0, 0

        #Compare the two words with one another, then decide which comes
        #first, increment the counter, repeat.
        while i < len(column1) and j < len(column2):
            if column1[i] < column2[j]:
                if not option1:
                    final_result += column1[i]
                i += 1
            elif column1[i] > column2[j]:
                if not option2:               
                    final_result += "\t" + column2[j]
                j += 1
            elif column1[i] == column2[j]:
                if not option3:
                    final_result += "\t\t" + column1[i]
                i += 1
                j += 1
        
        if i < len(column1):
            while i < len(column1):
                if not option1:
                    final_result += column1[i]
                i += 1

        if j < len(column2):
            while j < len(column2):
                if not option2:
                    final_result += "\t" + column2[j]
                j += 1
                    
    #The -u option is chosen
    else:
        i, j, k, flag = 0, 0, 0, False
        
        #The idea here is to compared FILE1 lines with FILE2, any matches with
        #FILE2 should be deleted in case we see another instance of the same
        #word in FILE1 which should NOT be paired with the same instance in
        #FILE2.
        while i < len(column1):
            while j < len(column2):
                if column1[i] == column2[j]:
                    flag = True
                    if not option3:
                        final_result += "\t\t" + column1[i]
                    del column2[j]
                    i += 1
                    j = 0
                    break
                else:
                    j += 1
            if not flag:
                if not option1:
                    final_result += column1[i]
                i += 1
                j = 0
            flag = False

        #The last line within each file does not have a new line char
        #final_result += "\n"
        
        #Write the remainder of FILE2
        if not option2:
            while k < len(column2):
                final_result += "\t" + column2[k]
                k += 1

    #Prints results
    sys.stdout.write(final_result)
if __name__ == "__main__":
    main()
