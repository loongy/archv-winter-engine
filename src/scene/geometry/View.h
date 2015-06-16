/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#pragma once
#ifndef _WNTR_VIEW_H_
#define _WNTR_VIEW_H_

#include <common/Transform3D.h>

namespace wntr {
    
    class View : public Transform3D {
	public:
		static std::shared_ptr<View> Create(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		static std::shared_ptr<View> Create(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

#ifndef _MSC_VER
		View(View&&) = default;
		View& operator=(View&&) = default;
#endif
		View(const View&) = delete;
		View& operator= (const View&) = delete;

		explicit View(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
		virtual ~View() = default;

		virtual const component_hierarchy& hierarchy() const override;

		virtual void translate(const glm::vec3& translation) override;
		virtual void scale(const glm::vec3& scale) override;
		virtual void rotate(const glm::quat& rotation) override;
        virtual void rotateInParentSpace (const glm::quat& rotation) override;
		
		virtual void setPosition(const glm::vec3& position) override;
		virtual void setScale(const glm::vec3& scale) override;
		virtual void setRotation(const glm::quat& rotation) override;

		void setForward(const glm::vec3& forward);
		void setRight(const glm::vec3& right);
		void setUp(const glm::vec3& up);

		const glm::mat4& view() const;

		const glm::vec3& getForward() const;
		const glm::vec3& getRight() const;
		const glm::vec3& getUp() const;

		glm::vec3 getBack() const;
		glm::vec3 getLeft() const;
		glm::vec3 getDown() const;

	protected:

	private:
		virtual const glm::vec3& getScale()    const override { return _scale;    }; // deprecated
		virtual const glm::quat& getRotation() const override { return _rotation; }; // deprecated

		static const component_hierarchy _Hierarchy;

		mutable bool _view_did_change;
		mutable glm::mat4 _view_mat;

		mutable glm::vec3 _forward;
		mutable glm::vec3 _right;
		mutable glm::vec3 _up;

    };
    
}

#endif
