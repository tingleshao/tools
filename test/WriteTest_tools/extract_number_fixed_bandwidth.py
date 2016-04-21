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
#  print "len"+str(len(tests))
  for i in range(len(range(four_mb - 8 * 10, four_mb + 8 * 10, 8))):
 #     print i
      test = tests[i+1]
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
  
#  print warm_up
#  print write_existing
#  print rw_existing
  return warm_up, write_existing, rw_existing 
  
  
file_sizes = range(four_mb - 8 * 10, four_mb + 8 * 10, 8)
bandwidths = [four_mb * i for i in range(100, 5000, 100)]
print bandwidths

bandwidth_wu_curve = [[] for i in xrange(len(bandwidths))]
bandwidth_we_curve = [[] for i in xrange(len(bandwidths))]
bandwidth_re_curve = [[] for i in xrange(len(bandwidths))]

result_file_names = ['result_fixed_bandwidth/result_fixed_bandwidth_' + str(bw) + '.txt' for bw in bandwidths]

for bw_idx, file_name in enumerate(result_file_names): 
  wu, we, rw = extract_number(file_name) # for each file size, we extract the number, result is three arrays, each one is the xx result on that bandwidth
  for i in xrange(len(file_sizes)):
      bandwidth_wu_curve[bw_idx].append(wu[i])
      bandwidth_we_curve[bw_idx].append(we[i])
      bandwidth_re_curve[bw_idx].append(rw[i])
      

# try to do a wu
do_wu = False
do_we = False
do_re = True
xlog = False


if do_wu:
    fig_wu = plt.figure()
    fig_wu.suptitle("warm up")
    ax1 = fig_wu.add_subplot(111)
    if xlog:
        plt.xscale('log')
    sz0, = ax1.plot(bandwidths, [wu[0] for wu in bandwidth_wu_curve], '--', label='sz0', color='#003366')
    sz1, = ax1.plot(bandwidths, [wu[1] for wu in bandwidth_wu_curve], '--', label='sz1', color='#003399')
    sz2, = ax1.plot(bandwidths, [wu[2] for wu in bandwidth_wu_curve], '--', label='sz2', color='#6699ff')
    sz3, = ax1.plot(bandwidths, [wu[3] for wu in bandwidth_wu_curve], '--', label='sz3', color='#6600cc')
    sz4, = ax1.plot(bandwidths, [wu[4] for wu in bandwidth_wu_curve], '--', label='sz4', color='#660066')
    sz5, = ax1.plot(bandwidths, [wu[5] for wu in bandwidth_wu_curve], '--', label='sz5', color='#ff66ff')
    sz6, = ax1.plot(bandwidths, [wu[6] for wu in bandwidth_wu_curve], '--', label='sz6', color='#993366')
    sz7, = ax1.plot(bandwidths, [wu[7] for wu in bandwidth_wu_curve], '--', label='sz7', color='#ff0066')
    sz8, = ax1.plot(bandwidths, [wu[8] for wu in bandwidth_wu_curve], '--', label='sz8', color='#993333')
    sz9, = ax1.plot(bandwidths, [wu[9] for wu in bandwidth_wu_curve], '--', label='sz9', color='#ff9966')
    sz10, = ax1.plot(bandwidths, [wu[10] for wu in bandwidth_wu_curve], '--', label='sz10', color='#cc3300')
    sz11, = ax1.plot(bandwidths, [wu[11] for wu in bandwidth_wu_curve], '--', label='sz11', color='#ffff00')
    sz12, = ax1.plot(bandwidths, [wu[12] for wu in bandwidth_wu_curve], '--', label='sz12', color='#003366')
    sz13, = ax1.plot(bandwidths, [wu[13] for wu in bandwidth_wu_curve], '--', label='sz13', color='#663300')
    sz14, = ax1.plot(bandwidths, [wu[14] for wu in bandwidth_wu_curve], '--', label='sz14', color='#666633')
    sz15, = ax1.plot(bandwidths, [wu[15] for wu in bandwidth_wu_curve], '--', label='sz15', color='#66ff66')
    sz16, = ax1.plot(bandwidths, [wu[16] for wu in bandwidth_wu_curve], '--', label='sz16', color='#003300')
    sz17, = ax1.plot(bandwidths, [wu[17] for wu in bandwidth_wu_curve], '--', label='sz17', color='#339966')
    sz18, = ax1.plot(bandwidths, [wu[18] for wu in bandwidth_wu_curve], '--', label='sz18', color='#00ffff')
    sz19, = ax1.plot(bandwidths, [wu[19] for wu in bandwidth_wu_curve], '--', label='sz19', color='#006699')

    plt.legend(handles=[sz0,sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8,sz9,sz10,sz11,sz12,sz13,sz14,sz15,sz16,sz17,sz18,sz19], loc=(0,0.5), fontsize=7)

# do a we
if do_we:
    fig_we = plt.figure()
    fig_we.suptitle("write existing")
    ax1 = fig_we.add_subplot(111)
    if xlog:
        plt.xscale('log')
    sz0, = ax1.plot(bandwidths, [we[0] for we in bandwidth_we_curve], '--', label='4MB - 10byte', color='#003366')
    sz1, = ax1.plot(bandwidths, [we[1] for we in bandwidth_we_curve], '--', label='sz1', color='#003399')
    sz2, = ax1.plot(bandwidths, [we[2] for we in bandwidth_we_curve], '--', label='sz2', color='#6699ff')
    sz3, = ax1.plot(bandwidths, [we[3] for we in bandwidth_we_curve], '--', label='sz3', color='#6600cc')
    sz4, = ax1.plot(bandwidths, [we[4] for we in bandwidth_we_curve], '--', label='sz4', color='#660066')
    sz5, = ax1.plot(bandwidths, [we[5] for we in bandwidth_we_curve], '--', label='sz5', color='#ff66ff')
    sz6, = ax1.plot(bandwidths, [we[6] for we in bandwidth_we_curve], '--', label='sz6', color='#993366')
    sz7, = ax1.plot(bandwidths, [we[7] for we in bandwidth_we_curve], '--', label='sz7', color='#ff0066')
    sz8, = ax1.plot(bandwidths, [we[8] for we in bandwidth_we_curve], '--', label='sz8', color='#993333')
    sz9, = ax1.plot(bandwidths, [we[9] for we in bandwidth_we_curve], '--', label='sz9', color='#ff9966')
    sz10, = ax1.plot(bandwidths, [we[10] for we in bandwidth_we_curve], '--', label='sz10', color='#cc3300')
    sz11, = ax1.plot(bandwidths, [we[11] for we in bandwidth_we_curve], '--', label='sz11', color='#ffff00')
    sz12, = ax1.plot(bandwidths, [we[12] for we in bandwidth_we_curve], '--', label='sz12', color='#003366')
    sz13, = ax1.plot(bandwidths, [we[13] for we in bandwidth_we_curve], '--', label='sz13', color='#663300')
    sz14, = ax1.plot(bandwidths, [we[14] for we in bandwidth_we_curve], '--', label='sz14', color='#666633')
    sz15, = ax1.plot(bandwidths, [we[15] for we in bandwidth_we_curve], '--', label='sz15', color='#66ff66')
    sz16, = ax1.plot(bandwidths, [we[16] for we in bandwidth_we_curve], '--', label='sz16', color='#003300')
    sz17, = ax1.plot(bandwidths, [we[17] for we in bandwidth_we_curve], '--', label='sz17', color='#339966')
    sz18, = ax1.plot(bandwidths, [we[18] for we in bandwidth_we_curve], '--', label='sz18', color='#00ffff')
    sz19, = ax1.plot(bandwidths, [we[19] for we in bandwidth_we_curve], '--', label='sz19', color='#006699')

    plt.legend(handles=[sz0,sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8,sz9,sz10,sz11,sz12,sz13,sz14,sz15,sz16,sz17,sz18,sz19], loc=(0,0.5), fontsize=7)


# do a rw
if do_re:
    fig_re = plt.figure()
    fig_re.suptitle("rw existing")
    ax1 = fig_re.add_subplot(111)
    if xlog:
        plt.xscale('log')
    sz0, = ax1.plot(bandwidths, [re[0] for re in bandwidth_re_curve], '--', label='4MB - 10byte', color='#003366')
    sz1, = ax1.plot(bandwidths, [re[1] for re in bandwidth_re_curve], '--', label='4MB - 9byte', color='#003399')
    sz2, = ax1.plot(bandwidths, [re[2] for re in bandwidth_re_curve], '--', label='4MB - 8byte', color='#6699ff')
    sz3, = ax1.plot(bandwidths, [re[3] for re in bandwidth_re_curve], '--', label='4MB - 7byte', color='#6600cc')
    sz4, = ax1.plot(bandwidths, [re[4] for re in bandwidth_re_curve], '--', label='4MB - 6byte', color='#660066')
    sz5, = ax1.plot(bandwidths, [re[5] for re in bandwidth_re_curve], '--', label='4MB - 5byte', color='#ff66ff')
    sz6, = ax1.plot(bandwidths, [re[6] for re in bandwidth_re_curve], '--', label='4MB - 4byte', color='#993366')
    sz7, = ax1.plot(bandwidths, [re[7] for re in bandwidth_re_curve], '--', label='4MB - 3byte', color='#ff0066')
    sz8, = ax1.plot(bandwidths, [re[8] for re in bandwidth_re_curve], '--', label='4MB - 2byte', color='#993333')
    sz9, = ax1.plot(bandwidths, [re[9] for re in bandwidth_re_curve], '--', label='4MB - 1byte', color='#ff9966')
    sz10, = ax1.plot(bandwidths, [re[10] for re in bandwidth_re_curve], '--', label='4MB', color='#cc3300')
    sz11, = ax1.plot(bandwidths, [re[11] for re in bandwidth_re_curve], '--', label='4MB + 1byte', color='#ffff00')
    sz12, = ax1.plot(bandwidths, [re[12] for re in bandwidth_re_curve], '--', label='4MB + 2byte', color='#003366')
    sz13, = ax1.plot(bandwidths, [re[13] for re in bandwidth_re_curve], '--', label='4MB + 3byte', color='#663300')
    sz14, = ax1.plot(bandwidths, [re[14] for re in bandwidth_re_curve], '--', label='4MB + 4byte', color='#666633')
    sz15, = ax1.plot(bandwidths, [re[15] for re in bandwidth_re_curve], '--', label='4MB + 5byte', color='#66ff66')
    sz16, = ax1.plot(bandwidths, [re[16] for re in bandwidth_re_curve], '--', label='4MB + 6byte', color='#003300')
    sz17, = ax1.plot(bandwidths, [re[17] for re in bandwidth_re_curve], '--', label='4MB + 7byte', color='#339966')
    sz18, = ax1.plot(bandwidths, [re[18] for re in bandwidth_re_curve], '--', label='4MB + 8byte', color='#00ffff')
    sz19, = ax1.plot(bandwidths, [re[19] for re in bandwidth_re_curve], '--', label='4MB + 9byte', color='#006699')

    plt.legend(handles=[sz0,sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8,sz9,sz10,sz11,sz12,sz13,sz14,sz15,sz16,sz17,sz18,sz19], loc=(0,0.5), fontsize=7)


plt.show()