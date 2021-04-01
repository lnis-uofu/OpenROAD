#include <iostream>
#include <gtest/gtest.h>
#include <sta/Sta.hh>
#include <sta/StaMain.hh>
#include <sta/Corner.hh>
#include <sta/ConcreteNetwork.hh>
#include <sta/VerilogReader.hh>
#include <sta/Liberty.hh>
#include "convert.hh"

TEST(sample_test_case, sample_test)
{
  const char *liberty_file = "/home/snelgrov/code/OpenROAD/src/PSOracle/tests/sky130_fd_sc_hd__tt_025C_1v80.lib";
  // const char *verilog_file = "/home/snelgrov/code/OpenROAD/src/PSOracle/tests/gcd_mapped.v";
  // const char *design = "gcd"
  const char *verilog_file = "/home/snelgrov/code/OpenROAD/src/PSOracle/tests/c17_mapped.v";
  const char *design = "c17";

  sta::Sta *test = new sta::Sta;
  sta::Sta::setSta(test);
  sta::initSta();
  test->makeComponents();
  sta::Corner *corner = new sta::Corner("tt", 0);
  sta::MinMaxAll *minmax = sta::MinMaxAll::all();
  bool read_ver = sta::readVerilogFile(verilog_file, test->networkReader());
  EXPECT_TRUE(read_ver) << "failed to read verilog";
  sta::LibertyLibrary *lib = test->readLiberty(liberty_file, corner, minmax, true);
  EXPECT_NE(lib, nullptr) << "failed to read liberty library";

  bool linked = test->linkDesign(design);
  EXPECT_TRUE(linked) << "Failed to link";

  sta::NetworkReader *net = test->networkReader();
  sta::ConcreteInstance *top = reinterpret_cast<sta::ConcreteInstance*>(net->topInstance());
  mockturtle::aig_network *turtle = psoracle::dothething(net);

  std::cout << "boom";
}
