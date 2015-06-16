/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_UTILITY_H_
#define _WNTR_UTILITY_H_

// include c++ standard headers
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <functional>
#include <list>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

// include glm maths
#define GLM_FORCE_RADIANS
#include <glm/geometric.hpp>
#include <glm/glm.hpp>

// include glm maths extensions
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

namespace wntr {

	template<typename T>
	class shared_vector {
	public:
#ifndef _MSC_VER
		shared_vector(shared_vector&&) = default;
		shared_vector& operator=(shared_vector&&) = default;
#endif
		shared_vector(const shared_vector&) = delete;
		shared_vector& operator=(const shared_vector&) = delete;

        shared_vector(const std::initializer_list<std::shared_ptr<T>>& members);
		shared_vector() = default;
		~shared_vector() = default;

		void add(const std::shared_ptr<T>& ptr);
		void add(std::shared_ptr<T>&& ptr);

		void remove(const std::shared_ptr<T>& ptr);
		void remove(std::shared_ptr<T>&& ptr);
		void remove(const typename std::vector<std::shared_ptr<T>>::iterator& it);
		void remove_if(const std::function<bool(const std::shared_ptr<T>&)>& should_remove);

		std::shared_ptr<T>& at(unsigned i);
		const std::shared_ptr<T>& at(unsigned i) const;

		unsigned size() const;
		typename std::vector<std::shared_ptr<T>>::const_iterator begin() const;
		typename std::vector<std::shared_ptr<T>>::const_iterator end() const;
        typename std::vector<std::shared_ptr<T>>::const_reverse_iterator rbegin() const;
		typename std::vector<std::shared_ptr<T>>::const_reverse_iterator rend() const;

		std::vector<std::shared_ptr<T>>& values();

	protected:
	private:
		std::vector<std::shared_ptr<T>> _members;
        
	};
    
    template<typename T>
    shared_vector<T>::shared_vector(const std::initializer_list<std::shared_ptr<T>>& members)
    : _members(members) {
    }

	template<typename T>
	void shared_vector<T>::add(const std::shared_ptr<T>& ptr) {
		_members.push_back(ptr);
	}

	template<typename T>
	void shared_vector<T>::add(std::shared_ptr<T>&& ptr) {
		_members.push_back(ptr);
	}

	template<typename T>
	void shared_vector<T>::remove(const std::shared_ptr<T>& ptr) {
		auto raw = ptr.get();
		for (auto it = _members.begin(), end = _members.end(); it != end; it++)
			if (it->get() == raw) {
				_members.erase(it);
				return;
			}
	}

	template<typename T>
	void shared_vector<T>::remove(std::shared_ptr<T>&& ptr) {
		auto raw = ptr.get();
		for (auto it = _members.begin(), end = _members.end(); it != end; it++)
			if (it->get() == raw) {
				_members.erase(it);
				return;
			}
	}

	template<typename T>
	void shared_vector<T>::remove(const typename std::vector<std::shared_ptr<T>>::iterator& it) {
		_members.erase(it);
	}

	template<typename T>
	void shared_vector<T>::remove_if(const std::function<bool(const std::shared_ptr<T>&)>& should_remove) {
		std::remove_if(_members.begin(), _members.end(), should_remove);
	}

	template<typename T>
	std::shared_ptr<T>& shared_vector<T>::at(unsigned i) {
		return _members.at(i);
	}

	template<typename T>
	const std::shared_ptr<T>& shared_vector<T>::at(unsigned i) const {
		return _members.at(i);
	}

	template<typename T>
	unsigned shared_vector<T>::size() const {
		return (unsigned)_members.size();
	}

	template<typename T>
	typename std::vector<std::shared_ptr<T>>::const_iterator shared_vector<T>::begin() const {
		return _members.begin();
	}

	template<typename T>
	typename std::vector<std::shared_ptr<T>>::const_iterator shared_vector<T>::end() const {
		return _members.end();
	}
    
    template<typename T>
    typename std::vector<std::shared_ptr<T>>::const_reverse_iterator shared_vector<T>::rbegin() const {
        return _members.rbegin();
    }
    
    template<typename T>
    typename std::vector<std::shared_ptr<T>>::const_reverse_iterator shared_vector<T>::rend() const {
        return _members.rend();
    }

	template<typename T>
	std::vector<std::shared_ptr<T>>& shared_vector<T>::values() {
		return _members;
	}
    
    class component_hierarchy {
    public:
        friend std::ostream& operator<<(std::ostream& os, const component_hierarchy& hierarchy);
        
#ifndef _MSC_VER
		component_hierarchy(component_hierarchy&&) = default;
		component_hierarchy& operator=(component_hierarchy&&) = default;
#endif
		component_hierarchy(const component_hierarchy&) = default;
		component_hierarchy& operator=(const component_hierarchy&) = default;
        
		explicit component_hierarchy(const std::vector<std::type_index>& types);
		virtual ~component_hierarchy() = default;
        
        const std::vector<std::type_index>& types() const;

    protected:
    private:
        std::vector<std::type_index> _types;
        
    };

}



#endif
