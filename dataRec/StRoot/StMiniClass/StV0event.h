#ifndef _StV0event_
#define _StV0event_


//#include "TObject.h"
//#include "TArrayI.h"
//#include "StarClassLibrary/SystemOfUnits.h"
//#include "StarClassLibrary/StHelix.hh"
//#include "StarClassLibrary/StThreeVectorF.hh"
//#include "StarClassLibrary/StPhysicalHelixD.hh"
//#include "StMuDSTMaker/COMMON/StMuTrack.h"
//#include "StMuDSTMaker/COMMON/StMuBTofHit.h"
//#include "StMuDSTMaker/COMMON/StMuProbPidTraits.h"
//#include "StMuDSTMaker/COMMON/StMuBTofPidTraits.h"
//#include "StBTofUtil/StV0TofCorrection.h"

#include "StJetCandidate.h"
#include "StJetEvent.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "StMiniTrack.h"

class StV0event : public TObject {

private:
  //TLorentzVector m_p;

  StMiniTrack m_dauPos;
  TVector3 m_pdca_dauPos;
  StMiniTrack m_dauNeg;
  TVector3 m_pdca_dauNeg;

  TVector3 m_p;

  double m_dca2;
  double m_dcaV0;
  double m_cosrp;
  TVector3 m_endPosition;
  TVector3 m_startPosition;
  double m_dl;

  // Jet association variables
  int m_index;
  int m_type;
  double m_deta;
  double m_dphi;

public:
  StV0event();
  StV0event( StMiniTrack &t_dauPos, StMiniTrack &t_dauNeg, 
    StThreeVectorF PV );
  //StV0event( StV0event &mV0 );
  virtual ~StV0event() {};

  // Getter
  inline StMiniTrack get_track_dauPos() {return m_dauPos;}
  inline StMiniTrack get_track_dauNeg() {return m_dauNeg;}
  inline TVector3 get_pdca_dauPos() {return m_pdca_dauPos;}
  inline TVector3 get_pdca_dauNeg() {return m_pdca_dauNeg;}
  inline TVector3 get_p() {return m_p;}
  //inline double get_im_dauPos() {return m_dauPosIm;}
  //iinline double get_im_dauNeg() {return m_dauNegIm;}
  inline int get_index() {return m_index;}
  inline int get_type() {return m_type;}
  inline double get_deta() {return m_deta;}
  inline double get_dphi() {return m_dphi;}
  inline double get_dr() {return sqrt(pow(m_deta,2)+pow(m_dphi,2));}
  inline double get_dca2() {return m_dca2;}
  inline double get_dcaV0() {return m_dcaV0;}
  inline double get_cosrp() {return m_cosrp;}
  inline double get_dl() {return m_dl;}
  inline TVector3 get_startPosition() {return m_startPosition;}
  inline TVector3 get_endPosition() {return m_endPosition;}

  // Setter
  void set_track_dauPos(StMiniTrack t) {m_dauPos = t;}
  void set_track_dauNeg(StMiniTrack t) {m_dauNeg = t;}
  void set_startPosition(StThreeVectorF PV) {m_startPosition.SetXYZ(PV.x(),PV.y(),PV.z());};

  const static double im_Proton = 0.938272;
  const static double im_Pion = 0.139570;
  const static double im_e = 0.000511;

  TLorentzVector rec_p4(double im_dauPos, double im_dauNeg);
  double rec_im(double im_dauPos, double im_dauNeg);
  double* rec_im_Lambda(bool useTOF = false);
  double* rec_im_antiLambda(bool useTOF = false);
  double* rec_im_K0s(bool useTOF = false);
  double* rec_im_2e(bool useTOF = false);
  double rec_ct(double im_dauPos, double im_dauNeg);
  double rec_ct_Lambda(bool useTOF = false);
  double rec_ct_antiLambda(bool useTOF = fmalse);
  double rec_ct_K0s(bool useTOF = false);
  double rec_ct_2e(bool useTOF = false);
  void associate_jet(StJetEvent* jet_evt);

  ClassDef(StV0event,0)
};

#endif

