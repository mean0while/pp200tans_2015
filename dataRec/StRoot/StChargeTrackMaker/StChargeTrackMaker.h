
#ifndef __StChargeTrackMaker_h__
#define __StChargeTrackMaker_h__


#include "StMaker.h"
#include "StMiniClass/StMiniTrack.h"


class TFile;
class TTree;
class StMuDst;
class StMuDstMaker;
class StSpinDbMaker;
class StTriggerSimuMaker;
class StMiniTrack;

using namespace std::vector;
using namespace std::map;


class StChargeTrackMaker : public StMaker {
private:
	static double c_massPion     = 0.139570;
	static double c_massProton   = 0.938272;
	static double c_masskaon     = 0.493677;
	static double c_massElectron = 0.000511;

	StMuDst             *m_MuDst;
	StMuDstMaker        *m_MuDstMaker;
	StSpinDbMaker       *m_SpinDbMaker;
	StTriggerSimuMaker  *m_TrigSimuMaker;

	const char *m_OutFn;
	std::vector<int> pool_trigID;

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

	std::vector<int>    m_nPVrank;
	std::vector<double> m_nPVz;

	std::map<int, bool> m_Trig;
	std::map<int, bool> m_TrigSoft;

	std::vector<StMiniTrack> m_vptr_p;
	std::vector<StMiniTrack> m_vptr_n;
	std::vector<StMiniTrack> m_vgtr_p;
	std::vector<StMiniTrack> m_vgtr_n;

	std::vector<double>   m_cosrp_L;
	std::vector<double>   m_openAngle_L;
	std::vector<double>   m_im_L;
	std::vector<int>      m_id_Lp;
	std::vector<int>      m_id_Lpi;
	std::vector<TVector3> m_v3_L;
	std::vector<TVector3> m_v0_L;
	std::vector<double>   m_dca2_L;

	std::vector<double>   m_cosrp_A;
	std::vector<double>   m_openAngle_A;
	std::vector<double>   m_im_A;
	std::vector<int>      m_id_Ap;
	std::vector<int>      m_id_Api;
	std::vector<TVector3> m_v3_A;
	std::vector<TVector3> m_v0_A;
	std::vector<double>   m_dca2_A;

	std::vector<double>   m_cosrp_K;
	std::vector<double>   m_openAngle_K;
	std::vector<double>   m_im_K;
	std::vector<int>      m_id_Kp;
	std::vector<int>      m_id_Kpi;
	std::vector<TVector3> m_v3_K;
	std::vector<TVector3> m_v0_K;
	std::vector<double>   m_dca2_K;

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