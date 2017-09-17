#ifndef __StV0recMaker_h__
#define __StV0recMaker_h__

//#include "StMiniClass/StMiniTrack.h"
//#include "StMiniClass/StV0event.h"
#include "StMaker.h"
#include <vector>


class TFile;
class TTree;
class StMuDst;
class StMuDstMaker;
class StSpinDbMaker;
class StTriggerSimuMaker;
class StMiniTrack;
class StV0event;

using namespace std;


class StV0recMaker : public StMaker {
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
	//int m_IsVPD;
	int m_IsJP1;
	int m_IsJP2;
	int m_IsL2J;

	vector<StMiniTrack> m_vptr_p;
	vector<StMiniTrack> m_vptr_n;
	vector<StMiniTrack> m_vgtr_p;
	vector<StMiniTrack> m_vgtr_n;
	vector<StV0event> m_v0event;

	TH1D *mH_pvz0;
	TH1D *mH_pvz1;

public:
	StV0recMaker(const char *name, StMuDstMaker *muDstMaker, const char *out_fn);

	virtual int Init();
	virtual int Make();
	virtual int Finish();

	void InitFile(void);
	void FinishFile(void);
	void InitTree();
	void vClear(void);
	bool checkTrack( StMuTrack* );
	bool passV0( StV0event );

	ClassDef(StV0recMaker,1)

};

#endif