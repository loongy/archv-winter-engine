/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "NeuralNetwork.h"

#include "NeuralLayer.h"

namespace wntr {

	std::shared_ptr<NeuralNetwork> NeuralNetwork::Create(const std::vector<unsigned>& neural_layers) {
		return std::make_shared<NeuralNetwork>(neural_layers);
	}

	NeuralNetwork::NeuralNetwork(const std::vector<unsigned>& neural_layers) {
		assert(neural_layers.size() >= 3);
		for (unsigned neuron_count : neural_layers) {
			_layers.add(NeuralLayer::Create(neuron_count));
		}
		for (unsigned i = 1; i < _layers.size(); i++) {
			_layers.at(i - 1)->connectTo(_layers.at(i));
		}
	}

	void NeuralNetwork::process(const std::vector<double>& inputs) {
		assert(_layers.size() >= 3);
		if (inputs.size() != _layers.at(0)->getNeuronCount())
			throw std::invalid_argument("error: NeuralNetwork::process: input size does not match input-layer size");

		_layers.at(0)->setAxons(inputs);
		for (unsigned i = 0; i < _layers.size() - 1; i++) {
			_layers.at(i)->feedForward();
		}
	}

}