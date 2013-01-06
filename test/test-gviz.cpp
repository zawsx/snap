#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include "Snap.h"
#include "test-helper.h"

class GVizTest { };  // For gtest highlighting

using namespace TSnap;

#define DIRNAME "graphviz"

// Test drawing of SNAP graphs using GraphViz with color labeling
TEST(GVizTest, DrawGViz) {

  PUNGraph UNGraph1;
  UNGraph1 = LoadEdgeList<PUNGraph>(TStr::Fmt("%s/sample_ungraph1.txt", DIRNAME));
  
  PNGraph NGraph1;
  NGraph1 = LoadEdgeList<PNGraph>(TStr::Fmt("%s/sample_ngraph1.txt", DIRNAME));

  mkdir(DIRNAME, S_IRWXU | S_IRWXG | S_IRWXO);
  
  TStrV LNames; //  gvlDot, gvlNeato, gvlTwopi, gvlCirco
  LNames.Add("Dot");
  LNames.Add("Neato");
  LNames.Add("Twopi");
  LNames.Add("Circo");
  
  TStrV Exts;
  Exts.Add("ps");
  Exts.Add("gif");
  Exts.Add("png");
  
  for (int i = 0; i < LNames.Len(); i++) {
    for (int e = 0; e < Exts.Len(); e++) {
      for (int d = 0; d < 2; d++) {
        
        // Baseline file has already been created (use as benchmark)
        TStr FNameBase = TStr::Fmt("%s/base_%s_%s.%s", DIRNAME, d ? "ngraph" : "ungraph" , LNames[i].CStr(), Exts[e].CStr());
        
        TStr FNameTest = TStr::Fmt("%s/test_%s_%s.%s", DIRNAME, d ? "ngraph" : "ungraph" , LNames[i].CStr(), Exts[e].CStr());
        
        // Remove test graph if it already exists
        remove(FNameTest.CStr());
        EXPECT_FALSE(fileExists(FNameTest.CStr()));
        
        // Draw new graph and check if created and equal to baseline (for ps only)
        if (d) {
          TSnap::DrawGViz(NGraph1, TGVizLayout(i), FNameTest, LNames[i], true);
        }
        else {
          TSnap::DrawGViz(UNGraph1, TGVizLayout(i), FNameTest, LNames[i], true);
        }
        // Check if file exists
        EXPECT_TRUE(fileExists(FNameTest.CStr()));
        
        // Compare directly for ps files, (can't compare png and gif due to EXIF-labels)
        if (Exts[e] == "ps") {
          EXPECT_TRUE(compareFiles(FNameBase.CStr(), FNameTest.CStr()));
        }
      }
    }
  }

}

// Test drawing of SNAP graphs using GraphViz with color labeling
TEST(GVizTest, DrawGVizColor) {
  
  PUNGraph UNGraph1;
  UNGraph1 = LoadEdgeList<PUNGraph>(TStr::Fmt("%s/sample_ungraph1.txt", DIRNAME));
    
  TIntStrH NIdColorH;
  TStr CName = TStr::Fmt("%s/node_colors.txt", DIRNAME);
  ASSERT_TRUE(fileExists(CName.CStr()));
  
  // Read in node to color mapping
  FILE *CFile;
  CFile = fopen(CName.CStr(), "rt");
  char line[80];
  while(fgets(line, 80, CFile) != NULL) {
    int NId;
    char Color[20];
    sscanf(line, "%d %s\n", &NId, Color);
    NIdColorH.AddDat(NId, Color);
  }
    
  PNGraph NGraph1;
  NGraph1 = LoadEdgeList<PNGraph>(TStr::Fmt("%s/sample_ngraph1.txt", DIRNAME));
    
  TStrV LNames; //  gvlDot, gvlNeato, gvlTwopi, gvlCirco
  LNames.Add("Dot");
  LNames.Add("Neato");
  LNames.Add("Twopi");
  LNames.Add("Circo");
  
  TStrV Exts;
  Exts.Add("ps");
  Exts.Add("gif");
  Exts.Add("png");
  
  for (int i = 0; i < LNames.Len(); i++) {
    for (int e = 0; e < Exts.Len(); e++) {
      for (int d = 0; d < 2; d++) {
        // Baseline file is already created (use as benchmark)
        TStr FNameBase = TStr::Fmt("%s/base_%s_%s_col.%s", DIRNAME, d ? "ngraph" : "ungraph" , LNames[i].CStr(), Exts[e].CStr());
        
        TStr FNameTest = TStr::Fmt("%s/test_%s_%s_col.%s", DIRNAME, d ? "ngraph" : "ungraph" , LNames[i].CStr(), Exts[e].CStr());
        
        // Remove test graph if it already exists
        remove(FNameTest.CStr());
        EXPECT_FALSE(fileExists(FNameTest.CStr()));
        
        // Draw new graph and check if created and equal to baseline (for ps only)
        if (d) {
          TSnap::DrawGViz(NGraph1, TGVizLayout(i), FNameTest, LNames[i], true, NIdColorH);
        }
        else {
          TSnap::DrawGViz(UNGraph1, TGVizLayout(i), FNameTest, LNames[i], true, NIdColorH);
        }
        // Check if file exists
        EXPECT_TRUE(fileExists(FNameTest.CStr()));
        
        // Compare directly for ps files, (can't compare png and gif due to EXIF-labels)
        if (Exts[e] == "ps") {
          EXPECT_TRUE(compareFiles(FNameBase.CStr(), FNameTest.CStr()));
        }
      }
    }
  }
    
}