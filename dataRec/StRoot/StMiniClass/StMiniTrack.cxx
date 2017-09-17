
#include "StMiniTrack.h"
#include "TROOT.h"
#include "TClass.h"


ClassImp(StMiniTrack)

StMiniTrack::StMiniTrack(): TObject()
{
  m_id = 0;
  m_type = 0; 
  m_flag = 0;
  m_charge = 0;
  m_nHitsFit = 0;
  m_nHitsPos = 0;

  m_dEdx = 0.0;
  m_nSigmaElectron = -999.9;
  m_nSigmaKaon = -999.9;
  m_nSigmaPion = -999.9;
  m_nSigmaProton = -999.9;
  m_pidElectron = -999.9;
  m_pidKaon = -999.9;
  m_pidPion = -999.9;
  m_pidProton = -999.9;

  m_yPid[0] = -999.9;
  m_yPid[1] = -999.9;
  m_yPid[2] = -999.9;
  m_yPid[3] = -999.9;

  m_matchTOF = 0;
  m_betaTOF = -999.9;
  m_matchBEMC = 0;
  m_energyBEMC = -999.9;
}

StMiniTrack::StMiniTrack( StMuTrack *mt ): TObject()
{
  m_id = mt->id();
  m_flag = mt->flag();
  m_type = mt->type();
  m_charge = mt->charge();
  m_nHitsFit = mt->nHitsFit();
  m_nHitsPos = mt->nHitsPoss();

  m_dEdx = mt->dEdx();
  m_nSigmaElectron = mt->nSigmaElectronFit();
  m_nSigmaKaon = mt->nSigmaKaonFit();
  m_nSigmaPion = mt->nSigmaPionFit();
  m_nSigmaProton = mt->nSigmaProtonFit();
  m_pidElectron = mt->pidProbElectron();
  m_pidKaon = mt->pidProbKaon();
  m_pidPion = mt->pidProbPion();
  m_pidProton = mt->pidProbProton();

  m_helix = mt->helix();
  m_dca = mt->dca();
  m_p_pv = mt->p();
  m_p_first = mt->firstPoint();
  m_p_last = mt->lastPoint();

  StMuProbPidTraits yMuPid = mt->probPidTraits();
  m_yPid[0] = yMuPid.probability(0);
  m_yPid[1] = yMuPid.probability(1);
  m_yPid[2] = yMuPid.probability(2);
  m_yPid[3] = yMuPid.probability(3);

  StMuBTofPidTraits btofPid = mt->btofPidTraits();
  m_matchTOF = btofPid.matchFlag();
  m_betaTOF = btofPid.beta();

  m_matchBEMC = mt->matchBEMC();
  m_energyBEMC = mt->energyBEMC();
  m_towerBEMC = mt->getTower(0,1);
}

StMiniTrack::StMiniTrack( StMiniTrack &mt )
{
  m_id = mt->get_id();
  m_flag = mt->get_flag();
  m_type = mt->get_type();
  m_charge = mt->get_charge();
  m_nHitsFit = mt->get_nHitsFit();
  m_nHitsPos = mt->get_nHitsPoss();

  m_dEdx = mt->get_dEdx();
  m_nSigmaElectron = mt->get_nSigmaElectron();
  m_nSigmaKaon = mt->get_nSigmaKaon();
  m_nSigmaPion = mt->get_nSigmaPion();
  m_nSigmaProton = mt->get_nSigmaProton();
  m_pidElectron = mt->get_pidElectron();
  m_pidKaon = mt->get_pidKaon();
  m_pidPion = mt->get_pidPion();
  m_pidProton = mt->get_pidProton();

  m_helix = mt->get_helix();
  m_dca = mt->get_dca();
  m_p_pv = mt->get_p_pv();
  m_p_first = mt->get_p_first();
  m_p_last = mt->get_p_last();

  m_yPid[0] = mt.get_yPidElectron();
  m_yPid[1] = mt.get_yPidProton();
  m_yPid[2] = mt.get_yPidKaon();
  m_yPid[3] = mt.get_yPidPion();

  StMuBTofPidTraits btofPid = mt->btofPidTraits();
  m_matchTOF = btofPid.matchFlag();
  m_betaTOF = btofPid.beta();

  m_matchBEMC = mt->matchBEMC();
  m_energyBEMC = mt->energyBEMC();
  m_towerBEMC = mt->getTower(0,1);
}

bool StMiniTrack::isGood()
{
  if (m_id<=0 || m_flag<=0 || m_flag>=1000 || m_nHitsFit<15)
    return false;
  if (float(m_nHitsFit)/float(m_nHitsPos)<0.51)
    return false;
  return true;
}

double StMiniTrack::m2TOF()
{
  double tm2 = -999.9;
  if (m_matchTOF<1 || m_betaTOF<0.0 || m_betaTOF>1.0)
    return tm2;
  // M^2 = p^2[(1/beta)^2 - 1]
  double p2 = m_p_last.mag2();
  double b2 = m_betaTOF * m_betaTOF;
  tm2 = p2 * ( (1 / b2) -1 );
  return tm2;
}