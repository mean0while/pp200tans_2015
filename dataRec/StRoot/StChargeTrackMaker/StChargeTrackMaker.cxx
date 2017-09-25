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
#include "StarClassLibrary/StLorentzVectorD.hh"
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
#include "StTriggerUtilities/StTriggerSimuMaker.h"
#include "StTriggerUtilities/Bemc/StBemcTriggerSimu.h"
#include "StTriggerUtilities/Eemc/StEemcTriggerSimu.h"
#include "StTriggerUtilities/Emc/StEmcTriggerSimu.h"

#include "StBTofUtil/StV0TofCorrection.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>
#include <map>

#include "StMiniClass/StMiniTrack.h"
#include "StChargeTrackMaker.h"

using namespace std;

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
	m_OutTree->Branch("nptr",&m_nptr,"nptr/I");
	m_OutTree->Branch("ngtr",&m_ngtr,"ngtr/I");

	m_OutTree->Branch("npvrank",&m_nPVrank);
	m_OutTree->Branch("npvz",&m_nPVz);
	m_OutTree->Branch("trig",&m_Trig);
	m_OutTree->Branch("trigSoft",&m_TrigSoft);
	m_OutTree->Branch("ptr_p",&m_vptr_p);
	m_OutTree->Branch("ptr_n",&m_vptr_n);
	m_OutTree->Branch("gtr_p",&m_vgtr_p);
	m_OutTree->Branch("grt_n",&m_vgtr_n);

	m_OutTree->Branch("cosrp_L",&m_cosrp_L);
	m_OutTree->Branch("openAngle_L",&m_openAngle_L);
	m_OutTree->Branch("id_Lp",&m_id_Lp);
	m_OutTree->Branch("id_Lpi",&m_id_Lpi);
	m_OutTree->Branch("v3_L",&m_v3_L);
	m_OutTree->Branch("im_L",&m_im_L);
	m_OutTree->Branch("decayPoint_L",&m_v0_L);
	m_OutTree->Branch("dca2_L",&m_dca2_L);
	m_OutTree->Branch("dcaV0_L",&m_dcaV0_L);

	m_OutTree->Branch("cosrp_A",&m_cosrp_A);
	m_OutTree->Branch("openAngle_A",&m_openAngle_A);
	m_OutTree->Branch("id_Ap",&m_id_Ap);
	m_OutTree->Branch("id_Api",&m_id_Api);
	m_OutTree->Branch("v3_A",&m_v3_A);
	m_OutTree->Branch("im_A",&m_im_A);
	m_OutTree->Branch("decayPoint_A",&m_v0_A);
	m_OutTree->Branch("dca2_A",&m_dca2_A);
	m_OutTree->Branch("dcaV0_A",&m_dcaV0_A);

	m_OutTree->Branch("cosrp_K",&m_cosrp_K);
	m_OutTree->Branch("openAngle_K",&m_openAngle_K);
	m_OutTree->Branch("id_Kp",&m_id_Kp);
	m_OutTree->Branch("id_Kpi",&m_id_Kpi);
	m_OutTree->Branch("v3_K",&m_v3_K);
	m_OutTree->Branch("im_K",&m_im_K);
	m_OutTree->Branch("decayPoint_K",&m_v0_K);
	m_OutTree->Branch("dca2_K",&m_dca2_K);
	m_OutTree->Branch("dcaV0_K",&m_dcaV0_K);
}

void StChargeTrackMaker::Add_poolTrigID(int tID)
{
	pool_trigID.push_back(tID);
}

void StChargeTrackMaker::vClear(void)
{
	m_nPVrank.clear();
	m_nPVz.clear();
	m_Trig.clear();
	m_TrigSoft.clear();
	m_vptr_p.clear();
	m_vptr_n.clear();
	m_vgtr_p.clear();
	m_vgtr_n.clear();

	m_cosrp_L.clear();
	m_openAngle_L.clear();
	m_im_L.clear();
	m_id_Lp.clear();
	m_id_Lpi.clear();
	m_v3_L.clear();
	m_v0_L.clear();
	m_dca2_L.clear();
	m_dcaV0_L.clear();

	m_cosrp_A.clear();
	m_openAngle_A.clear();
	m_im_A.clear();
	m_id_Ap.clear();
	m_id_Api.clear();
	m_v3_A.clear();
	m_v0_A.clear();
	m_dca2_A.clear();
	m_dcaV0_A.clear();

	m_cosrp_K.clear();
	m_openAngle_K.clear();
	m_im_K.clear();
	m_id_Kp.clear();
	m_id_Kpi.clear();
	m_v3_K.clear();
	m_v0_K.clear();
	m_dca2_K.clear();
	m_dcaV0_K.clear();
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

	m_H_pvz0 = new TH1D("h_pvz0","h_pvz0",200,-100.0,100.0);
	m_H_pvz1 = new TH1D("h_pvz1","h_pvz1",200,-100.0,100.0);

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
	m_nptr = 0;
	m_ngtr = 0;

	pool_trigID.shrink_to_fit();
	for (vector<int>::iterator i = pool_trigID.begin(); i != pool_trigID.end(); ++i)
	{
		int tID = *i;
		m_Trig[tID] = false;
		m_TrigSoft[tID] = false;
	}

	cout << "Tree variables initialization over !" << endl;
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
	int sum_Trig = 0;
	for (vector<int>::iterator i = pool_trigID.begin(); i != pool_trigID.end(); ++i)
	{
		int tID = *i;
		if (nominal.isTrigger(tID)) m_Trig[tID] = true;
		if (m_TrigSimuMaker->isTrigger(tID)) m_TrigSoft[tID] = true;
		sum_Trig = m_Trig[tID] + sum_Trig;
	}

	if (!sum_Trig) return kStSkip;

	m_npv = m_MuDst->numberOfPrimaryVertices();
	if (m_npv<1) return kStSkip;
	for (int i = 0; i < m_npv; ++i)
	{
		StMuPrimaryVertex *mupv = m_MuDst->primaryVertex(i);
		StThreeVectorF posV = mupv->position();
		m_nPVrank.push_back(mupv->ranking());
		m_nPVz.push_back(posV.z());
	}
	m_nPVrank.shrink_to_fit();
	m_nPVz.shrink_to_fit();
	if (m_nPVrank[0]<0) return kStSkip;

	m_nptr = m_MuDst->numberOfPrimaryTracks();
	m_ngtr = m_MuDst->numberOfGlobalTracks();
	if ( m_nptr<1 || m_ngtr<1 ) return kStSkip;
	m_H_pvz0->Fill(m_pvz);
	if (fabs(m_pvz)>60.0) return kStSkip;
	//LOG_INFO << m_evtID << " ~ " << m_pvz << endm;
	m_H_pvz1->Fill(m_pvz);

	//StMiniTrack testMt;
	//LOG_INFO << testMt.get_charge() << endm;

	for (int i = 0; i < m_nptr; ++i)
	{
		StMuTrack *mt = m_MuDst->primaryTracks(i);
		if (checkTrack(mt)) continue;
		if (mt->pt()<0.5) continue;
		//LOG_INFO << mt->pt() << endm;

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
		if (mt->pt()<0.0001) continue;
		StMiniTrack miniMt(mt);
		if (miniMt.beyond_nSigma()) continue;
		if (mt->charge()>0) m_vgtr_p.push_back(miniMt);
		if (mt->charge()<0) m_vgtr_n.push_back(miniMt);
	}
	m_vgtr_p.shrink_to_fit();
	m_vgtr_n.shrink_to_fit();

	for (vector<StMiniTrack>::iterator ip = m_vgtr_p.begin(); ip != m_vgtr_p.end(); ++ip)
	{
		auto tp = *ip;
		if (fabs(tp.get_nSigmaPion())>3.0 && fabs(tp.get_nSigmaProton())>3.0) continue;

		for (std::vector<StMiniTrack>::iterator in = m_vgtr_n.begin(); in != m_vgtr_n.end(); ++in)
		{
			auto tn = *in;
			if (fabs(tn.get_nSigmaPion())>3.0 && fabs(tn.get_nSigmaProton())>3.0) continue;

			StPhysicalHelixD helix_tp = tp.get_helix();
			StPhysicalHelixD helix_tn = tn.get_helix();
			pair< double, double > 	ss = helix_tp.pathLengths(helix_tn);
			double s_tp = ss.first;
			double s_tn = ss.second;
			double dca2_v0 = (helix_tp.at(s_tp)-helix_tn.at(s_tn)).mag();
			if (dca2_v0>1.20) continue;
			
			StThreeVectorD v3_v0position = ( helix_tp.at(s_tp)+helix_tn.at(s_tn) ) / 2.0;
			StThreeVectorD v3_tp = helix_tp.momentumAt(s_tp);
			StThreeVectorD v3_tn = helix_tn.momentumAt(s_tn);
			StLorentzVectorD v4_tp;
			v4_tp.setVect(v3_tp);
			StLorentzVectorD v4_tn;
			v4_tn.setVect(v3_tn);
			StThreeVectorD v3_decay = v3_v0position - pv_position;
			double openAngle = v3_tp.angle(v3_tn);
			double cosrp = cos(v3_decay.angle(v3_tp+v3_tn));
			double sinrp = sin(v3_decay.angle(v3_tp+v3_tn));
			double dcaV0 = (sinrp*v3_decay).mag2();

			if ( fabs(tp.get_nSigmaProton())<3.0 && fabs(tn.get_nSigmaPion()<3.0) )
			{
				v4_tp.setE(v3_tp*v3_tp + c_massProton*c_massProton);
				v4_tn.setE(v3_tn*v3_tn + c_massPion*c_massPion);
				StLorentzVectorD v4_v0 = v4_tp + v4_tn;
				double mass_v0 = v4_v0.m();
				if (mass_v0>1.06 && mass_v0<=1.18)
				{	
					m_cosrp_L.push_back(cosrp);
					m_openAngle_L.push_back(openAngle);
					m_id_Lp.push_back(tp.get_id());
					m_id_Lpi.push_back(tn.get_id());
					m_v3_L.push_back( TVector3(v4_v0.x(),v4_v0.y(),v4_v0.z()) );
					m_im_L.push_back(mass_v0);
					m_v0_L.push_back( TVector3(v3_v0position.x(),v3_v0position.y(),v3_v0position.z()) );
					m_dca2_L.push_back(dca2_v0);
					m_dcaV0_L.push_back(dcaV0);
				}
			}
			if ( fabs(tn.get_nSigmaProton())<3.0 && fabs(tp.get_nSigmaPion()<3.0) )
			{
				v4_tp.setE(v3_tp*v3_tp + c_massPion*c_massPion);
				v4_tn.setE(v3_tn*v3_tn + c_massProton*c_massProton);
				StLorentzVectorD v4_v0 = v4_tp + v4_tn;
				double mass_v0 = v4_v0.m();

				if (mass_v0>1.06 && mass_v0<=1.18)
				{
					m_cosrp_A.push_back(cosrp);
					m_openAngle_A.push_back(openAngle);
					m_id_Ap.push_back(tn.get_id());
					m_id_Api.push_back(tp.get_id());
					m_v3_A.push_back( TVector3(v4_v0.x(),v4_v0.y(),v4_v0.z()) );
					m_im_A.push_back(mass_v0);
					m_v0_A.push_back( TVector3(v3_v0position.x(),v3_v0position.y(),v3_v0position.z()) );
					m_dca2_A.push_back(dca2_v0);
					m_dcaV0_A.push_back(dcaV0);
				}
			}
			if ( fabs(tn.get_nSigmaPion())<3.0 && fabs(tp.get_nSigmaPion()<3.0) )
			{
				v4_tp.setE(v3_tp*v3_tp + c_massPion*c_massPion);
				v4_tn.setE(v3_tn*v3_tn + c_massPion*c_massPion);
				StLorentzVectorD v4_v0 = v4_tp + v4_tn;
				double mass_v0 = v4_v0.m();
				if (cosrp<0.98) continue;
				if (dca2_v0>1.0) continue;
				if (tp.get_dca()<0.1) continue;
				if (tn.get_dca()<0.1) continue;
				if (v3_decay.mag()<1.0) continue;
				if (dcaV0>0.8) continue;

				if (mass_v0>0.42 && mass_v0<=0.58)
				{
					m_cosrp_K.push_back(cosrp);
					m_openAngle_K.push_back(openAngle);
					m_id_Kp.push_back(tp.get_id());
					m_id_Kpi.push_back(tn.get_id());
					m_v3_K.push_back( TVector3(v4_v0.x(),v4_v0.y(),v4_v0.z()) );
					m_im_K.push_back(mass_v0);
					m_v0_K.push_back( TVector3(v3_v0position.x(),v3_v0position.y(),v3_v0position.z()) );
					m_dca2_K.push_back(dca2_v0);
					m_dcaV0_K.push_back(dcaV0);
				}
			}
		}
	}
	m_cosrp_L.shrink_to_fit();
	m_openAngle_L.shrink_to_fit();
	m_id_Lp.shrink_to_fit();
	m_id_Lpi.shrink_to_fit();
	m_v3_L.shrink_to_fit();
	m_im_L.shrink_to_fit();
	m_v0_L.shrink_to_fit();
	m_dca2_L.shrink_to_fit();

	m_cosrp_A.shrink_to_fit();
	m_openAngle_A.shrink_to_fit();
	m_id_Ap.shrink_to_fit();
	m_id_Api.shrink_to_fit();
	m_v3_A.shrink_to_fit();
	m_im_A.shrink_to_fit();
	m_v0_A.shrink_to_fit();
	m_dca2_A.shrink_to_fit();

	m_cosrp_K.shrink_to_fit();
	m_openAngle_K.shrink_to_fit();
	m_id_Kp.shrink_to_fit();
	m_id_Kpi.shrink_to_fit();
	m_v3_K.shrink_to_fit();
	m_im_K.shrink_to_fit();
	m_v0_K.shrink_to_fit();
	m_dca2_K.shrink_to_fit();

	m_OutTree->Fill();

	return kStOk;
}







