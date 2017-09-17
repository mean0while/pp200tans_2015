
#ifndef __StChargeTrackMaker_h__
#define __StChargeTrackMaker_h__


#include "StMaker.h"
#include "StMiniClass/StMiniTrack.h"
#include <vector>
#include <map>
#include "TVector3.h"


class TFile;
class TTree;
class StMuDst;
class StMuDstMaker;
class StSpinDbMaker;
class StTriggerSimuMaker;
class StMiniTrack;
class TVector3;

using namespace std;

static double c_massPion     = 0.139570;
static double c_massProton   = 0.938272;
static double c_masskaon     = 0.493677;
static double c_massElectron = 0.000511;


class StChargeTrackMaker : public StMaker {
private:		

	StMuDst             *m_MuDst;
	StMuDstMaker        *m_MuDstMaker;
	StSpinDbMaker       *m_SpinDbMaker;
	StTriggerSimuMaker  *m_TrigSimuMaker;

	const char *m_OutFn;
	vector<int> pool_trigID;

	TFile *m_OutFile;

	TTree *m_OutTree;

	int m_runID;
	int m_evtID;
	int m_spin;
	int m_spinbit;
	int m_spinbit8;
	int m_bx7;
	int m_bx48;
	double m_magn;
	double m_bbcrate;
	double m_zdcrate;
	int m_bbcTimebin;
	double m_pvx;
	double m_pvy;
	double m_pvz;
	int m_nptr;
	int m_ngtr;
	int m_npv;

	vector<int>    m_nPVrank;
	vector<double> m_nPVz;

	map<int, bool> m_Trig;
	map<int, bool> m_TrigSoft;

	vector<StMiniTrack> m_vptr_p;
	vector<StMiniTrack> m_vptr_n;
	vector<StMiniTrack> m_vgtr_p;
	vector<StMiniTrack> m_vgtr_n;

	vector<double>   m_cosrp_L;
	vector<double>   m_openAngle_L;
	vector<double>   m_im_L;
	vector<int>      m_id_Lp;
	vector<int>      m_id_Lpi;
	vector<TVector3> m_v3_L;
	vector<TVector3> m_v0_L;
	vector<double>   m_dca2_L;

	vector<double>   m_cosrp_A;
	vector<double>   m_openAngle_A;
	vector<double>   m_im_A;
	vector<int>      m_id_Ap;
	vector<int>      m_id_Api;
	vector<TVector3> m_v3_A;
	vector<TVector3> m_v0_A;
	vector<double>   m_dca2_A;

	vector<double>   m_cosrp_K;
	vector<double>   m_openAngle_K;
	vector<double>   m_im_K;
	vector<int>      m_id_Kp;
	vector<int>      m_id_Kpi;
	vector<TVector3> m_v3_K;
	vector<TVector3> m_v0_K;
	vector<double>   m_dca2_K;

	TH1D *m_H_pvz0;
	TH1D *m_H_pvz1;

public:
	StChargeTrackMaker(const char *name, StMuDstMaker *muDstMaker, const char *out_fn);

	virtual int Init();
	virtual int Make();
	virtual int Finish();

	void InitFile(void);
	void FinishFile(void);
	void InitTree();
	void vClear(void);
	void Add_poolTrigID(int tID);
	bool checkTrack( StMuTrack* );

	ClassDef(StChargeTrackMaker,1)

};

#endif