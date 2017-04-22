# This macro is used to qeury run information from RunLog database
# The parameter is prepared for run15trans at first
# output as a csv file

from MySQLdb import *

#---------------------------------------------------------------------------------------------
my_host = 'dbbak.starp.bnl.gov'
my_port = 3414   #work for run15, and other runs can be found from https://drupal.star.bnl.gov/STAR/comp/db/onlinedb/online-server-port-map
#---------------------------------------------------------------------------------------------

# input list file
fin = open('../../List/runNumber_0.lis')
run_list = fin.readlines()

# output file
fout = open('qa_runSummary.csv', 'w')
out_str = 'FillNumber,RunNumber,RtsStatus,ShiftStatus,RunType,BeginRunTime,TotalRunTime,nEvents,nFiles\n'
fout.write(out_str)

# connect RunLog db
my_db = connect(host=my_host,port=my_port,db='RunLog')
my_cr = my_db.cursor()

for runN in run_list:

    runN = runN.strip()
    print runN

    # Global Variables
    my_FillN             = -1
    my_runType           = -1
    my_nEvts             = -1
    my_nFiles            = -1
    my_nFilesSca         = -1
    my_rtsStatus         = -1
    my_shiftLeaderStatus = -1
    my_beginRunTime      = 'Null'
    my_runTime           = -1

    # beamInfo
    cmd = 'SELECT blueFillNumber,yellowFillNumber FROM beamInfo WHERE runNumber=%s;' % runN
    my_cr.execute(cmd)
    tres = my_cr.fetchall()
    blueFN = tres[0][0]
    yellFN = tres[1][0]
    if blueFN == yellFN:
       my_FillN = blueFN
    # daqSummary
    cmd = 'SELECT runTypeID,numberOfEvents,numberOfFiles FROM daqSummary WHERE runNumber=%s;' % runN
    my_cr.execute(cmd)
    tres = my_cr.fetchone()
    my_runType = tres[0]
    my_nEvts = tres[1]
    my_nFiles = tres[2]
    # runDescriptor
    cmd = 'SELECT startRunTime,endRunTime,beginTime FROM runDescriptor WHERE runNumber=%s;' % runN
    my_cr.execute(cmd)
    tres = my_cr.fetchone()
    my_runTime = tres[1] - tres[0]
    my_beginRunTime = tres[2]
    # runStatus
    cmd = 'SELECT rtsStatus,shiftLeaderStatus FROM runStatus WHERE runNumber=%s;' % runN
    my_cr.execute(cmd)
    tres = my_cr.fetchone()
    my_rtsStatus = tres[0]
    my_shiftLeaderStatus = tres[1]

    # out_str = 'FillNumber,RunNumber,RtsStatus,ShiftStatus,RunType,BeginRunTime,TotalRunTime,nEvents,nFiles\n'
    out_str = '{0},{1},{2},{3},{4},{5},{6},{7},{8}\n'.format(my_FillN, runN, my_rtsStatus, my_shiftLeaderStatus, my_runType, my_beginRunTime, my_runTime, my_nEvts, my_nFiles)
    fout.write(out_str)

fout.close()

