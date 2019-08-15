import os

arr = os.listdir('./test_completi_10000righe/i/')
for i in arr:
    os.system("cat test_completi_10000righe/i/"+i+" | ./main > out.txt")
    os.system("diff out.txt test_completi_10000righe/o/"+i+" -Z >diffRes/outDiff"+i+".txt")
    f = open("diffRes/outDiff"+i+".txt", "r")
    if f.read() == "":
        os.remove("diffRes/outDiff"+i+".txt")