#!/usr/bin/python

import sys
import getopt


def main(argv):
    inputfile = ''
    outputfile = ''
    try:
        opts, args = getopt.getopt(argv, "hi:o:", ["ifile=", "ofile="])
    except getopt.GetoptError:
        print 'test.py -i <inputfile> -o <outputfile>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'test.py -i <inputfile> -o <outputfile>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-o", "--ofile"):
            outputfile = arg
    print 'Input file is "', inputfile
    print 'Output file is "', outputfile
    with open(inputfile) as f:
        lines = f.readlines()
        include_line = -1
        myMain_line = -1
        for line, i in zip(lines, range(len(lines))):
            # print(line)
            if '#include \"main.h\"' in line:
                print(line)
                print(i)
                include_line = i
            if '/* USER CODE BEGIN 2 */' in line:
                print(line)
                print(i)
                myMain_line = i

        lines.insert(myMain_line+1, "\tmyMain();\r\n")
        lines.insert(include_line+1, "#include \"myMain.h\"\r\n")

        f.close()

        with open(outputfile, "w") as of:
            for line in lines:
                of.write(line)
            of.close()


if __name__ == "__main__":
    main(sys.argv[1:])
