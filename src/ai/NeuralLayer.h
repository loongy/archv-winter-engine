/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_NEURALLAYER_H_
#define _WNTR_NEURALLAYER_H_

#include <common/utility.h>

namespace wntr {

	class NeuralLayer {
	public:
		static std::shared_ptr<NeuralLayer> Create(unsigned neuron_count);

		explicit NeuralLayer(unsigned neuron_count);
		virtual ~NeuralLayer() = default;
		
		void connectTo(const std::shared_ptr<NeuralLayer>& layer);
		void feedForward();

		void setAxons(const std::vector<double>& axons);
		void setAxonInputs(const std::vector<double>& axon_inputs);

		unsigned getNeuronCount() const;
		const std::vector<double>& getAxons() const;

	protected:

	private:
		unsigned _neuron_count;
		std::function<double(double)> _sigmoid;
		std::function<double(double)> _sigmoid_prime;
		std::vector< std::vector<double> > _weights;
		std::vector<double> _axons;
		std::vector<double> _biases;

		std::weak_ptr<NeuralLayer> _forward_layer;

	};

}

#endif