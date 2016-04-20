import sys
import matplotlib.pyplot as plt

four_mb = 4194304 


def extract_number(file_name):
  file = open(file_name)
  data = file.read()


  warm_up = []
  write_existing = []
  rw_existing = []


  tests = data.split("writeTest application\n")
  print "len"+str(len(tests))
  for i in xrange(0,4900,100):
      print i/100+2 
      test = tests[i/100+2]
      test_lines = test.split("\n")
      wu_words = test_lines[1].split(" ")
      we_words = test_lines[3].split(" ")
      re_words = test_lines[6].split(" ")
      wu = float(wu_words[5])
    #  print "we words: " + we_words
  #    print "we words 5: " +  we_words[5]
      we = float(we_words[5])
    #  print "re words: " + " ".join(re_words)
   #   print len(re_words)
      re = float(re_words[5])
      warm_up.append(wu)
      write_existing.append(we)
      rw_existing.append(re)
  
  print warm_up
  print write_existing
  print rw_existing
  return warm_up, write_existing, rw_existing 
  
  
file_sizes = range(four_b - 8 * 10, four_mb + 8 * 10, 8)
bandwidths = four_mb * range(0, 5000, 100)

bandwidth_wu_curve = [] for i in len(bandwidths)
bandwidth_we_curve = [] for i in len(bandwidths)
bandwidth_re_curve = [] for i in len(bandwidths)

result_file_names = [result_fixed_bandwidth_ + str(bw) + '.txt' for bw in bandwidths]

for bw_idx, file_name in enumerate(result_file_names): 
  wu, we, rw = extract_number(file_name) # for each file size, we extract the number, result is three arrays, each one is the xx result on that bandwidth
  for i in xrange(len(file_sizes)):
      bandwidth_wu_curve[bw_index].append(wu[i])
      bandwidth_we_curve[bw_index].append(we[i])
      bandwidth_re_curve[bw_index].append(rw[i])
      

# try to do a wu
fig_wu = plt.figrue()
fig_wu.subtitle("warm up")
ax1.fig_wu.add_subplot(111)
sz0 = ax1.plot(bandwidths, [wu[0] for wu in bandwidth_wu_curve], 'r--', label='sz0')
sz1 = ax1.plot(bandwidths, [wu[1] for wu in bandwidth_wu_curve], 'r--', label='sz1')
sz2 = ax1.plot(bandwidths, [wu[2] for wu in bandwidth_wu_curve], 'r--', label='sz2')
sz3 = ax1.plot(bandwidths, [wu[3] for wu in bandwidth_wu_curve], 'r--', label='sz3')
sz4 = ax1.plot(bandwidths, [wu[4] for wu in bandwidth_wu_curve], 'r--', label='sz4')
sz5 = ax1.plot(bandwidths, [wu[5] for wu in bandwidth_wu_curve], 'r--', label='sz5')
sz6 = ax1.plot(bandwidths, [wu[6] for wu in bandwidth_wu_curve], 'r--', label='sz6')
sz7 = ax1.plot(bandwidths, [wu[7] for wu in bandwidth_wu_curve], 'r--', label='sz7')
sz8 = ax1.plot(bandwidths, [wu[8] for wu in bandwidth_wu_curve], 'r--', label='sz8')
sz9 = ax1.plot(bandwidths, [wu[9] for wu in bandwidth_wu_curve], 'r--', label='sz9')
sz10 = ax1.plot(bandwidths, [wu[10] for wu in bandwidth_wu_curve], 'r--', label='sz10')
sz11 = ax1.plot(bandwidths, [wu[11] for wu in bandwidth_wu_curve], 'r--', label='sz11')
sz12 = ax1.plot(bandwidths, [wu[12] for wu in bandwidth_wu_curve], 'r--', label='sz12')
sz13 = ax1.plot(bandwidths, [wu[13] for wu in bandwidth_wu_curve], 'r--', label='sz13')
sz14 = ax1.plot(bandwidths, [wu[14] for wu in bandwidth_wu_curve], 'r--', label='sz14')
sz15 = ax1.plot(bandwidths, [wu[15] for wu in bandwidth_wu_curve], 'r--', label='sz15')
sz16 = ax1.plot(bandwidths, [wu[16] for wu in bandwidth_wu_curve], 'r--', label='sz16')
sz17 = ax1.plot(bandwidths, [wu[17] for wu in bandwidth_wu_curve], 'r--', label='sz17')
sz19 = ax1.plot(bandwidths, [wu[18] for wu in bandwidth_wu_curve], 'r--', label='sz18')
sz20 = ax1.plot(bandwidths, [wu[19] for wu in bandwidth_wu_curve], 'r--', label='sz19')

plt.legend(handles=[sz0,sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8,sz9,sz10,sz11,sz12,sz13,sz14,sz15,sz16,sz17,sz18,sz19], loc=(0,0.8))


#wu3, we3, rw3 = extract_number('result_3mb.txt')
#u4, we4, rw4 = extract_number('result_4mb.txt')
#wu5, we5, rw5 = extract_number('result_5mb.txt')
#wu6, we6, rw6 = extract_number('result_6mb.txt')
#fig1 = plt.figure()
#fig2 = plt.figure()
#fig3 = plt.figure()
#fig1.suptitle("warm up")
#fig2.suptitle("write existing")
#fig3.suptitle("rw existing")

#ax1=fig1.add_subplot(111)
#wu3l,=ax1.plot(range(100,5000,100), wu3, 'r--',label='3mb')
#wu4l,=ax1.plot(range(100,5000,100), wu4, 'g--',label='4mb')
#wu5l,=ax1.plot(range(100,5000,100), wu5, 'b--',label='5mb')
#wu6l,=ax1.plot(range(100,5000,100), wu6, 'k--',label='6mb')

#plt.legend(handles=[wu3l,wu4l,wu5l,wu6l], loc=(0,0.8))
#ax2=fig2.add_subplot(111)
#we3l,=ax2.plot(range(100,5000,100), we3, 'r--',label='3mb')
#we4l,=ax2.plot(range(100,5000,100), we4, 'g--',label='4mb')
#we5l,=ax2.plot(range(100,5000,100), we5, 'b--',label='5mb')
#we6l,=ax2.plot(range(100,5000,100), we6, 'k--',label='6mb')
#plt.legend(handles=[we3l,we4l,we5l,we6l],loc=(0,0.8))
#ax3=fig3.add_subplot(111)
#rw3l,=ax3.plot(range(100,5000,100), rw3, 'r--',label='3mb')
#rw4l,=ax3.plot(range(100,5000,100), rw4, 'g--',label='4mb')
#rw5l,=ax3.plot(range(100,5000,100), rw5, 'b--',label='5mb')
#rw6l,=ax3.plot(range(100,5000,100), rw6, 'k--',label='6mb')
#plt.legend(handles=[rw3l,rw4l,rw5l, rw6l],loc=(0,0.8))
plt.show()