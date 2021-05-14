
#include "PTFTTree.hh"
#include <cmath>

void ptfplot( char * fname = "geant4ptf_full.root"){

  // setup input
  TFile * fin = new TFile( fname, "read" );
  PTFTTree * ptf = new PTFTTree;  
  TTree * ptf_tree = (TTree*) fin->Get("ptf_tree");
  ptf->SetBranchAddresses( ptf_tree );
  
  // setup output
  TFile* fout = new TFile( "ptfplot.root", "recreate" );

  // book histograms
  TH2D * hq = new TH2D( "hq", "dig_Q variables; x (mm); y (mm)",
			250, -250.0, 250.0, 250, -250.0, 250.0 );
  
  TH2D * hqscan = new TH2D( "hqscan", "Number of hits on photocathode; x (mm); y (mm)",
			    250, -250.0, 250.0, 250, -250.0, 250.0 );

  TH2D * hscanxy = new TH2D( "hscanxy", "Number of hits on photocathode(not same); x (mm); y (mm)",
			    250, -250.0, 250.0, 250, -250.0, 250.0 );
				
  TH2D * d_eff = new TH2D( "d_eff", "Detection efficiency; x (mm); y (mm)",
			  			    250, -250.0, 250.0, 250, -250.0, 250.0 );	
 TH1D * timing = new TH1D( "timing", "Timing variable; time(sec)",
						  			  			    100,0, 10);
  TH1D * dig_Time = new TH1D( "dig_Time", "dig_Time; time(sec)",
						  			  			    100,0,10 );				    	
  TH2D* d_eff_norm;//=new TH2D( "d_eff_norm", "Detection efficiency normalized; x (mm); y (mm)",
			  	//		    250, -250.0, 250.0, 250, -250.0, 250.0 );
  
  unsigned long long nevents = ptf_tree->GetEntries();
  cout<<nevents<<endl;
  //TCanvas c( "hscanxy", "hscanxy" );
  //hscanxy->Draw("colz");
  int i=0;
  for (unsigned long long iev =0 ; iev < nevents; ++iev ){
    
    ptf_tree->GetEvent( iev );
	//cout<< iev<<endl;
    // clear hscanxy
    //hscanxy->Reset();
	//if (ptf->true_t[iev]>1);{ //Investigation around the value of what dig_T actually represents
	
	i++;
		//continue;  
		//}
	
    // get average x, y position
    double xav = 0.0, yav = 0.0;
    int    nphotons = ptf->NPhotons;


	if (ptf->true_t[0]>0.5){ //Get rid of the reflection that we had
				//cout<<ptf->true_t[0]<<endl;//ptf->true_t[0]<<endl
				//cout<<ptf->true_used[0]<<endl;
				continue;
				};
	//bool j=0; 
	bool k=ptf->true_used[0];
	if (k==0){ //We only want to display the photons that were detected
		   //cout<<ptf->true_used[0]<<endl;//ptf->true_t[0]<<endl
		   continue;
		  						};
    for ( int iph = 0; iph<nphotons; ++iph ){
		
      xav += ptf->true_x[iph];
      yav += ptf->true_y[iph];
      hscanxy->Fill( ptf->true_x[iph], ptf->true_y[iph] );
      //hscanxy->Reset();
	 
		//cout<<ptf->true_used[iev] <<endl;
  	//	i++;
    //  dig_Time->Fill( ptf->true_x[iph], ptf->true_y[iph], ptf->dig_T[iph]); //
	//}
    if ( nphotons > 0 ){
      xav /= nphotons;
      yav /= nphotons;
    }
    hq->Fill( xav, yav, ptf->dig_Q[0] );
    //d_eff->Fill( xav, yav, ptf->dig_Q[0] );
    int ix = iev / 250;//Why 250, separate into 250 different sections ?
    int iy = iev % 250;
    
    double xloc = -250.0 + ix * 2.0; //Only a bin that is continued
    double yloc = -250.0 + iy * 2.0;
    //hqscan->SetBinContent( ix+1, iy+1, ptf->dig_Q[0] );
    hqscan->Fill( ptf->true_x[0], ptf->true_y[0] );
	timing->Fill(ptf->true_t[0]);
	dig_Time->Fill(ptf->dig_T[0]);
  	//cout<<ptf->true_t[0]<<endl;
	bool j;
	j=round(ptf->true_t[0])>3;
	bool k= true;
	//if (round(ptf->true_t[0])>3){ //Investigation around the value of what dig_T actually represents
	//	cout<<j<<endl;//ptf->true_t[0]<<endl
	 //};
	 
	//d_eff->Fill( ptf->true_x[0], ptf->true_y[0],(ptf->true_used[0]) );

    // 
    //hscanxy->SetBinContent( ix+1, iy+1, 10000.0 );//why ?

   

    //c.Update();
    //char gif[100];
    //sprintf(gif,"gifsicle/hscanxy%05d.gif",iev);
    //c.SaveAs(gif);
        
    //cout << "iev="<<iev<<" ix="<<ix<<" iy="<<iy
	 //<<" xloc="<<xloc<<" yloc="<<yloc
	 //<<" x="<<xav<<" y="<<yav<<" Q="<<ptf->dig_Q[0]<<endl;
	//cout<<ptf->true_t[iev]<<endl;
  }
}
 //int    nphotons = ptf->NPhotons;
//cout<< nphotons<<endl;
  //d_eff_norm = (TH2D*) d_eff->Clone("d_eff_norm");
  //d_eff->Divide(hqscan);
  //cout<<i<<endl;
for(int ix=1; ix<=hqscan->GetNbinsX(); ix++){
	 for(int iy=1; iy<= hqscan->GetNbinsY(); iy++){
		   double weight= ( hqscan->GetBinContent(ix, iy))/100;
		    d_eff->SetBinContent(ix,iy,weight);
	  //cout<<weight<<endl;
	  
		}
	}
  hq->SetStats(0);//Made to not display statistic box
  TCanvas* c = new TCanvas("canvas");
  string plotname;
  hq->SetDirectory( fout );
  hq->Draw("colz0");
  gPad->Modified();
  gPad->Update();
  plotname = string("/Users/vincentgousy-leblanc/Desktop/Research/Geant4/Geant4_PTF_2/Geant4_PTF/postprocess/De_test.pdf");
  c->SaveAs(plotname.c_str(),"pdf");
  hscanxy->SetStats(0);//Made to not display statistic box
  hscanxy->SetDirectory( fout );
  hscanxy->Draw("colz0");
  gPad->Modified();
  gPad->Update();
  plotname = string("/Users/vincentgousy-leblanc/Desktop/Research/Geant4/Geant4_PTF_2/Geant4_PTF/postprocess/De_test_2.pdf");
  c->SaveAs(plotname.c_str(),"pdf");
  hqscan->SetStats(0);//Made to not display statistic box
  hqscan->SetDirectory( fout );
  hqscan->Draw("colz0");
  gPad->Modified();
  gPad->Update();
  plotname = string("/Users/vincentgousy-leblanc/Desktop/Research/Geant4/Geant4_PTF_2/Geant4_PTF/postprocess/De_test_4.pdf");
  c->SaveAs(plotname.c_str(),"pdf");
  //timing->SetStats(0);//Made to not display statistic box
  timing->SetDirectory( fout );
  timing->Draw();
  gPad->Modified();
  gPad->Update();
  plotname = string("/Users/vincentgousy-leblanc/Desktop/Research/Geant4/Geant4_PTF_2/Geant4_PTF/postprocess/timing.pdf");
  c->SaveAs(plotname.c_str(),"pdf");
  
  d_eff->SetStats(0);//Made to not display statistic box
  d_eff->SetDirectory( fout );
  d_eff->Draw("colz0");
  gPad->Modified();
  gPad->Update();
  plotname = string("/Users/vincentgousy-leblanc/Desktop/Research/Geant4/Geant4_PTF_2/Geant4_PTF/postprocess/De_test_3.pdf");
  c->SaveAs(plotname.c_str(),"pdf");
  /*
  //d_eff_norm->SetMinimum(0.3);
  d_eff_norm->SetMaximum(1);
  d_eff_norm->SetStats(0);//Made to not display statistic box
  d_eff_norm->SetDirectory( fout );
  d_eff_norm->Draw("colz0");
  gPad->Modified();
  gPad->Update();
  plotname = string("/Users/vincentgousy-leblanc/Desktop/Research/Geant4/Geant4_PTF_2/Geant4_PTF/postprocess/De_test_5.pdf");
  c->SaveAs(plotname.c_str(),"pdf");
  */
 
  dig_Time->SetMinimum(0.0);
  //dig_Time->SetStats(0);//Made to not display statistic box
  dig_Time->SetDirectory( fout );
  dig_Time->SetFillColor( kRed);
  //SetFillStyle( 3001);
  dig_Time->Draw("SAME");
  gPad->Modified();
  gPad->Update();
  plotname = string("/Users/vincentgousy-leblanc/Desktop/Research/Geant4/Geant4_PTF_2/Geant4_PTF/postprocess/dig_Time.pdf");
  c->SaveAs(plotname.c_str(),"pdf");
  
  
  
  fin->Close();
  
  fout->Write();
  fout->Close();

}
