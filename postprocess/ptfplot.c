
#include "PTFTTree.hh"

void ptfplot( char * fname = "geant4ptf_000000.root"){

  // setup input
  TFile * fin = new TFile( fname, "read" );
  PTFTTree * ptf = new PTFTTree;  
  TTree * ptf_tree = (TTree*) fin->Get("ptf_tree");
  ptf->SetBranchAddresses( ptf_tree );
  
  // setup output
  TFile* fout = new TFile( "ptfplot.root", "recreate" );

  // book histograms
  TH2D * hq = new TH2D( "hq", "Number of Photons; x (mm); y (mm)",
			250, -250.0, 250.0, 250, -250.0, 250.0 );
  
  TH2D * hqscan = new TH2D( "hqscan", "Number of Photons; x (mm); y (mm)",
			    250, -250.0, 250.0, 250, -250.0, 250.0 );

  TH2D * hscanxy = new TH2D( "hscanxy", "Locations; x (mm); y (mm)",
			    250, -250.0, 250.0, 250, -250.0, 250.0 );
  
  unsigned long long nevents = ptf_tree->GetEntries();

  TCanvas c( "hscanxy", "hscanxy" );
  hscanxy->Draw("colz");
  
  for (unsigned long long iev =0 ; iev < nevents; ++iev ){
    
    ptf_tree->GetEvent( iev );

    // clear hscanxy
    hscanxy->Reset();
    
    // get average x, y position
    double xav = 0.0, yav = 0.0;
    int    nphotons = ptf->NPhotons;
    for ( int iph = 0; iph<nphotons; ++iph ){
      xav += ptf->true_x[iph];
      yav += ptf->true_y[iph];
      hscanxy->Fill( ptf->true_x[iph], ptf->true_y[iph] );
    }
    if ( nphotons > 0 ){
      xav /= nphotons;
      yav /= nphotons;
    }
    hq->Fill( xav, yav, ptf->dig_Q[0] );

    int ix = iev / 250;
    int iy = iev % 250;
    
    double xloc = -250.0 + ix * 2.0;
    double yloc = -250.0 + iy * 2.0;
    hqscan->SetBinContent( ix+1, iy+1, ptf->dig_Q[0] );

    // 
    hscanxy->SetBinContent( ix+1, iy+1, 10000.0 );



    c.Update();
    char gif[100];
    sprintf(gif,"gifsicle/hscanxy%05d.gif",iev);
    c.SaveAs(gif);
        
    cout << "iev="<<iev<<" ix="<<ix<<" iy="<<iy
	 <<" xloc="<<xloc<<" yloc="<<yloc
	 <<" x="<<xav<<" y="<<yav<<" Q="<<ptf->dig_Q[0]<<endl;
  }

  fin->Close();
  
  fout->Write();

}
