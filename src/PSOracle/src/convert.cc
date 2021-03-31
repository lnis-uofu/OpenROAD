#include <mockturtle/mockturtle.hpp>
#include <sta/Network.hh>
#include <sta/FuncExpr.hh>
#include <sta/Liberty.hh>
#include <sta/PortDirection.hh>

mockturtle::aig_network::signal convertFuncHack(mockturtle::aig_network *ntk, sta::FuncExpr *expr)
{
  switch (expr->op()) {
  case sta::FuncExpr::Operator::op_port:
    {
      if (expr->left() != nullptr) { // hack the left pointer to be an indirect buffer
	return convertFuncHack(ntk, *(reinterpret_cast<sta::FuncExpr **>(expr->left())));
      } else { // hack the right pointer to be an already generated pi signal
	return *(reinterpret_cast<mockturtle::aig_network::signal *>(expr->right()));
      }
    }
  case sta::FuncExpr::Operator::op_not:
    {
      return ntk->create_not(convertFuncHack(ntk, expr->left()));
    }
  case sta::FuncExpr::Operator::op_or:
    {
      return ntk->create_or(convertFuncHack(ntk, expr->left()), convertFuncHack(ntk, expr->right()));
    }
  case sta::FuncExpr::Operator::op_and:
    {
      return ntk->create_and(convertFuncHack(ntk, expr->left()), convertFuncHack(ntk, expr->right()));
    }
  case sta::FuncExpr::Operator::op_xor:
    {
      return ntk->create_xor(convertFuncHack(ntk, expr->left()), convertFuncHack(ntk, expr->right()));
    }
  case sta::FuncExpr::Operator::op_one:
    {
      return ntk->get_constant(true);
    }
  case sta::FuncExpr::Operator::op_zero:
    {
      return ntk->get_constant(false);
    }
  }
  return ntk->get_constant(false);
}

sta::FuncExpr *hackFunc(std::map<sta::LibertyPort*, sta::FuncExpr*> *drivers, sta::FuncExpr *expr) {
  if (expr->op() == sta::FuncExpr::Operator::op_port) {
    return expr;
  } else {
    switch (expr->op()) {
    case sta::FuncExpr::Operator::op_port:
      {
	sta::FuncExpr* foo = new sta::FuncExpr(sta::FuncExpr::Operator::op_not,
					       drivers->find(expr->port())->second,
					       nullptr,
					       nullptr);
	return foo;
      }
    case sta::FuncExpr::Operator::op_not:
      {
	return sta::FuncExpr::makeNot(hackFunc(drivers, expr->left()));
      }
    case sta::FuncExpr::Operator::op_or:
      {
	return sta::FuncExpr::makeOr(hackFunc(drivers, expr->left()), hackFunc(drivers, expr->right()));
      }
    case sta::FuncExpr::Operator::op_and:
      {
	return sta::FuncExpr::makeAnd(hackFunc(drivers, expr->left()), hackFunc(drivers, expr->right()));
      }
    case sta::FuncExpr::Operator::op_xor:
      {
	return sta::FuncExpr::makeXor(hackFunc(drivers, expr->left()), hackFunc(drivers, expr->right()));
      }
    case sta::FuncExpr::Operator::op_one:
      {
	return sta::FuncExpr::makeOne();
      }
    case sta::FuncExpr::Operator::op_zero:
      {
	return sta::FuncExpr::makeZero();
      }
    default:
      return expr; // TODO throw
    }
  }
}

void recurse(std::map<sta::Net*, sta::FuncExpr**>* drivers,
	sta::Instance *instance,
	     sta::Network *adapter);

mockturtle::aig_network *dothething(sta::Network *adapter)
{
  mockturtle::aig_network *network;

  std::map<sta::Net*, sta::FuncExpr**> *drivers;
  sta::Instance *top = adapter->topInstance();
  std::map<sta::Term*, mockturtle::aig_network::signal> *input_terminals;
  // create PI from the terminals

  sta::InstancePinIterator *pins = adapter->pinIterator(top);
  while (pins->hasNext()) {
    sta::Pin *pin = pins->next();
    sta::Term *term = adapter->term(pin);
    if (term) {
      input_terminals->insert({term, network->create_pi(adapter->name(term))});
    }
  }
  //  std::map<sta::Term*, mockturtle::aig_network::signal> *output_terminals =
    recurse(drivers, top, adapter);
  // for(auto it = output_terminals.begin(); it != output_terminals.end(); it++) {
  //   sta::Term *term = it->first;
  //   mockturtle::aig_network::signal signal = it->second;
  //   network->create_po(signal, adapter->name(term));
  // }
  // TODO memory cleanup
  return network;
}
  //


  /*
    // create PI from the terminals
    std::map<sta::Term*, mockturtle::aig_network::signal> input_terminals;
    sta::InstancePinIterator *pins = adapter->pinIterator(top);
    while (pins->hasNext()) {
      sta::Pin *pin = pins->next();
      sta::Term *term = adapter->term(pin);
      if (term) {
	input_terminals.insert({term, network->create_pi(adapter->name(term))});
      }
    }
    std::map<sta::Net*, mockturtle::aig_network::signal> *output_terminals =
      recurse(inputs, network, top, adapter);
    for(auto it = output_terminals.begin(); it != output_terminals.end(); it++) {
      sta::Term *term = it->first;
      mockturtle::aig_network::signal signal = it->second;
      network->create_po(signal, adapter->name(term));
    }
    // TODO memory cleanup
    return network;
  */





void extractLeaf(std::map<sta::Net*, sta::FuncExpr**> *drivers,
	sta::Instance *instance,
	sta::Network *adapter)
{
  sta::LibertyCell *cell = adapter->libertyCell(instance);
  std::map<sta::LibertyPort*, sta::FuncExpr*> ports;
  // TODO this doesn't handle internal registers/ports

  sta::InstancePinIterator *pins = adapter->pinIterator(instance);
  // map ports to nets
  while (pins->hasNext()) {
    sta::Pin *pin = pins->next();
    sta::LibertyPort *liberty_port = adapter->libertyPort(pin);
    sta::Net *net = adapter->net(pin);
    sta::FuncExpr *func = liberty_port->function();
    ports.insert({liberty_port, func});
  }

  // map port functions to net drivers
  pins = adapter->pinIterator(instance);
  while (pins->hasNext()) {
    sta::Pin *pin = pins->next();
    sta::FuncExpr *func = adapter->libertyPort(pin)->function();
    if (func) {
      sta::FuncExpr *hacked = hackFunc(&ports, func);
      sta::Net *net = adapter->net(pin);
      sta::FuncExpr** ref = drivers->find(net)->second;
      *ref = hacked;
    }
  }
}

void extractHierarchical(std::map<sta::Net*, sta::FuncExpr**> *drivers,
			 sta::Instance *instance,
			 sta::Network *adapter)
{
  sta::InstanceChildIterator *childs = adapter->childIterator(instance);
  // extract each child.
  while (childs->hasNext()) {
    sta::Instance *child = childs->next();
    recurse(drivers, child, adapter);
  }
}

void recurse(std::map<sta::Net*, sta::FuncExpr**> *drivers,
	sta::Instance *instance,
	sta::Network *adapter)
{
  // instance->netIterator - includes nets that are connect to the instance parent through pins.
  // for each output terminal, extract function, add signal to output map.
  sta::InstanceNetIterator *nets = adapter->netIterator(instance);
  while (nets->hasNext()) {
    sta::Net *net = nets->next();
    //insert empty pointer into drivers for each network if does not exists
    if (drivers->find(net) == drivers->end()) {
      drivers->insert({net, nullptr});
    }
  }

  if (adapter->isLeaf(instance)) {
    return extractLeaf(drivers, instance, adapter);
  } else {
    return extractHierarchical(drivers, instance, adapter);
  }
}
