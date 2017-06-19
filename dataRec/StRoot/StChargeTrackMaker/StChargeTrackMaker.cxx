#include "StTriggerIdCollection.h"
#include "StTriggerId.h"
#include "TH1.h"
//#include "TH2.h"
//#include "TChain.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TMemStat.h"
#include "StarClassLibrary/SystemOfUnits.h"
#include "StarClassLibrary/StHelix.hh"
#include "StarClassLibrary/StThreeVectorD.hh"
#include "StarClassLibrary/StPhysicalHelixD.hh"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuBTofHit.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
#include "StEmcTriggerMaker/StEmcTriggerMaker.h"
#include "StEmcTriggerMaker/StBemcTrigger.h"

#include "StBTofUtil/StV0TofCorrection.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "StMiniClass/StMiniTrack.h"
#include "StChargeTrackMaker.h"


ClassImp(StChargeTrackMaker)

StChargeTrackMaker::StChargeTrackMaker(const char *name, StMuDstMaker *muDstMaker, const char *out_fn)
: StMaker(name), m_MuDstMaker(muDstMaker), m_OutFn(out_fn) {}

void StChargeTrackMaker::InitFile(void)
{
	cout << "StChargeTrackMaker : output file: " << m_OutFn << endl;
	m_OutFile = new TFile(m_OutFn, "recreate");
}

void StChargeTrackMaker::InitTree()
{
	m_OutTree = new TTree("chargeTrack_tree","Charged Track Information Collection");
	m_OutTree->SetDirectory(m_OutFile);

	m_OutTree->Branch("runID",&m_runID,"runID/I");
	m_OutTree->Branch("evtID",&m_evtID,"evtID/I");
	m_OutTree->Branch("spin",&m_spin,"spin/I");
	m_OutTree->Branch("spinbit",&m_spinbit,"spinbit/I");
	m_OutTree->Branch("spinbit8",&m_spinbit8,"spinbit8/I");
	m_OutTree->Branch("bx7",&m_bx7,"bx7/I");
	m_OutTree->Branch("bx48",&m_bx48,"bx48/I");
	m_OutTree->Branch("magn",&m_magn,"magn/D");
	m_OutTree->Branch("bbcrate",&m_bbcrate,"bbcrate/D");
	m_OutTree->Branch("zdcrate",&m_zdcrate,"zdcrate/D");
	m_OutTree->Branch("bbcTimebin",&m_bbcTimebin,"bbcTimebin/I");
	m_OutTree->Branch("pvx",&m_pvx,"pvx/D");
	m_OutTree->Branch("pvy",&m_pvy,"pvy/D");
	m_OutTree->Branch("pvz",&m_pvz,"pvz/D");
	m_OutTree->Branch("npv",&m_npv,"npv/I");
	m_OutTree->Branch("npvrank",m_npvrank,"npvrank[100]/I");
	m_OutTree->Branch("npvz",m_npvz,"npvz[100]/I");
	m_OutTree->Branch("nptr",&m_nptr,"nptr/I");
	m_OutTree->Branch("ngtr",&m_ngtr,"ngtr/I");
	m_OutTree->Branch("trig",m_trig,"trig[5]/I");
	m_OutTree->Branch("IsVPD",&m_IsVPD,"IsVPD/I");
	m_OutTree->Branch("IsJP1",&m_IsJP1,"IsJP1/I");
	m_OutTree->Branch("IsJP2",&m_IsJP2,"IsJP2/I");
	m_OutTree->Branch("IsL2J",&m_IsL2J,"IsL2J/I");
	m_OutTree->Branch("vptr_p",&m_vptr_p);
	m_OutTree->Branch("vptr_n",&m_vptr_n);
	m_OutTree->Branch("vgtr_p",&m_vgtr_p);
	m_OutTree->Branch("vgrt_n",&m_vgtr_n);
}

void StChargeTrackMaker::vClear(void)
{
	m_vptr_p.clear();
	m_vptr_n.clear();
	m_vgtr_p.clear();
	m_vgtr_n.clear();
}

bool StChargeTrackMaker::checkTrack( StMuTrack *mt )
{
	short tFlag = mt->flag();
	if( tFlag<0 || tFlag>1000 ) return true;
	short tCharge = mt->charge();
	if( !tCharge ) return true;
	short tNHits = mt->nHitsFit();
	if( tNHits<15 ) return true;
	short tNHitsPoss = mt->nHitsPoss();
	float tNratio = float(tNHits)/float(tNHitsPoss);
	if( tNratio<0.51 ) return true;

	return false;
}

int StChargeTrackMaker::Init()
{
	InitFile();
	InitTree();

	mH_pvz0 = new TH1D("h_pvz0","h_pvz0",200,-100.0,100.0);
	mH_pvz1 = new TH1D("h_pvz1","h_pvz1",200,-100.0,100.0);

	m_SpinDbMaker = (StSpinDbMaker*)GetMaker("spinDbMaker");
	assert(m_SpinDbMaker);

	m_TrigSimuMaker = (StTriggerSimuMaker*)GetMaker("trigSimuMaker");
	assert(m_TrigSimuMaker);

	return StMaker::Init();
}

int StChargeTrackMaker::Finish()
{
	m_OutFile->Write();
	m_OutFile->Close();
	delete m_OutFile;

	return kStOk;
}

int StChargeTrackMaker::Make()
{
	vClear();
	m_runID = -1;
	m_evtID = -1;
	m_spin = -1;
	m_spinbit = -1;
	m_spinbit8 = -1;
	m_bx7 = -1;
	m_bx48 = -1;
	m_magn = 0;
	m_bbcrate = -1;
	m_zdcrate = -1;
	m_bbcTimebin = -1;
	m_pvx = -999.9;
	m_pvy = -999.9;
	m_pvz = -999.9;
	m_npv = -1;
	for (int i = 0; i < 100; ++i)
	{
		m_npvrank[i] = -1;
		m_npvz[i] = -999.9;
	}
	m_nptr = 0;
	m_ngtr = 0;
	for (int i = 0; i < 5; ++i) m_trig[i] = 0;
	m_IsVPD = 0;
	m_IsJP1 = 0;
	m_IsJP2 = 0;
	m_IsL2J = 0;

	cout << "" << endl;
	if (m_MuDstMaker!=NULL)
	{
		m_MuDst = m_MuDstMaker->muDst();
	}
	else {
		cout << "StChargeTrackMaker : StMuDstMaker Error! " << endl;
		abort();
	}

	StMuEvent *muEvent = m_MuDst->event();
	StThreeVectorF pv_position = muEvent->primaryVertexPosition();
	m_pvx = pv_position.x();
	m_pvy = pv_position.y();
	m_pvz = pv_position.z();
	m_magn = muEvent->magneticField();

	m_runID = muEvent->runNumber();
	m_evtID = muEvent->eventId();

	StL0Trigger & spintrig = muEvent->l0Trigger();
	m_bx48 = spintrig.bunchCrossingId();
	m_bx7 = spintrig.bunchCrossingId7bit(m_runID);
	m_spin = 0;
	m_spinbit = -1;
	m_spinbit8 = -1;
	if (m_SpinDbMaker->isMaskedUsingBX48(m_bx48)) return kStSkip;
	if (m_SpinDbMaker->isPolDirLong()) m_spin = 1;
	if (m_SpinDbMaker->isPolDirTrans()) m_spin = 2;
	if (m_spin!=2) return kStSkip;
	m_spinbit = m_SpinDbMaker->spin4usingBX7(m_bx7);
	m_spinbit8 = m_SpinDbMaker->spin8usingBX7(m_bx7);
	if (m_spinbit!=5 && m_spinbit!=6 && m_spinbit!=9 && m_spinbit!=10) return kStSkip;
	StRunInfo & runInfo = muEvent->runInfo();
	m_bbcrate = runInfo.bbcCoincidenceRate();
	m_zdcrate = runInfo.zdcCoincidenceRate();

	StBbcTriggerDetector & bbc = muEvent->bbcTriggerDetector();
	m_bbcTimebin = bbc.onlineTimeDifference() >> 9 & 0xf;

	StTriggerId nominal = muEvent->triggerIdCollection().nominal();

	if (nominal.isTrigger(480202)) m_trig[0]=1; // BHT1*VPDMB-30
	if (nominal.isTrigger(480206)) m_trig[1]=1; // BHT1*VPDMB-30-nobsmd
	if (nominal.isTrigger(480404)||nominal.isTrigger(480414)) m_trig[2]=1; // JP1
	if (nominal.isTrigger(480401)||nominal.isTrigger(480411)) m_trig[3]=1; // JP2
	if (nominal.isTrigger(480405)||nominal.isTrigger(480415)) m_trig[4]=1; // JP2*L2JetHigh

	if ( !(m_trig[0]+m_trig[1]+m_trig[2]+m_trig[3]+m_trig[4]) ) return kStSkip;

	m_npv = m_MuDst->numberOfPrimaryVertices();
	for (int i = 0; i < m_npv; ++i)
	{
		StMuPrimaryVertex *mupv = m_MuDst->primaryVertex(i);
		StThreeVectorF posV = mupv->position();
		m_npvrank[i] = mupv->ranking();
		m_npvz[i] = posV.z();
	}
	if (m_npvrank[0]<0) return kStSkip;

	m_nptr = m_MuDst->numberOfPrimaryTracks();
	m_ngtr = m_MuDst->numberOfGlobalTracks();
	if ( m_nptr<1 || m_nptr<1 ) return kStSkip;
	mH_pvz0->Fill(m_pvz);
	if (fabs(m_pvz)>60.0) return kStSkip;
	LOG_INFO << m_evtID << " ~ " << m_pvz << endm;
	mH_pvz1->Fill(m_pvz);

	StMiniTrack testMt;
	LOG_INFO << testMt.get_charge() << endm;
	
	for (int i = 0; i < m_nptr; ++i)
	{
		StMuTrack *mt = m_MuDst->primaryTracks(i);
		if (checkTrack(mt)) continue;
		if (mt->pt()<1.0) continue;
		LOG_INFO << mt->pt() << endm;
		StMiniTrack miniMt(mt);
		if (miniMt.beyond_nSigma())	continue;
		if (miniMt.dcaXY()>1.5) continue;
		if (mt->charge()>0) m_vptr_p.push_back(miniMt);
		if (mt->charge()<0) m_vptr_n.push_back(miniMt);
	}
	m_vptr_p.shrink_to_fit();
	m_vptr_n.shrink_to_fit();

	for (int i = 0; i < m_ngtr; ++i)
	{
		StMuTrack *mt = m_MuDst->globalTracks(i);
		if (checkTrack(mt)) continue;
		if (mt->pt()<1.0) continue;
		StMiniTrack miniMt(mt);
		if (miniMt.beyond_nSigma()) continue;
		if (miniMt.dcaXY()>1.5) continue;
		if (mt->charge()>0) m_vgtr_p.push_back(miniMt);
		if (mt->charge()<0) m_vgtr_n.push_back(miniMt);
	}
	m_vgtr_p.shrink_to_fit();
	m_vgtr_n.shrink_to_fit();

	m_OutTree->Fill();

	return kStOk;
}







