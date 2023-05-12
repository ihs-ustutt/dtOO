import glob

f = open("swig_includeAll.i", "w")
for i in glob.glob("xml/classdtOO_1_1*.i"):
    f.write("%%include %s\n" % i)