/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#include "NeuralLayer.h"

namespace wntr {

	std::shared_ptr<NeuralLayer> NeuralLayer::Create(unsigned neuron_count) {
		return std::make_shared<NeuralLayer>(neuron_count);
	}

	NeuralLayer::NeuralLayer(unsigned neuron_count)
	: _neuron_count(neuron_count) {
		for (unsigned i = 0; i < _neuron_count; i++) {
			_axons.push_back(0.0);
			_biases.push_back((double)rand() / (double)RAND_MAX);
		}
		_sigmoid = [] (double z) -> double {
			return 1.0/(1.0 + exp(z));
		};
		_sigmoid_prime = [] (double z) -> double {
			double s = 1.0/(1.0 + exp(z));
			return s*(1.0 - s);
		};
	}

	void NeuralLayer::connectTo(const std::shared_ptr<NeuralLayer>& layer) {
		_forward_layer = layer;
		for (unsigned i = 0; i < _neuron_count; i++) {
			std::vector<double> neuron_weights;
			for (unsigned j = 0; j < layer->_neuron_count; j++)
				neuron_weights.push_back((double)rand() / (double)RAND_MAX);
			_weights.push_back(neuron_weights);
		}
	}

	void NeuralLayer::feedForward() {
		assert(false);
	}

	void NeuralLayer::setAxons(const std::vector<double>& axons) {
		if (axons.size() != _axons.size())
			throw std::invalid_argument("error: NeuralLayer::setAxons: incorrect array size");
		for (unsigned i = 0; i < _neuron_count; i++) {
			_axons.at(i) = axons.at(i);
		}
	}

	void NeuralLayer::setAxonInputs(const std::vector<double>& axon_inputs) {
		if (axon_inputs.size() != _axons.size())
			throw std::invalid_argument("error: NeuralLayer::setAxonInputs: incorrect array size");

		for (unsigned i = 0; i < _neuron_count; i++) {
			_axons.at(i) = _sigmoid(axon_inputs.at(i));
		}
	}

	unsigned NeuralLayer::getNeuronCount() const {
		return _neuron_count;
	}

	const std::vector<double>& NeuralLayer::getAxons() const {
		return _axons;
	}

}