/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2014 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#ifndef XCSOAR_SCREEN_PAINT_WINDOW_HXX
#define XCSOAR_SCREEN_PAINT_WINDOW_HXX

#include "Screen/Window.hpp"

#ifdef USE_GDI
#include <tchar.h>
#endif

class ContainerWindow;

/**
 * A #Window implementation for custom drawing.  Implement
 * Window::OnPaint() to draw something.
 */
class PaintWindow : public Window {
private:
  /* hide this method */
  void InstallWndProc();

public:
  PaintWindow() {
    EnableCustomPainting();
  }

#ifdef USE_GDI
  static bool register_class(HINSTANCE hInstance);
#endif

#ifndef USE_GDI
  using Window::Create;

  void Create(ContainerWindow &parent, PixelRect rc,
              const WindowStyle style=WindowStyle()) {
    Create(&parent, rc, style);
  }
#else /* USE_GDI */
  void Create(ContainerWindow *parent, const TCHAR *cls, PixelRect rc,
              const WindowStyle style=WindowStyle()) {
    Window::Create(parent, cls, nullptr, rc, style);
  }

  void Create(ContainerWindow &parent, const TCHAR *cls, PixelRect rc,
              const WindowStyle style=WindowStyle()) {
    Create(&parent, cls, rc, style);
  }

  void Create(ContainerWindow &parent, PixelRect rc,
              const WindowStyle style=WindowStyle()) {
    Create(parent, _T("PaintWindow"), rc, style);
  }
#endif /* USE_GDI */

  constexpr
  static bool SupportsPartialRedraw() {
#ifdef USE_GDI
    /* we can use the GDI function InvalidateRect() with a non-nullptr
       RECT */
    return true;
#else
    /* SDL and OpenGL can't do partial redraws, they always repaint
       the whole screen */
    return false;
#endif
  }

  /**
   * Invalidates the visible area and schedules a repaint (which will
   * occur in the main thread).
   */
  void Invalidate() {
    AssertThread();

#ifndef USE_GDI
    Window::Invalidate();
#else
    ::InvalidateRect(hWnd, nullptr, false);
#endif
  }

  /**
   * Invalidates a part of the visible area and schedules a repaint
   * (which will occur in the main thread).
   */
  void Invalidate(const PixelRect &rect) {
#ifndef USE_GDI
    Invalidate();
#else
    ::InvalidateRect(hWnd, &rect, false);
#endif
  }
};

#endif
