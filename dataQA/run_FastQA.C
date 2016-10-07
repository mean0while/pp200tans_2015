
void run_FastQA( int nEvts=10, const char* fin_name="in.root", const char *fout_name="out.root")
{
    TDatime *cTime = new TDatime;
    cout << "\n>>>======--->\tStart Date and Time\t<---======<<<\n";
    cTime->Print();
    cout << endl;

    cout << ">>>======--->\tMuDst chain file : " << fin_name << endl;
    cout << ">>>======--->\tOutput file : " << fout_name << endl;
    cout << endl;

    // Load libraries
    gROOT->Macro("loadMuDst.C");
    gROOT->Macro("LoadLogger.C");

    gSystem->Load("StarMagField");
    gSystem->Load("StMagF");
    gSystem->Load("StTpcDb");
    gSystem->Load("StDetectorDbMaker");
    gSystem->Load("StDbUtilities");
//  gSystem->Load("StMcEvent");
//  gSystem->Load("StMcEventMaker");
//  gSystem->Load("StMCAsymMaker");

    gSystem->Load("StDaqLib");
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StEpcMaker");
//  gSystem->Load("StEmcSimulatorMaker");
    gSystem->Load("StDbLib");
    gSystem->Load("StDbBroker");
    gSystem->Load("St_db_Maker");
    gSystem->Load("StEEmcUtil");
    gSystem->Load("StEEmcDbMaker");
    gSystem->Load("StTriggerUtilities");
    gSystem->Load("StTriggerFilterMaker");

//  gSystem->Load("StJetEvent");
    gSystem->Load("StSpinDbMaker");
    gSystem->Load("StEmcTriggerMaker");
    gSystem->Load("StBTofUtil");

    gSystem->Load("StFastQaMaker");

    cout << ">>>======--->\tLoading Libraries Done." << endl;

    StChain *chain = new StChain;

    StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",fin_name,"",1000,"MuDst");
    muDstMaker->SetStatus("*",0);
    muDstMaker->SetStatus("MuEventAll" ,1);
    muDstMaker->SetStatus("EmcAll" ,1);

    StMuDbReader* db = StMuDbReader::instance();

    StTriggerFilterMaker* filterMaker = new StTriggerFilterMaker;
    filterMaker->SetDebug(0);
    filterMaker->addTrigger(480202);  // BHT1*VPDMB-30
    filterMaker->addTrigger(480206);  // BHT1*VPDMB-30-nobsmd
    filterMaker->addTrigger(480404);  // JP1
    filterMaker->addTrigger(480414);
    filterMaker->addTrigger(480401);  // JP2
    filterMaker->addTrigger(480411);
    filterMaker->addTrigger(480405);  // JP2*L2JetHigh
    filterMaker->addTrigger(480415);

    St_db_Maker *dbMk = new St_db_Maker("StarDb", "MySQL:StarDb");
    StSpinDbMaker *spDb = new StSpinDbMaker("spinDb");

    StEEmcDbMaker* eemcDb = new StEEmcDbMaker;
    StEmcADCtoEMaker* adc2e = new StEmcADCtoEMaker;
    adc2e->saveAllStEvent(true);

    StFastQaMaker *fastQaMaker = new StFastQaMaker("fastQa", muDstMaker, fout_name);

    chain->SetDebug(0);
    chain->Init();
    chain->EventLoop(1,nEvts);

/*
    for( int iE=1; iE<=nEvts; ++iE ) {
        chain->Clear();
        int iStat = chain->Make(iE);
        if(iStat==2) cout << "\n>>>======--->\tLast  event processed. \n";
        if(iStat==3) cout << "\n>>>======--->\tError  event processed. \n";
        if(iStat != kStOk && iStat != kStSkip) break;
    }// EOF or input error
*/  
    chain->Finish();

    TDatime *cTimeE = new TDatime;
    cout << "\n>>>======--->\tEnding Date and Time\t<---======<<<\n";
    cTimeE->Print();
    cout << endl;

    cout << ">>>======--->\tTotal\t<---======<<<\n";
    cout << ">>>\tTotal Events :\t" << chain->GetNTotal() << endl;
    cout << ">>>\tTotal Time :\t" << cTimeE->Get()-cTime->Get() << endl;
    cout << ">>>\tTime per event: \t" << float(cTimeE->Get()-cTime->Get())/float(chain->GetNTotal()) << endl;
    cout << endl;
    delete chain;
    cout << ">>>======--->\tJob Finished." << endl;
}
