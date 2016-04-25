# batch execute system commands 
# this is a fixed bandwidth batch test 
# bandwidth = file size * number of files 
# no need to modify the code. 

import os 


four_mb = 4194304

for n in xrange(0, 5000, 100):
    bandwidth = four_mb * n 
    for file_size in xrange(four_mb - 8 * 1024, four_mb + 8 * 1024, 1024):
          number_of_files = bandwidth / file_size
          command = './writetest -d ~/SSD/drive2/Test/fstest/ -n ' + str(number_of_files) + ' -s ' + str(file_size) + ' >> fixed_bandwidth_test2/result_fixed_bandwidth_' + str(bandwidth) + '.txt' 
          print " current file size: " + str(file_size)
          print " current number of files: " + str(number_of_files)
          os.system(command) 
    with open("fixed_bandwidth_test2/result_fixed_bandwidth_" + str(bandwidth) + ".txt", "a") as result:
        result.write("\n\n===========finished one bandwidth test\n\n")
        
