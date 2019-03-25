// This is a fiTQun efficiency class. Basically a glorified plotting script...
// It is fairly simplistic, but it will be expanded and generalized later.
#include "fitqunEfficiency.h"


// Boring default constructor
fqeff::fqeff(){
  outputfilename = "outputfilename";
  nbins = 10;

  T2KCUTnumuDis = false;
  T2KCUTnueApp = false;
  T2KCUTnueCC1pi = false;
}

// Overload the constructor with core
fqeff::fqeff(muenalysis *coreIn){
  // Set core
  core = coreIn;

  // Sort out the file name
  outputfilename = core->getOutputName();
  std::size_t pos;
  pos = outputfilename.find(".root");
  outputfilename = outputfilename.substr(0, pos);

  // Get the number of files
  nfiles = core->getNFiles();

  // Initialize plots and chains
  plots1D = new TH1D*[nfiles];
  chains = new TChain*[nfiles];

  // Now REALLY initialize each chain and it's parser
  for(int i = 0; i < nfiles; ++i){
    chains[i] = new TChain("h1");
    chains[i]->Add((core->getFileName(i)).c_str());

    // Make a new parser and add to a vector (one parser per TChain)
    fqbrparser *parserdummy = new fqbrparser();
    parser.push_back(parserdummy);
  }

  // The rest of the constructor...
  nbins = 10;
  T2KCUTnumuDis = false;
  T2KCUTnueApp = false;
  T2KCUTnueCC1pi = false;
}

// Sets core 
void fqeff::setCore(muenalysis *coreIn){
  // Set the core
  core = coreIn;

  // Sort out the output filename
  outputfilename = core->getOutputName();
  std::size_t pos;
  pos = outputfilename.find(".root");
  outputfilename = outputfilename.substr(0, pos);

  // Get the number of files
  nfiles = core->getNFiles();

  // Initialize plots and chains
  plots1D = new TH1D*[nfiles];
  chains = new TChain*[nfiles];

  // Now REALLY initialize each chain and it's parser
  for(int i = 0; i < nfiles; ++i){
    chains[i] = new TChain("h1");
    chains[i]->Add((core->getFileName(i) + "/h1").c_str());
    std::cout << "Adding " << (core->getFileName(i) + "/h2").c_str() << std::endl;

    // Make a new parser and add to a vector (one parser per TChain)
    fqbrparser *parserdummy = new fqbrparser();
    parser.push_back(parserdummy);
  }
}

// Sets the cuts.
void fqeff::setCuts(bool numu, bool nue, bool nuecc1pi){
  T2KCUTnumuDis = numu;
  T2KCUTnueApp = nue;
  T2KCUTnueCC1pi = nuecc1pi;

  if(numu)
    std::cout << "FQEFF: Running numu disappearance cuts!" << std::endl;
  if(nue)
    std::cout << "FQEFF: Running nue appearance cuts!" << std::endl;
  if(nuecc1pi)
    std::cout << "FQEFF: Running nueCC1pi sample cuts!" << std::endl;
}

//void fqeff::plotTemplate(TH1D *dummy_TH1D){
void fqeff::plotTemplate(){

  // Some initial asthetics
  // Maybe move it to the config...
  gStyle->SetOptStat(0);
  gStyle->SetPalette(51,0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetTitleX(0.1f);
  gStyle->SetTitleW(0.8f);

  for(int i = 0; i < nfiles; ++i){
//    plots1D[i] = (TH1D*)dummy_TH1D->Clone();

    // This operator actually sets the branches. 
    (*parser[i])(chains[i]);
  }

  // Some sanity checks, just in case
  for(int i = 1; i < nfiles; ++i){
    if(chains[i-1]->GetEntries()!= chains[i]->GetEntries()){
      std::cout << "FLAG: number of entries don't math between: " << std::endl;
    }
  }


}

bool fqeff::T2KCuts(int file){
  bool check = true;
  if((T2KCUTnumuDis)&&(!parser[file]->isT2KnumuDis()))
    check = false;
  if((T2KCUTnueApp)&&(!parser[file]->isT2KnueApp()))
    check = false;
  if((T2KCUTnueCC1pi)&&(!parser[file]->isT2KnueCC1pi()))
    check = false;
  return check;
}

std::string fqeff::getWallDef(int wall){
  switch(wall){
    case 0:
      return "wall";
    case 1:
      return "fqwall";
    case 2:
      return "fqtowall";
    case 3:
      return "fqfromwall";
    default:
      return "ERROR";
  }
}


// Makes fqwall plots for lepton hypothesis, using default geant 
// pid;  0==gamma, 1==electron, 2==muon.
// wall; 0==wall, 1==fqwall, 2==fqtowall, 3==fqfromwal
void fqeff::plotWalls(int nbins, int wall, int pid ){
  std::cout << "Making apfit wall, fqwall etc. comparisons! " << std::endl;
//  TH1D* dummy_fqwall = new TH1D(getWallDef(wall).c_str(), (getWallDef(wall)+"comparison").c_str(), nbins, 0, 2000);
  for(int file = 0; file < nfiles; ++file){
    switch(wall){
      case 0:
        parser[file]->is_wall = true;
      case 1:
        parser[file]->is_fqwall = true;
      case 2:
        parser[file]->is_fqtowall = true;
      case 3:
        parser[file]->is_fqfromwall = true;
      default:
        parser[file]->is_wall = true;
    }
  }
  //plotTemplate(dummy_fqwall);
  plotTemplate();


  // Start plotting "fun". 
  TCanvas *c = new TCanvas("c", "c", 1000, 800);
  c->SetFillColor(0);

  for(int file = 0; file < nfiles; ++file){
    std::cout << "File: " << file << " nEntries: " << chains[file]->GetEntries() << std::endl;
    parser[file]->InitPlots();
    parser[file]->fillVector();

    // Now go into the file and fill the histograms
    for(int i = 0; i < chains[file]->GetEntries(); ++i){

      // Get entry for this file/event. The parser contains has all the yummy values
      chains[file]->GetEntry(i);
      parser[file]->GetEntry();

      if(i == 1)
        parser[file]->PrintEvent(file);

      // Are the T2K cuts met?
      if(!T2KCuts(file))
        continue;

      parser[file]->Fill();

//      switch(wall){
//        case 0:
//          plots1D[file]->Fill(parser[file]->E->wall);
//        case 1:
//          plots1D[file]->Fill(parser[file]->E->fqwall[pid]);
//        case 2:
//          plots1D[file]->Fill(parser[file]->E->fqtowall[pid]);
//        case 3:
//          plots1D[file]->Fill(parser[file]->E->fqfromwall[pid]);
//        default:
//          plots1D[file]->Fill(parser[file]->E->wall);
//      }

      // Prints first event for each file, just for dome extra debugging.
    }
    // Scale the histogram to get an "efficiency". In this case the denominator
    // is the pre-cut number of events
    for(size_t iplot = 0; iplot < parser[file]->th1d_vector.size(); ++iplot){
      parser[file]->th1d_vector[iplot]->Scale(1/(double)chains[file]->GetEntries());

      // Apply colors. Starting with Black
      parser[file]->th1d_vector[iplot]->SetLineColor(file+1);

      // The rest of the asthetics (local)
      parser[file]->th1d_vector[iplot]->GetYaxis()->SetTitle("Tagging efficiency");

      parser[file]->th1d_vector[iplot]->SetLineWidth(2);

      if (file==0)
        parser[file]->th1d_vector[iplot]->GetYaxis()->SetTitleOffset(1.5);
    }
//      plots1D[file]->Scale(1/(double)chains[file]->GetEntries());
//
//      // Apply colors. Starting with Black
//      plots1D[file]->SetLineColor(file+1);
//
//      // The rest of the asthetics (local)
//      if(pid==1)
//        plots1D[file]->GetXaxis()->SetTitle((getWallDef(wall)+" for e ring hypothesis").c_str());
//      if(pid==2)
//        plots1D[file]->GetXaxis()->SetTitle((getWallDef(wall)+" for #mu ring hypothesis").c_str());
//      plots1D[file]->GetYaxis()->SetTitle("Tagging efficiency");
//
//      plots1D[file]->SetLineWidth(2);
//
//      if (file==0)
//        plots1D[file]->GetYaxis()->SetTitleOffset(1.5);

  }

  // The rest of the asthetics (global)
  gPad->SetLeftMargin(0.15);

  // Make a legend!
  TLegend *leg = new TLegend(0.48, 0.7, 0.89, 0.89);
  leg->SetFillColor(10);
  leg->SetLineColor(0);
  leg->SetTextSize(0.05);
  for(int i = 0; i < nfiles ; ++i){
    leg->AddEntry(parser[i]->th1d_vector[0], legendTitles[i].c_str());
    if(i==0)
      parser[i]->th1d_vector[0]->Draw();
    else
      parser[i]->th1d_vector[0]->Draw("SAME");
  }
//  for(int i = 0; i < nfiles ; ++i){
//    leg->AddEntry(plots1D[i], legendTitles[i].c_str());
//    if(i==0)
//      plots1D[i]->Draw();
//    else
//      plots1D[i]->Draw("SAME");
//  }
  leg->Draw("SAME");
  c->SaveAs((outputfilename + "_eff_" + getWallDef(wall) + ".png").c_str());

}

// The main plotting function. For now it's just a llh plotter, but we will
// expand ang generalise this!
// Perhaps should REALLY generalise this; Put titles/binning/margins in
// separate functions/constructors/overloading, and the rest of
// cosmetics/setup/saving in one...
void fqeff::plotLlh(){

  // Some initial asthetics
  // Maybe move it to the config...
  gStyle->SetOptStat(0);
  gStyle->SetPalette(51,0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetTitleX(0.1f);
  gStyle->SetTitleW(0.8f);

  // Set the plot type template
  TH1D* dummy_template = new TH1D("fqmrnll[0]", "fqmrnll[0] comparison", nbins, 0, 60000);

  // Setup plots and set branches through parser.
  for(int i = 0; i < nfiles; ++i){
    plots1D[i] = (TH1D*)dummy_template->Clone();

    // This operator actually sets the branches. 
    (*parser[i])(chains[i]);
  }

  // Some sanity checks, just in case
  for(int i = 1; i < nfiles; ++i){
    if(chains[i-1]->GetEntries()!= chains[i]->GetEntries()){
      std::cout << "FLAG: number of entries don't math between: " << std::endl;
    }
  }

  // Start plotting "fun". 
  TCanvas *c = new TCanvas("c", "c", 1000, 800);
  c->SetFillColor(0);

  // We will fill all the files/plots separately.
  for( int file = 0; file < nfiles; ++file){
    std::cout << "File: " << file << " nEntries: " << chains[file]->GetEntries() << std::endl;

    // Now go into the file and fill the histograms
    for(int i = 0; i < chains[file]->GetEntries(); ++i){

      // Get entry for this file/event. The parser contains has all the yummy values
      chains[file]->GetEntry(i);
      parser[file]->GetEntry();

      if(i == 1)
        parser[file]->PrintEvent(file);

      // Are the T2K cuts met?
      if((T2KCUTnumuDis)&&(!parser[file]->isT2KnumuDis()))
        continue;
      if((T2KCUTnueApp)&&(!parser[file]->isT2KnueApp()))
        continue;
      if((T2KCUTnueCC1pi)&&(!parser[file]->isT2KnueCC1pi()))
        continue;

      plots1D[file]->Fill(parser[file]->E->fqmrnll[0]);

      // Prints first event for each file, just for dome extra debugging.
    }
    // Scale the histogram to get an "efficiency". In this case the denominator
    // is the pre-cut number of events
    plots1D[file]->Scale(1/(double)chains[file]->GetEntries());
    
    // Apply colors. Starting with Black
    plots1D[file]->SetLineColor(file+1);

    // The rest of the asthetics (local)
    plots1D[file]->GetXaxis()->SetTitle("Best fit fiTQun -lnL (fqmrnll[0])");
    plots1D[file]->GetYaxis()->SetTitle("Tagging efficiency");

    plots1D[file]->SetLineWidth(2);

    if (file==0)
      plots1D[file]->GetYaxis()->SetTitleOffset(1.5);

  }
  // The rest of the asthetics (global)
  gPad->SetLeftMargin(0.15);

  // Make a legend!
  TLegend *leg = new TLegend(0.48, 0.7, 0.89, 0.89);
  leg->SetFillColor(10);
  leg->SetLineColor(0);
  leg->SetTextSize(0.05);
  for(int i = 0; i < nfiles ; ++i){
    leg->AddEntry(plots1D[i], legendTitles[i].c_str());
    if(i==0)
      plots1D[i]->Draw();
    else
      plots1D[i]->Draw("SAME");
  }
  leg->Draw("SAME");
  c->SaveAs((outputfilename + "_eff_fqmrnll.png").c_str());
}


