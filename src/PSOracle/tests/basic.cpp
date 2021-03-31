#include <iostream>
#include <gtest/gtest.h>

#include "sta/Sta.hh"
#include "sta/StaMain.hh"
#include "db_sta/dbReadVerilog.hh"
#include "db_sta/dbSta.hh"

TEST(sample_test_case, sample_test)
{
  // sta::dbSta * dbsta = new sta::dbSta;
  odb::dbDatabase *db = new odb::dbDatabase;
  // using ord::dbVerilogNetwork;
  ord::dbVerilogNetwork *verilogNetwork = new ord::dbVerilogNetwork;
  ord::readVerilogFile("gcd.v", verilogNetwork);

  // sta::Sta *sta = new sta::Sta;
  // sta::Sta::setSta(sta);
  // sta::initSta();
  // sta::LibertyLibrary *lib = sta->readLibertyFile("sky130_fd_sc_hd__tt_025C_1v80.lib", true, );
  // sta::NetworkReader *network = sta->networkReader();
  // sta->readNetlistBefore();
 // VerilogReader *verilog_reader = new VerilogReader(network);
 //  bool success = verilog_reader->read("gcd.v");
  // EXPECT_EQ(success, true) << "Don't work";

  //sta::dbSta *sta = ord::makeDbSta();
  //verilog_network_ = makeDbVerilogNetwork();
  // sta->init(openroad->tclInterp(), openroad->getDb(),
  //           // Broken gui api missing openroad accessor.
  //           gui::Gui::get(),
  //           openroad->getLogger());
  // openroad->addObserver(sta);
  // openroad->getVerilogNetwork()->init(openroad->getDbNetwork());

  EXPECT_EQ(1, 1);
}

/*
  read_liberty $libFile
  read_verilog $::env(RESULTS_DIR)/1_synth.v
  read_sdc $::env(RESULTS_DIR)/1_synth.sdc

StaTcl.i
bool
read_liberty_cmd(char *filename,
		 Corner *corner,
		 const MinMaxAll *min_max,
		 bool infer_latches)
{
  LibertyLibrary *lib = Sta::sta()->readLiberty(filename, corner, min_max,
						infer_latches);
  return (lib != nullptr);
}

Verilog.i
bool
read_verilog(const char *filename)
{
  Sta *sta = Sta::sta();
  NetworkReader *network = sta->networkReader();
  if (network) {
    sta->readNetlistBefore();
    return readVerilogFile(filename, network);
  }
  else
    return false;
}


*/
