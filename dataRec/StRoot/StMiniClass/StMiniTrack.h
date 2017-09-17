#ifndef _StMiniTrack_
#define _StMiniTrack_


#include "TObject.h"
#include "TArrayI.h"
#include "StarClassLibrary/SystemOfUnits.h"
#include "StarClassLibrary/StHelix.hh"
#include "StarClassLibrary/StThreeVectorF.hh"
#include "StarClassLibrary/StPhysicalHelixD.hh"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuBTofHit.h"
#include "StMuDSTMaker/COMMON/StMuProbPidTraits.h"
#include "StMuDSTMaker/COMMON/StMuBTofPidTraits.h"
#include "StBTofUtil/StV0TofCorrection.h"


class StMiniTrack : public TObject {

private:
  int m_id;
  int m_flag;
  int m_type;
  int m_charge;

  // TPC infor
  int m_nHitsFit;
  int m_nHitsPos;

  double m_dEdx;
  double m_nSigmaElectron;
  double m_nSigmaKaon;
  double m_nSigmaPion;
  double m_nSigmaProton;
  double m_pidElectron;
  double m_pidKaon;
  double m_pidPion;
  double m_pidProton;
  double m_yPid[4];

  StThreeVectorF m_dca;
  StThreeVectorF m_p_pv;
  StThreeVectorF m_p_first;
  StThreeVectorF m_p_last;
  StPhysicalHelixD m_helix;

  // TOF infor
  int m_matchTOF;
  double m_betaTOF;

  // EMC infor
  int m_matchBEMC;
  double m_energyBEMC;
  TArrayI m_towerBEMC;

public:
  StMiniTrack();
  StMiniTrack( StMuTrack *muTrack );
  StMiniTrack( StMiniTrack &mT );
  virtual ~StMiniTrack() {};

  inline int get_id() const {return m_id;}
  inline int get_type() const {return m_type;}
  inline int get_flag() const {return m_flag;}
  inline int get_charge() const {return m_charge;}
  inline int get_nHitsFit() const {return m_nHitsFit;}
  inline int get_nHitsPos() const {return m_nHitsPos;}

  inline double get_dEdx() const {return m_dEdx;}
  inline double get_nSigmaElectron() const {return m_nSigmaElectron;}
  inline double get_nSigmaKaon() const {return m_nSigmaKaon;}
  inline double get_nSigmaPion() const {return m_nSigmaPion;}
  inline double get_nSigmaProton() const {return m_nSigmaProton;}
  inline double get_pidElectron() const {return m_pidElectron;}
  inline double get_pidKaon() const {return m_pidKaon;}
  inline double get_pidPion() const {return m_pidPion;}
  inline double get_pidProton() const {return m_pidProton;}
  inline double get_yPidElectron() const {return m_yPid[0];}
  inline double get_yPidProton() const {return m_yPid[1];}
  inline double get_yPidKaon() const {return m_yPid[2];}
  inline double get_yPidPion() const {return m_yPid[3];}
  inline int get_matchTOF() const {return m_matchTOF;}
  inline double get_betaTOF() const {return m_betaTOF;}
  inline int get_matchBEMC() const {return m_matchBEMC;}
  inline double get_energyBEMC() const {return m_energyBEMC;}
  inline int get_towerBEMC(int index) const {return m_towerBEMC.At(index);}
  
  inline StThreeVectorF get_dca() const {return m_dca;}
  inline StThreeVectorF get_p_pv() const {return m_p_pv;}
  inline StThreeVectorF get_p_first() const {return m_p_first;}
  inline StThreeVectorF get_p_last() const {return m_p_last;}
  inline StPhysicalHelixD get_helix() const {return m_helix;}

  inline float dcaXY() const {return m_dca.perp();}
  bool isGood();
  inline bool beyond_nSigma()
  {
    if (fabs(m_nSigmaKaon)>3.0 && fabs(m_nSigmaPion)>3.0 && fabs(m_nSigmaProton)>3.0 && fabs(m_nSigmaElectron)>3.0)
      return true;
    return false;
  }
  double m2TOF();

  ClassDef(StMiniTrack,2)
};

#endif

