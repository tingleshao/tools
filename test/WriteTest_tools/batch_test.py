# batch execute system commands 
import os 

for n in xrange(0,1000,100):
    command = './writetest -d ~/SSD/drive2/Test/fstest/ -n ' + str(n) + ' >> result.txt' 
    print "n = " + str(n)
    os.system(command) 
   
