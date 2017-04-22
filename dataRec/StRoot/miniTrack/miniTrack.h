#ifndef __miniTrack__
#define __miniTrack__


#include "TObject.h"
#include "TArrayI.h"
#include "StarClassLibrary/StThreeVector.hh"
#include "StarClassLibrary/StPhysicalHelix.hh"

class miniTrack : public TObject {

private:
  int m_id;
  int m_flag;
  int m_type;
  int m_charge;

  // TPC infor
  int m_nHitsFit;
  int m_nHitsPos;

  float m_dEdx;
  float m_nSigmaElectron;
  float m_nSigmaKaon;
  float m_nSigmaPion;
  float m_nSigmaProton;
  float m_pidElectron;
  float m_pidKaon;
  float m_pidPion;
  float m_pidProton;
  float m_yPid[4];

  float m_dca;
  StThreeVectorF m_p_pv;
  StThreeVectorF m_p_first;
  StThreeVectorF m_p_last;
  StPhysicalHelixD m_helix;

  // TOF infor
  int m_matchTOF;
  float m_betaTOF;

  // EMC infor
  int m_matchBEMC;
  float m_energyBEMC;
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
  float get_dca() const {return m_dca;}
  float get_dEdx() const {return m_dEdx;}
  float get_nSigmaElectron() const {return m_nSigmaElectron;}
  float get_nSigmaKaon() const {return m_nSigmaKaon;}
  float get_nSigmaPion() const {return m_nSigmaPion;}
  float get_nSigmaProton() const {return m_nSigmaProton;}
  float get_pidElectron() const {return m_pidElectron;}
  float get_pidKaon() const {return m_pidKaon;}
  float get_pidPion() const {return m_pidPion;}
  float get_pidProton() const {return m_pidProton;}
  float get_yPidElectron() const {return m_yPid[0];}
  float get_yPidProton() const {return m_yPid[1];}
  float get_yPidKaon() const {return m_yPid[2];}
  float get_yPidPion() const {return m_yPid[3];}
  int get_matchTOF() const {return m_matchTOF;}
  float get_betaTOF() const {return m_betaTOF;}
  int get_matchBEMC() const {return m_matchBEMC;}
  float get_energyBEMC() const {return m_energyBEMC;}
  int get_towerBEMC(int index) const {return m_towerBEMC.At(index);}
  StThreeVectorF get_p_pv() const {return m_p_pv;}
  StThreeVectorF get_p_first() const {return m_p_first;}
  StThreeVectorF get_p_last() const {return m_p_last;}
  StPhysicalHelixD get_helix() const {return m_helix;}

  bool isGood();
  float m2TOF();

  ClassDef(miniTrack,1)
};

#endif

