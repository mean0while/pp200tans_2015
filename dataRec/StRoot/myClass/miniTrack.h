#ifndef __miniTrack__
#define __miniTrack__


#include "TObject.h"
#include "TArrayI.h"
#include "StarClassLibrary/SystemOfUnits.h"
#include "StarClassLibrary/StHelix.hh"
#include "StarClassLibrary/StThreeVectorD.hh"
#include "StarClassLibrary/StPhysicalHelixD.hh"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuBTofHit.h"
#include "StMuDSTMaker/COMMON/StMuProbPidTraits.h"
#include "StMuDSTMaker/COMMON/StMuBTofPidTraits.h"
#include "StBTofUtil/StV0TofCorrection.h"


class miniTrack : public TObject {

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
  miniTrack();
  miniTrack( StMuTrack *muTrack );
  miniTrack( const miniTrack &miTrack );
  virtual ~miniTrack() {};

  int get_id() const {return m_id;}
  int get_type() const {return m_type;}
  int get_flag() const {return m_flag;}
  int get_charge() const {return m_charge;}
  int get_nHitsFit() const {return m_nHitsFit;}
  int get_nHitsPos() const {return m_nHitsPos;}
  double get_dca() const {return m_dca;}
  double get_dEdx() const {return m_dEdx;}
  double get_nSigmaElectron() const {return m_nSigmaElectron;}
  double get_nSigmaKaon() const {return m_nSigmaKaon;}
  double get_nSigmaPion() const {return m_nSigmaPion;}
  double get_nSigmaProton() const {return m_nSigmaProton;}
  double get_pidElectron() const {return m_pidElectron;}
  double get_pidKaon() const {return m_pidKaon;}
  double get_pidPion() const {return m_pidPion;}
  double get_pidProton() const {return m_pidProton;}
  double get_yPidElectron() const {return m_yPid[0];}
  double get_yPidProton() const {return m_yPid[1];}
  double get_yPidKaon() const {return m_yPid[2];}
  double get_yPidPion() const {return m_yPid[3];}
  int get_matchTOF() const {return m_matchTOF;}
  double get_betaTOF() const {return m_betaTOF;}
  int get_matchBEMC() const {return m_matchBEMC;}
  double get_energyBEMC() const {return m_energyBEMC;}
  int get_towerBEMC(int index) const {return m_towerBEMC.At(index);}
  StThreeVectorD get_p_pv() const {return m_p_pv;}
  StThreeVectorD get_p_first() const {return m_p_first;}
  StThreeVectorD get_p_last() const {return m_p_last;}
  StPhysicalHelixD get_helix() const {return m_helix;}

  bool isGood();
  double m2TOF();

  ClassDef(miniTrack,1)
};

#endif __miniTrack__

