#pragma once

#include <mockturtle/networks/aig.hpp>
#include <sta/Network.hh>
namespace psoracle {
void recurse(std::map<sta::Net*, sta::FuncExpr**>* drivers,
	     sta::Instance *instance,
	     sta::Network *adapter);

mockturtle::aig_network *dothething(sta::Network *adapter);
}
