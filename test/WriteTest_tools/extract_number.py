import sys
import matplotlib.pyplot as plt


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
  
wu3, we3, rw3 = extract_number('result_3mb.txt')
wu4, we4, rw4 = extract_number('result_4mb.txt')
wu5, we5, rw5 = extract_number('result_5mb.txt')
wu6, we6, rw6 = extract_number('result_6mb.txt')
fig1 = plt.figure()
#fig2 = plt.figure()
#fig3 = plt.figure()
fig1.suptitle("warm up")
#fig2.suptitle("write existing")
#fig3.suptitle("rw existing")

ax1=fig1.add_subplot(111)
wu3l,=ax1.plot(range(100,5000,100), wu3, 'r--',label='3mb')
wu4l,=ax1.plot(range(100,5000,100), wu4, 'g--',label='4mb')
wu5l,=ax1.plot(range(100,5000,100), wu5, 'b--',label='5mb')
wu6l,=ax1.plot(range(100,5000,100), wu6, 'k--',label='6mb')

plt.legend(handles=[wu3l,wu4l,wu5l,wu6l], loc=(0,0.8))
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