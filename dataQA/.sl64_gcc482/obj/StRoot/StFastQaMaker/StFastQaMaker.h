//
//  StFastQaMaker.hpp
//  
//
//  Created by MeiJincheng on 9/21/16.
//
//

#ifndef StFastQaMaker_h
#define StFastQaMaker_h

#include "StMaker.h"
//#include <string>
//#include <vector>
//#include "StTriggerIdCollection.h"
//#include "StTriggerId.h"
//#include "TTree.h"
//#include "TChain.h"
//#include "TSystem.h"
//#include "TFile.h"
//#include "TMemStat.h"
//#include <iostream>
//#include <fstream>
//#include <stdlib.h>
//#include "StarClassLibrary/SystemOfUnits.h"
//#include "StarClassLibrary/StHelix.hh"
//#include "StarClassLibrary/StThreeVectorD.hh"
//#include "StarClassLibrary/StPhysicalHelixD.hh"
//#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
//#include "StMuDSTMaker/COMMON/StMuDst.h"
//#include "StMuDSTMaker/COMMON/StMuEvent.h"
//#include "StMuDSTMaker/COMMON/StMuTrack.h"
//#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
//#include "StTriggerUtilities/StTriggerSimuMaker.h"
//#include "StTriggerUtilities/Bemc/StBemcTriggerSimu.h"
//#include "StTriggerUtilities/Eemc/StEemcTriggerSimu.h"
//#include "StTriggerUtilities/Emc/StEmcTriggerSimu.h"
//#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
//#include "StEmcTriggerMaker/StEmcTriggerMaker.h"
//#include "StEmcTriggerMaker/StBemcTrigger.h"
//#include "StSpinPool/StJetEvent/StJetEvent.h"
//#include "StSpinPool/StJetEvent/StJetCandidate.h"

class TFile;
class TH1I;
class TH1D;
class TH2D;

class StMuTrack;
class StMuDst;
class StMuDstMaker;
class StSpinDbMaker;
//class StTriggerSimuMaker;

class StFastQaMaker : public StMaker {

private:
    StMuDst             *mMuDst;
    StMuDstMaker        *mMuDstMaker;
    StSpinDbMaker       *mSpDb;
//    StTriggerSimuMaker  *mTrigSimu;
    
    const char *mOutFn;
    TFile      *mOutFile;
    
    TH1D       *mH_magn;
    TH1I       *mH_trig;
    
    TH1I       *mH_bx7;
    TH1I       *mH_bx48;
    TH1I       *mH_spin;
    TH1I       *mH_spinbit;
    TH1I       *mH_spinbit8;
    
    TH1D       *mH_BBCrate;
    TH1D       *mH_ZDCrate;
    TH1I       *mH_BBCTimebin;
    
    TH1I       *mH_nPV;
    TH1D       *mH_PVz;
    TH2D       *mH_PVxy;
    TH1I       *mH_n_ptr[2];
    TH1I       *mH_n_gtr[2];
    TH1I       *mH_nHits_ptr[2];
    TH1I       *mH_nHits_gtr[2];
    TH2D       *mH_qFit_ptr[2];
    TH2D       *mH_qFit_gtr[2];
    TH1D       *mH_pt_ptr[2];
    TH1D       *mH_pt_gtr[2];
    TH2D       *mH_etaphi_ptr[2];
    TH2D       *mH_etaphi_gtr[2];
    TH2D       *mH_dEdxpt_ptr[2];
    TH2D       *mH_dEdxpt_gtr[2];
    TH1I       *mH_tofM_ptr[2];
    TH1I       *mH_tofM_gtr[2];
    TH2D       *mH_tofBeta_ptr[2];
    TH2D       *mH_tofBeta_gtr[2];
    TH1I       *mH_bemcM_ptr[2];
    TH1I       *mH_bemcM_gtr[2];
    TH2D       *mH_bemcE_ptr[2];
    TH2D       *mH_bemcE_gtr[2];

    TH2D       *mH_nPVvsBBCrate;
    
public:
    StFastQaMaker(const char *name, StMuDstMaker *muDstMaker, const char *fn_out);
//    virtual      ~StFastQaMaker();
    
    virtual int  Init();
    virtual int  Make();
    virtual int  Finish();
    
    bool trackCut( StMuTrack* tT);
    
    ClassDef(StFastQaMaker, 0)
};


#endif /* StFastQaMaker_h */
