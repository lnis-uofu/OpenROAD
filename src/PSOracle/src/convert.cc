#include <mockturtle/mockturtle.hpp>
#include <sta/FuncExpr.hh>
#include <sta/Liberty.hh>
#include <sta/PortDirection.hh>

mockturtle::aig_network *convert(sta::Network *adapter)
{
    mockturtle::aig_network *network;
    sta::Instance *top = adapter->topInstance();
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
    std::map<sta::Term*, mockturtle::aig_network::signal> *output_terminals =
      recurse(inputs, network, top, adapter);
    for(auto it = output_terminals.begin(); it != output_terminals.end(); it++) {
      sta::Term *term = it->first;
      mockturtle::aig_network::signal signal = it->second;
      network->create_po(signal, adapter->name(term));
    }
    // TODO memory cleanup
    return network;
}

std::map<sta::Term*, mockturtle::aig_network::signal>
recurse(std::map<sta::Term*, mockturtle::aig_network::signal> input_terminals,
	mockturtle::aig_network *network,
	sta::Instance *instance,
	sta::Network *adapter)
{
  if (instance->isLeaf) {
    sta::LibertyCell *cell = adapter->libertyCell(instance);
    sta::InstancePinIterator *pins = adapter->pinIterator(top);
    std::map<sta::LibertyPort*, mockturtle::aig_network::signal> *input_terminals;
    // TODO this doesn't handle internal registers/ports
    // for each input terminal, get the port and build the input signal list.
    while (pins->hasNext()) {
      sta::Pin *pin = pins->next();
      sta::Term *term = adapter->term(pin);
      if (term) {
	sta::LibertyPort *liberty_port = adapter->libertyPort(pin);
	if (port->isClock() || !port->direction()->isInput()) continue;
	mockturtle::aig_network::signal signal = input_terminals->find(term);
	input_terminal.insert({port, signal});
      }
    }
    // for each output terminal, extract function, add signal to output map.
    std::map<sta::Term*, mockturtle::aig_network::signal> output_terminals;
    pins = adapter->pinIterator(top);
    while (pins->hasNext()) {
      sta::Pin *pin = pins->next();
      sta::Term *term = adapter->term(pin);
      if (term) {
	sta::LibertyPort *liberty_port = adapter->libertyPort(pin);
	if (!port->direction()->isOutput()) continue;
	mockturtle::aig_network::signal output = convertFunc(ntk, &input_terminals, liberty_port->function());
	output_terminals.push(term, signal);
      }
    }
    return output_terminals;
  } else {
    std::map<sta::Net*, mockturtle::aig_network::signal> nets;
    for(auto it = input_terminals.begin(); it != input_terminals.end(); it++) {
      sta::Term *term = it->first;
      mockturtle::aig_network::signal signal = it->second;
      nets.push({adapter->net(term), signal});
    }

    sta::InstanceChildIterator *children =  adapter.childIterator(instance);
    size_t visits_remaining = children;
    while(visits_remaining
    for(auto it = output_terminals.begin(); it != output_terminals.end(); it++) {
      sta::Term *term = it->first;
      mockturtle::aig_network::signal signal = it->second;
      network->create_po(signal, adapter->name(term));
    }



    //
    // while not all instances have been visited,
    // find each child that has input pins fully connected to available nets.
    // recurse on that instance.
    // take the returned terminal signals, add to the net map.
    // once done, get the net for each terminal, make the output

    /*
    while (children->hasNext()) {
      sta::Instance *child = children.next();
      recurse(
    }
    */
  }

}

mockturtle::aig_network::signal convertFunc(mockturtle::aig_network *ntk,
					    std::map<sta::LibertyPort*, mockturtle::aig_network::signal> *ports,
					    sta::FuncExpr *expr)
{
  switch (expr->op()) {
  case sta::FuncExpr::Operator::op_port:
    return ports->find(expr->port())->second;
  case sta::FuncExpr::Operator::op_not:
    return ntk->create_not(convertFunc(ntk, ports, expr->left()));
  case sta::FuncExpr::Operator::op_or:
    return ntk->create_or(convertFunc(ntk, ports, expr->left()), convertFunc(ntk, ports, expr->right()));
  case sta::FuncExpr::Operator::op_and:
    return ntk->create_and(convertFunc(ntk, ports, expr->left()), convertFunc(ntk, ports, expr->right()));
  case sta::FuncExpr::Operator::op_xor:
    return ntk->create_xor(convertFunc(ntk, ports, expr->left()), convertFunc(ntk, ports, expr->right()));
  case sta::FuncExpr::Operator::op_one:
    return ntk->get_constant(true);
  case sta::FuncExpr::Operator::op_zero:
    return ntk->get_constant(false);
  }
  return ntk->get_constant(false);
}

// functions on libertycell of potential interest.
  // typedef Map<LibertyPort*, Sequential*> PortToSequentialMap; // LibertyPort to Sequential register class
  // PortToSequentialMap port_to_seq_map_;
  // bool hasInternalPorts();
  // findLibertyPort(name)

  // bool isLatchData(LibertyPort *port);
  // is{macro, memory, pad, dontuse, buffer, inverter}
  // bool hasSequentials() const;
  // void makeSequential(...)
  // void bufferPorts(// Return values.
  // 		   LibertyPort *&input,
  // 		   LibertyPort *&output) const;
  //// Find the sequential with the output connected to an (internal) port.
  // Sequential *outputPortSequential(LibertyPort *port);

mockturtle::aig_network *convert(sta::LibertyCell *cell) {
  mockturtle::aig_network *ntk;

  sta::LibertyCellPortIterator iterator(cell);
  std::map<sta::LibertyPort*, mockturtle::aig_network::signal> ports;
  for (sta::LibertyPort *port = iterator.next(); iterator.hasNext(); port = iterator.next()) {
    if (port->isClock() || !port->direction()->isInput()) continue;
    // Generate signals for each input port.
    ports.insert({port, ntk->create_pi()});
  }
  for (sta::LibertyPort *port = iterator.next(); iterator.hasNext(); port = iterator.next()) {
    if (!port->direction()->isOutput()) continue;
    // for each output port, generate signals for function.

    ntk->create_po(convertFunc(ntk, &ports, port->function()));
  }
  return ntk;
}
