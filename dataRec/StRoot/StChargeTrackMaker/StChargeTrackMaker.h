#ifndef __StChargeTrackMaker_h__
#define __StChargeTrackMaker_h__

#include "myClass/miniTrack.h"
#include "StMaker.h"


class TFile;
class TTree;
class StMuDst;
class StMuDstMaker;
class StSpinDbMaker;
class StTriggerSimuMaker;
class miniTrack;

using namespace std;


class StChargeTrackMaker : public StMaker {
private:
	StMuDst             *m_MuDst;
	StMuDstMaker        *m_MuDstMaker;
	StSpinDbMaker       *m_SpinDbMaker;
	StTriggerSimuMaker  *m_TrigSimuMaker;

	const char *m_OutFn;
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
	int m_npv;
	int m_npvrank[100];
	double m_npvz[100];
	int m_nptr;
	int m_ngtr;
	int m_trig[5];
	int m_IsVPD;
	int m_IsJP1;
	int m_IsJP2;
	int m_IsL2J;

	vector<miniTrack> m_vptr_p;
	vector<miniTrack> m_vptr_n;
	vector<miniTrack> m_vgtr_p;
	vector<miniTrack> m_vgtr_n;

	TH1D *mH_pvz0;
	TH1D *mH_pvz1;

public:
	StChargeTrackMaker(const char *name, StMuDstMaker *muDstMaker, const char *out_fn);

	virtual int Init();
	virtual int Make();
	virtual int Finish();

	void InitFile(void);
	void FinishFile(void);
	void InitTree(const char *tName);
	void vClear(void);
	bool checkTrack( StMuTrack* );

};

#endif