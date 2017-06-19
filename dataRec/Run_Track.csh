#!/bin/csh

set runs = ` awk '{print $1}' < ../List/runNumber_1_runlogQA.lis `
set dir = /star/u/jcmei/pp200trans_2015/dataRec/data

cd /star/u/jcmei/pp200trans_2015/dataRec/JOBS

set i = 701
while ( $i <= 786 )
   mkdir -p $dir/$runs[$i]
   mkdir -p $dir/$runs[$i]/{out,err}
   star-submit-template -template /star/u/jcmei/pp200trans_2015/dataRec/Sub_dataRec.xml -entities runnumber=$runs[$i]
@ i++
end


