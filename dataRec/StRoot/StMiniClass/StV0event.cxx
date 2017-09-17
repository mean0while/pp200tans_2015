#include "StV0event.h"
#include "TROOT.h"
#include "TClass.h"


ClassImp(StV0event)

StV0event::StV0event(): TObject()
{
  m_dca2 = 999.9;
  m_dcaV0 = 999.9;
  m_cosrp = -999.9;
  m_dl = -999.9;
  m_ctau = -999.9;

  m_index = -1;
  m_type = -1;
  m_deta = 999.9;
  m_dphi = 999.9;
}

// Construction function using two daughter particle track.
// Only topological variables obtained.
StV0event::StV0event( StMiniTrack &indauPos, 
  StMiniTrack &indauNeg, StThreeVectorF PV ): TObject()
{
  m_dca2 = 999.9;
  m_dcaV0 = 999.9;
  m_cosrp = -999.9;
  m_dl = -999.9;
  m_ctau = -999.9;

  m_index = -1;
  m_type = -1;
  m_deta = 999.9;
  m_dphi = 999.9;

  m_dauPos = indauPos;
  m_dauNeg = indauNeg;
  StPhysicalHelixD helix_dauPos = m_dauPos.get_helix();
  StPhysicalHelixD helix_dauNeg = m_dauNeg.get_helix();

  pair<double,double> ss = helix_dauPos.pathLengths(helix_dauNeg);
  double sPos = ss.first;
  double sNeg = ss.second;
  StThreeVectorD t_pdca_dauPos = helix_dauPos.at(sPos);
  StThreeVectorD t_pdca_dauNeg = helix_dauPos.at(sNeg);
  StThreeVectorD V0 = (t_pdca_dauPos + t_pdca_dauNeg)/2.0;
  StThreeVectorD V0mp = V0 - PV;  // V0_distance to Primary Vetex
  m_pdca_dauPos.SetXYZ(t_pdca_dauPos.x(),t_pdca_dauPos.y(),t_pdca_dauPos.z());
  m_pdca_dauNeg.SetXYZ(t_pdca_dauNeg.x(),t_pdca_dauNeg.y(),t_pdca_dauNeg.z());

  m_endPosition.SetXYZ(V0.x(),V0.y(),V0.z());
  m_startPosition.SetXYZ(PV.x(),PV.y(),PV.z());

  StThreeVectorD tdca2 = helix_dauPos.at(sPos) - helix_dauNeg.at(sNeg);
  StThreeVectorD p_dauPos = helix_dauPos.momentumAt(sPos,magn*kilogauss);
  StThreeVectorD p_dauNeg = helix_dauNeg.momentumAt(sNeg,magn*kilogauss);
  StThreeVectorD p_mom = p_dauPos + p_dauNeg;
  double dot = p_mom.dot(V0mp);

  m_dcaV0 = sqrt(V0mp.mag2() - dot*dot/p_mom.mag2());
  m_cosrp = dot/(p_mom.mag()*V0mp.mag());
  m_dca2 = tdca2.mag();
  m_dl = V0mp.mag();
  m_p.SetXYZ(p_mom.x(),p_mom.y(),p_mom.z());
}


// V0 particle invariant mass reconstruction
TLorentzVector StV0event::rec_p4(double im_dauPos, double im_dauNeg)
{
  TLorentzVector p4_dauPos;
  TLorentzVector p4_dauNeg;
  p4_dauPos.SetVectM(m_pdca_dauPos,im_dauPos);
  p4_dauNeg.SetVectM(m_pdca_dauNeg,im_dauNeg);
  TLorentzVector p4_v0 = p4_dauPos + p4_dauNeg;

  return p4_v0;
}

double StV0event::rec_im(double im_dauPos, double im_dauNeg)
{
  TLorentzVector p4_v0 = rec_p4(im_dauPos, double im_dauNeg);

  return p4_v0.M();
}
double* StV0event::rec_im_Lambda(bool useTOF = false)
{
  if (fabs(m_dauPos.get_nSigmaProton())>3.0) return -999.9;
  if (fabs(m_dauNeg.get_nSigmaPion())>3.0) return -999.9;
  double im_dauPos = im_Proton;
  double im_dauNeg = im_Pion;
  if (useTOF)
  {
    double im2tof_dauPos = m_dauPos.m2TOF();
    if (im2tof_dauPos<0.0) return -999.9;
    double im2tof_dauNeg = m_dauNeg.m2TOF();
    if (im2tof_dauNeg<0.0) return -999.9;
    im_dauPos = sqrt(im2tof_dauPos);
    im_dauNeg = sqrt(im2tof_dauNeg);
  }
  double* im[3]={im_dauPos, im_dauNeg, rec_im(im_dauPos, im_dauNeg)};
  return im;
}
double* StV0event::rec_im_antiLambda(bool useTOF = false)
{
  if (fabs(m_dauPos.get_nSigmaPion())>3.0) return -999.9;
  if (fabs(m_dauNeg.get_nSigmaProton())>3.0) return -999.9;
  double im_dauPos = im_Pion;
  double im_dauNeg = im_Proton;
  if (useTOF)
  {
    double im2tof_dauPos = m_dauPos.m2TOF();
    if (im2tof_dauPos<0.0) return -999.9;
    double im2tof_dauNeg = m_dauNeg.m2TOF();
    if (im2tof_dauNeg<0.0) return -999.9;
    im_dauPos = sqrt(im2tof_dauPos);
    im_dauNeg = sqrt(im2tof_dauNeg);
  }
  double* im[3]={im_dauPos, im_dauNeg, rec_im(im_dauPos, im_dauNeg)};
  return im;
}
double* StV0event::rec_im_K0s(bool useTOF = false)
{
  if (fabs(m_dauPos.get_nSigmaPion())>3.0) return -999.9;
  if (fabs(m_dauNeg.get_nSigmaPion())>3.0) return -999.9;
  double im_dauPos = im_Pion;
  double im_dauNeg = im_Pion;
  if (useTOF)
  {
    double im2tof_dauPos = m_dauPos.m2TOF();
    if (im2tof_dauPos<0.0) return -999.9;
    double im2tof_dauNeg = m_dauNeg.m2TOF();
    if (im2tof_dauNeg<0.0) return -999.9;
    im_dauPos = sqrt(im2tof_dauPos);
    im_dauNeg = sqrt(im2tof_dauNeg);
  }
  double* im[3]={im_dauPos, im_dauNeg, rec_im(im_dauPos, im_dauNeg)};
  return im;
}
double* StV0event::rec_im_2e(bool useTOF = false)
{
  if (fabs(m_dauPos.get_nSigmaElectron())>3.0) return -999.9;
  if (fabs(m_dauNeg.get_nSigmaElectron())>3.0) return -999.9;
  double im_dauPos = im_e;
  double im_dauNeg = im_e;
  if (useTOF)
  {
    double im2tof_dauPos = m_dauPos.m2TOF();
    if (im2tof_dauPos<0.0) return -999.9;
    double im2tof_dauNeg = m_dauNeg.m2TOF();
    if (im2tof_dauNeg<0.0) return -999.9;
    im_dauPos = sqrt(im2tof_dauPos);
    im_dauNeg = sqrt(im2tof_dauNeg);
  }
  double* im[3]={im_dauPos, im_dauNeg, rec_im(im_dauPos, im_dauNeg)};
  return im;
}

double StV0event::rec_ct(double im_dauPos, double im_dauNeg)
{
  TLorentzVector p4_v0 = rec_p4(im_dauPos, double im_dauNeg);
  TVector3 bv = p4_v0.BoostVector();
  p4_v0.Boost(-1*bv);

  return p4_v0.T();
}
double StV0event::rec_ct_Lambda(bool useTOF = false)
{
  if (fabs(m_dauPos.get_nSigmaProton())>3.0) return -999.9;
  if (fabs(m_dauNeg.get_nSigmaPion())>3.0) return -999.9;
  double im_dauPos = im_Proton;
  double im_dauNeg = im_Pion;
  if (useTOF)
  {
    double im2tof_dauPos = m_dauPos.m2TOF();
    if (im2tof_dauPos<0.0) return -999.9;
    double im2tof_dauNeg = m_dauNeg.m2TOF();
    if (im2tof_dauNeg<0.0) return -999.9;
    im_dauPos = sqrt(im2tof_dauPos);
    im_dauNeg = sqrt(im2tof_dauNeg);
  }
  return rec_t(im_dauPos, im_dauNeg);
}
double StV0event::rec_ct_antiLambda(bool useTOF = false)
{
  if (fabs(m_dauPos.get_nSigmaPion())>3.0) return -999.9;
  if (fabs(m_dauNeg.get_nSigmaProton())>3.0) return -999.9;
  double im_dauPos = im_Pion;
  double im_dauNeg = im_Proton;
  if (useTOF)
  {
    double im2tof_dauPos = m_dauPos.m2TOF();
    if (im2tof_dauPos<0.0) return -999.9;
    double im2tof_dauNeg = m_dauNeg.m2TOF();
    if (im2tof_dauNeg<0.0) return -999.9;
    im_dauPos = sqrt(im2tof_dauPos);
    im_dauNeg = sqrt(im2tof_dauNeg);
  }
  return rec_t(im_dauPos, im_dauNeg);
}
double StV0event::rec_ct_K0s(bool useTOF = false)
{
  if (fabs(m_dauPos.get_nSigmaPion())>3.0) return -999.9;
  if (fabs(m_dauNeg.get_nSigmaPion())>3.0) return -999.9;
  double im_dauPos = im_Pion;
  double im_dauNeg = im_Pion;
  if (useTOF)
  {
    double im2tof_dauPos = m_dauPos.m2TOF();
    if (im2tof_dauPos<0.0) return -999.9;
    double im2tof_dauNeg = m_dauNeg.m2TOF();
    if (im2tof_dauNeg<0.0) return -999.9;
    im_dauPos = sqrt(im2tof_dauPos);
    im_dauNeg = sqrt(im2tof_dauNeg);
  }
  return rec_t(im_dauPos, im_dauNeg);
}
double StV0event::rec_ct_2e(bool useTOF = false)
{
  if (fabs(m_dauPos.get_nSigmaElectron())>3.0) return -999.9;
  if (fabs(m_dauNeg.get_nSigmaElectron())>3.0) return -999.9;
  double im_dauPos = im_e;
  double im_dauNeg = im_e;
  if (useTOF)
  {
    double im2tof_dauPos = m_dauPos.m2TOF();
    if (im2tof_dauPos<0.0) return -999.9;
    double im2tof_dauNeg = m_dauNeg.m2TOF();
    if (im2tof_dauNeg<0.0) return -999.9;
    im_dauPos = sqrt(im2tof_dauPos);
    im_dauNeg = sqrt(im2tof_dauNeg);
  }
  return rec_t(im_dauPos, im_dauNeg);
}

void StV0event::associate_jet(StJetEvent* jet_evt)
{
  double dr = 999.9;
  int nJet = jet_evt->numberOfJets();
  for (int i = 0; i < nJet; ++i)
  {
    StJetCandidate* jet = jet_evt(i);
    if (jet.detEta()<-0.7 || jet.detEta()>0.9) continue;
    if (jet.rt()>0.95 ) continue;
    TVector3 p_jet = jet->momentum();
    double tdr = m_p.DeltaR(p_jet);
    if (tdr<dr)
    {
      dr = tdr;
      m_deta = m_p.Eta() - p_jet.Eta();
      m_dphi = m_p.DeltaPhi(p_jet);
      m_index = i;
    }
  }
  if (m_index<0) return;
  StJetCandidate* jet = jet_evt(m_index);
  bool flag_pos = false;
  bool flag_neg = false;
  for (int i = 0; i < jet->numberOfTracks(); ++i)
  {
    StJetTrack* jtrack = jet.track(i);
    if (jtrack->id() == m_dauPos.get_id()) flag_pos = true;
    if (jtrack->id() == m_dauNeg.get_id()) flag_neg = true;
  }
  if (flag_pos==false && flag_neg==false) m_type = 0;
  if (flag_pos==true  && flag_neg==false) m_type = 1;
  if (flag_pos==false && flag_neg==true ) m_type = 2;
  if (flag_pos==true  && flag_neg==true ) m_type = 3;
  return;
}



