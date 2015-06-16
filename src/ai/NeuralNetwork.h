/*
* Copyright Benjamin Wang 2014.
*
* This source code may not be distributed, or used for any commercial or non-commerical
* purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
* exception, any modified versions of this source code.
* This copyright notice may not be removed.
*/

#pragma once
#ifndef _WNTR_NEURALNETWORK_H_
#define _WNTR_NEURALNETWORK_H_

#include <common/utility.h>

namespace wntr {

	class NeuralLayer;

	class NeuralNetwork {
	public:
		static std::shared_ptr<NeuralNetwork> Create(const std::vector<unsigned>& layers);

		explicit NeuralNetwork(const std::vector<unsigned>& layers);
		virtual ~NeuralNetwork() = default;

		void process(const std::vector<double>& inputs);

	protected:

	private:
		shared_vector<NeuralLayer> _layers;

	};

}

#endif