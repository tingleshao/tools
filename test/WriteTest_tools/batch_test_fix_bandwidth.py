# batch execute system commands 
# this is a fixed bandwidth batch test 
# bandwidth = file size * number of files 
# no need to modify the code. 

import os 


four_mb = 4194304

for n in xrange(0, 200, 100):
    bandwidth = four_mb * n 
    for file_size in xrange(four_mb - 8 * 10, four_mb + 8 * 10, 8):
          number_of_files = bandwidth / file_size
          command = './writetest -d ~/SSD/drive2/Test/fstest/ -n ' + str(number_of_files) + ' -s ' + str(file_size) + ' >> result.txt' 
          print " current file size: " + str(file_size)
          print " current number of files: " + str(number_of_files)
          os.system(command) 
    with opend("result.txt", "a") as result:
        result.write("\n\n===========finished one bandwidth test\n\n")
        
