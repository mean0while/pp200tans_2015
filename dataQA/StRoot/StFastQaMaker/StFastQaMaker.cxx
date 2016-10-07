//
//  StFastQaMaker.cxx
//  
//
//  Created by MeiJincheng on 9/21/16.
//
//

#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TString.h"

#include "StTriggerIdCollection.h"
#include "StTriggerId.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "TSystem.h"
#include "TFile.h"
#include "TMemStat.h"
#include "StarClassLibrary/SystemOfUnits.h"
//#include "StarClassLibrary/StHelix.hh"
#include "StarClassLibrary/StThreeVectorD.hh"
//#include "StarClassLibrary/StPhysicalHelixD.hh"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
#include "StEmcTriggerMaker/StEmcTriggerMaker.h"
#include "StEmcTriggerMaker/StBemcTrigger.h"
#include "StMuDSTMaker/COMMON/StMuBTofHit.h"
#include "StBTofUtil/StV0TofCorrection.h"

#include "StFastQaMaker.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

ClassImp(StFastQaMaker)

StFastQaMaker::StFastQaMaker(const char *name, StMuDstMaker *muDstMaker, const char *fn_out)
:StMaker(name), mMuDstMaker(muDstMaker), mOutFn(fn_out)
{}

//StFastQaMaker::~StFastQaMaker() {}

int StFastQaMaker::Init()
{
    // creating lambda nano ROOT file name
    cout << "StFastQaMaker : Output File Name:\t" << mOutFn << endl;
    // open udst file
    mOutFile = new TFile(mOutFn, "RECREATE");
    
    // creat histograms
    mH_magn = new TH1D("h_magn","h_magn",110, -5.5,5.5);
    mH_trig = new TH1I("h_trig","h_trig",2,0,2);
    
    mH_bx7 = new TH1I("h_bx7","h_bx7",120,0,120);
    mH_bx48 = new TH1I("h_bx48","h_bx48",120,0,120);
    mH_spin = new TH1I("h_spin","h_spin",5,0,5);
    mH_spinbit = new TH1I("h_spinbin","h_spinbit",30,-10,20);
    mH_spinbit8 = new TH1I("h_spinbit8","h_spinbit8",50,50,100);
    
    mH_BBCrate = new TH1D("h_BBCrate","h_BBCrate",900,200e3,1100e3);
    mH_ZDCrate = new TH1D("h_ZDCrate","h_ZDCrate",900,2e3,100e3);
    mH_BBCTimebin = new TH1I("h_BBCTimebin","h_BBCTimebin",100,0,100);
    
    mH_nPV = new TH1I("h_nPV","h_nPV",50,0,50);
    mH_PVz = new TH1D("h_PVz","h_PVz",200,-100,100);
    mH_PVxy = new TH2D("h_PVxy","h_PVxy",200,-0.1,0.1,200,-0.1,0.1);
    
    TString hn;
    for (int i=0; i<2; ++i) { // 0,1:   1~ positive charged     0~ nagetive charged
        hn = Form("h_n_ptr_%d", i);
        mH_n_ptr[i] = new TH1I(hn.Data(),hn.Data(),1000,0,1000);
        hn = Form("h_n_gtr_%d", i);
        mH_n_gtr[i] = new TH1I(hn.Data(),hn.Data(),2000,0,2000);
        hn = Form("h_nHits_ptr_%d", i);
        mH_nHits_ptr[i] = new TH1I(hn.Data(),hn.Data(),46,0,46);
        hn = Form("h_nHits_gtr_%d", i);
        mH_nHits_gtr[i] = new TH1I(hn.Data(),hn.Data(),46,0,46);
        hn = Form("h_qFit_ptr_%d", i);
        mH_qFit_ptr[i] = new TH2D(hn.Data(),hn.Data(),31,15,46,100,0,10);
        hn = Form("h_qFit_gtr_%d", i);
        mH_qFit_gtr[i] = new TH2D(hn.Data(),hn.Data(),31,15,46,100,0,10);
        hn = Form("h_pt_ptr_%d", i);
        mH_pt_ptr[i] = new TH1D(hn.Data(),hn.Data(),500,0,10);
        hn = Form("h_pt_gtr_%d", i);
        mH_pt_gtr[i] = new TH1D(hn.Data(),hn.Data(),500,0,10);
        hn = Form("h_etaphi_ptr_%d", i);
        mH_etaphi_ptr[i] = new TH2D(hn.Data(),hn.Data(),360,-1.8,1.8,320,-3.2,3.2);
        hn = Form("h_etaphi_gtr_%d", i);
        mH_etaphi_gtr[i] = new TH2D(hn.Data(),hn.Data(),360,-1.8,1.8,320,-3.2,3.2);
        hn = Form("h_dEdxpt_ptr_%d", i);
        mH_dEdxpt_ptr[i] = new TH2D(hn.Data(),hn.Data(),500,0,5,500,-1,4);
        hn = Form("h_dEdxpt_gtr_%d", i);
        mH_dEdxpt_gtr[i] = new TH2D(hn.Data(),hn.Data(),500,0,5,500,-1,4);
        hn = Form("h_tofM_ptr_%d", i);
        mH_tofM_ptr[i] = new TH1I(hn.Data(),hn.Data(),10,0,10);
        hn = Form("h_tofM_gtr_%d", i);
        mH_tofM_gtr[i] = new TH1I(hn.Data(),hn.Data(),10,0,10);
        hn = Form("h_tofBeta_ptr_%d", i);
        mH_tofBeta_ptr[i] = new TH2D(hn.Data(),hn.Data(),500,0,5,400,0,4);
        hn = Form("h_tofBeta_gtr_%d", i);
        mH_tofBeta_gtr[i] = new TH2D(hn.Data(),hn.Data(),500,0,5,400,0,4);
        hn = Form("h_bemcM_ptr_%d", i);
        mH_bemcM_ptr[i] = new TH1I(hn.Data(),hn.Data(),2,0,2);
        hn = Form("h_bemcM_gtr_%d", i);
        mH_bemcM_gtr[i] = new TH1I(hn.Data(),hn.Data(),2,0,2);
        hn = Form("h_bemcE_ptr_%d", i);
        mH_bemcE_ptr[i] = new TH2D(hn.Data(),hn.Data(),500,0,10,500,0,10);
        hn = Form("h_bemcE_gtr_%d", i);
        mH_bemcE_gtr[i] = new TH2D(hn.Data(),hn.Data(),500,0,10,500,0,10);
    }
    // assert the StSpinDbMaker
    mSpDb = (StSpinDbMaker*)GetMaker("spinDb");
    assert(mSpDb);
    
    // assert the StTriggerSimuMaker
    //    mTrigSimu = (StTriggerSimuMaker*)GetMaker("TrgSimuMaker");
    //    assert(mTrigSimu);

    return kStOk;
}

int StFastQaMaker::Finish()
{
    mOutFile->Write();
    mOutFile->Close();
    delete mOutFile;
    
    return kStOk;
}

int StFastQaMaker::Make()
{
    if (mMuDstMaker != NULL)
    {
        mMuDst = mMuDstMaker->muDst();
    }
    else
    {
        cout << "StFastQaMaker Error!" << endl;
        abort();
    }
    
    StMuEvent *mMuEvent = mMuDst->event();
    StThreeVectorD PV_position = mMuEvent->primaryVertexPosition();
    double pvz = PV_position.z();
    double pvy = PV_position.y();
    double pvx = PV_position.x();
    double magn = mMuEvent->magneticField();
    
    int runID = mMuEvent->runNumber();
    int evtID = mMuEvent->eventId();
    
    StL0Trigger & spintrig = mMuEvent->l0Trigger();
    int bx48 = spintrig.bunchCrossingId();
    int bx7  = spintrig.bunchCrossingId7bit(runID);
    short spin = 0;
    short spinbit = -1;
    short spinbit8 = -1;
    if (mSpDb->isPolDirLong())  spin = 1;
    if (mSpDb->isPolDirTrans()) spin = 2;
    if (mSpDb->isMaskedUsingBX48(bx48))
    {
        spinbit = -1;
        spinbit8 = -1;
    }
    else
    {
        spinbit = mSpDb->spin4usingBX7(bx7);
        spinbit8 = mSpDb->spin8usingBX7(bx7);
    }
    if (spinbit<0 || spinbit8<0) {
        return kStSkip;
    }
    
    StRunInfo &runinfo = mMuEvent->runInfo();
    double bbcrate = runinfo.bbcCoincidenceRate();
    double zdcrate = runinfo.zdcCoincidenceRate();

    StBbcTriggerDetector& bbc = mMuEvent->bbcTriggerDetector();
    int bbcTimebin = bbc.onlineTimeDifference() >> 9 & 0xf;
  
    if (fabs(pvz)>60.0) {
        return kStSkip;
    }
    // TriggerId;
    StTriggerId nominal = mMuEvent->triggerIdCollection().nominal();
    int trig[5];
    for (int i=0; i<5; i++) trig[i] = 0;
    
    if (nominal.isTrigger(480202))  trig[0]=1; // BHT1*VPDMB-30
    if (nominal.isTrigger(480206))  trig[1]=1; // BHT1*VPDMB-30-nobsmd

    if (nominal.isTrigger(480404) || nominal.isTrigger(480414))  trig[2]=1; // JP1
    if (nominal.isTrigger(480401) || nominal.isTrigger(480411))  trig[3]=1; // JP2
    if (nominal.isTrigger(480405) || nominal.isTrigger(480415))  trig[4]=1; // JP2*L2JetHigh

    if ( !(trig[2]+trig[3]+trig[4]) ) return kStSkip;

    int npv = mMuDst->numberOfPrimaryVertices();
    if (npv<1) {
        return kStSkip;
    }
    int npvrank[100] = {-999};
    double npvz[100] = {-999.9};
    for (int i=0; i<npv; i++)
    {
        StMuPrimaryVertex* mupv = mMuDst->primaryVertex(i);
        StThreeVectorD posV = mupv->position();
        npvrank[i] = mupv->ranking();
        npvz[i] = posV.z();
    }
    if (npvrank[0]<0) {
        return kStSkip;
    }
    
    unsigned int nptr = 0;
    unsigned int ngtr = 0;
    nptr = mMuDst->numberOfPrimaryTracks();
    ngtr = mMuDst->numberOfGlobalTracks();
    if (nptr<1 || ngtr<1) {
       return kStSkip;
    }
    
    mH_nPV->Fill(npv);
    mH_PVz->Fill(pvz);
    mH_PVxy->Fill(pvx, pvy);
    
    mH_magn->Fill(magn);
    mH_bx7->Fill(bx7);
    mH_bx48->Fill(bx48);
    mH_spin->Fill(spin);
    mH_spinbit->Fill(spinbit);
    mH_spinbit8->Fill(spinbit8);
    
    mH_BBCrate->Fill(bbcrate);
    mH_ZDCrate->Fill(zdcrate);

    for (unsigned int iP=0; iP<nptr; ++iP) {
        StMuTrack *tTrack = mMuDst->primaryTracks( iP );
        //short tId = tTrack->id();
        short tFlag = tTrack->flag();
        if(tFlag<0 || tFlag>1000) continue;
        short tCharge = tTrack->charge();
        if(!tCharge) continue;
        short tNHits = tTrack->nHitsFit();
        if(tNHits<15) continue;
        short tNHitsPoss = tTrack->nHitsPoss();
        double tNratio = double(tNHits)/double(tNHitsPoss);
        if(tNratio<0.51) continue;
        double tPt = tTrack->pt();
        double tEta = tTrack->eta();
        double tPhi = tTrack->phi();
        double tDEdx = tTrack->dEdx();
        double tChi2 = tTrack->chi2();
        bool tBemcM = tTrack->matchBEMC();
        double tBemcE = -999.9;
        if (tBemcM) tBemcE = tTrack->energyBEMC();
        StMuBTofPidTraits tBtof = tTrack->btofPidTraits();
        unsigned char tBtofM = tBtof.matchFlag();
        double tBtofBeta = -999.9;
        if (tBtofM) tBtofBeta = tBtof.beta();
        //cout << "Primary Hist Fill\n";   
        unsigned int cF = 999.9;
        if (tCharge<0) cF = 0;
        if (tCharge>0) cF = 1;
        if ( cF>1 ) continue;
        //cout << "cFlag : " << cF << endl;
         
        mH_nHits_ptr[cF]->Fill(tNHits);
        mH_qFit_ptr[cF]->Fill(tNHits,tChi2);
        mH_pt_ptr[cF]->Fill(tPt);
        mH_etaphi_ptr[cF]->Fill(tEta, tPhi);
        //cout << "dEdx : " << tDEdx*10e3 << endl;
        mH_dEdxpt_ptr[cF]->Fill(tPt, tDEdx*10e3);
        mH_tofM_ptr[cF]->Fill(tBtofM);
        if(tBtofM) mH_tofBeta_ptr[cF]->Fill(tPt, 1.0/tBtofBeta);
        mH_bemcM_ptr[cF]->Fill(tBemcM);
        if(tBemcM) mH_bemcE_ptr[cF]->Fill(tPt, tBemcE);
    }      
    for (unsigned int iG=0; iG<ngtr; ++iG) {
        StMuTrack *tTrack = mMuDst->globalTracks( iG );
        //short tId = tTrack->id();
        short tFlag = tTrack->flag();
        if(tFlag<0 || tFlag>1000) continue;
        short tCharge = tTrack->charge();
        if(!tCharge) continue;
        short tNHits = tTrack->nHitsFit();
        if(tNHits<15) continue;
        short tNHitsPoss = tTrack->nHitsPoss();
        double tNratio = double(tNHits)/double(tNHitsPoss);
        if(tNratio<0.51) continue;
        double tPt = tTrack->pt();
        double tEta = tTrack->eta();
        double tPhi = tTrack->phi();
        double tDEdx = tTrack->dEdx();
        double tChi2 = tTrack->chi2();
        bool tBemcM = tTrack->matchBEMC();
        double tBemcE = -999.9;
        if (tBemcM) tBemcE = tTrack->energyBEMC();
        StMuBTofPidTraits tBtof = tTrack->btofPidTraits();
        unsigned char tBtofM = tBtof.matchFlag();
        double tBtofBeta = -999.9;
        if (tBtofM) tBtofBeta = tBtof.beta();
        
        unsigned int cF = 999.9;
        if (tCharge<0) cF = 0;
        if (tCharge>0) cF = 1;
        if ( cF>1 ) continue;
         
        mH_nHits_gtr[cF]->Fill(tNHits);
        mH_qFit_gtr[cF]->Fill(tNHits, tChi2);
        mH_pt_gtr[cF]->Fill(tPt);
        mH_etaphi_gtr[cF]->Fill(tEta, tPhi);
        //cout << "dEdx : " << tDEdx*10e3 << endl;
        mH_dEdxpt_gtr[cF]->Fill(tPt, tDEdx*10e3);
        mH_tofM_gtr[cF]->Fill(tBtofM);
        mH_tofBeta_gtr[cF]->Fill(tPt, 1.0/tBtofBeta);
        if(tBtofM) mH_bemcM_gtr[cF]->Fill(tBemcM);
        if(tBemcM) mH_bemcE_gtr[cF]->Fill(tPt, tBemcE);
    }
    return kStOk;
}
