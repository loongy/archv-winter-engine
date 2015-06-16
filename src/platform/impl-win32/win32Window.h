/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#ifdef _WIN32

#pragma once
#ifndef _WNTR_WIN32WINDOW_H_
#define _WNTR_WIN32WINDOW_H_

#include "../Window.h"

#include <unordered_map>

struct HWND__;

namespace wntr {

	class win32Window : public Window {
	public:
#ifndef _MSC_VER
		win32Window(win32Window&&) = default;
		win32Window& operator=(win32Window&&) = default;
#endif
		win32Window(const win32Window&) = delete;
		win32Window& operator=(const win32Window&) = delete;

		explicit win32Window(const std::string& name, int x, int y, unsigned width, unsigned height, bool fullscreen);
		virtual ~win32Window();

		virtual void show() override;
		virtual void hide() override;
		virtual void close() override;

		virtual void setX(int x) override;
		virtual void setY(int y) override;
		virtual void setXY(int x, int y) override;
		virtual void setWidth(unsigned width) override;
		virtual void setHeight(unsigned height) override;
		virtual void setSize(unsigned width, unsigned height) override;
		virtual void setFullscreen(bool fullscreen) override;

		const std::shared_ptr<HWND__>& getHWND() const;

	protected:

	private:
		static std::unordered_map<HWND__*, win32Window*> _HWNDMap;
		static long __stdcall WindowProcedure(HWND__* hwnd, unsigned msg, unsigned wparam, long lparam);
		static unsigned GetKeyCodeFromWParam(unsigned wparam);

		void registerWndClassEx();
		void createHWND();

		std::string _hwnd_name;
		std::shared_ptr<HWND__> _hwnd;

	};

}

#endif
#endif