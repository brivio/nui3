/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiScrollView_h__
#define __nuiScrollView_h__

//#include "nui.h"
#include "nuiWidget.h"
#include "nuiSimpleContainer.h"
#include "nuiScrollBar.h"
#include "nuiContainer.h"
#include "nuiWidgetAnimation.h"

class nuiScrollView : public nuiSimpleContainer
{
public:
  nuiScrollView(bool Horizontal = true, bool Vertical = true);
  nuiScrollView(nuiScrollBar* pHorizontalScrollbar, nuiScrollBar* pVerticalScrollbar );
  virtual ~nuiScrollView();
  
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);

  void Scrolled(const nuiEvent& rEvent);

  void SetFillChildren(bool Set); ///< If filling is enabled the scroll view will try to move its child in order to honor the available space
  bool GetFillChildren();

  virtual bool Clear();

  virtual bool MouseClicked(const nglMouseInfo& rInfo);
  virtual bool MouseUnclicked(const nglMouseInfo& rInfo);
  virtual bool MouseMoved(const nglMouseInfo& rInfo);
  virtual bool MouseWheelMoved(const nglMouseInfo& rInfo);
  virtual bool MouseCanceled(const nglMouseInfo& rInfo);

  void Dragged(const nglMouseInfo& rInfo);

  const nuiRect& GetChildrenUnionRect() { return mChildrenUnionRect; }

  virtual nuiRange* GetRange(nuiOrientation Orientation);
  nuiScrollBar* GetScrollBar(nuiOrientation Orientation);

  void SetHIncrement(nuiSize hIncrement);
  nuiSize GetHIncrement() const;
  void SetVIncrement(nuiSize vIncrement);
  nuiSize GetVIncrement() const;

  void SetBarSize(float Set)    { mBarSize = Set; InvalidateLayout(); }

  void EnableSmoothScrolling(bool set);
  bool IsSmoothScrollingEnabled() const;

  void EnableMinimalResize(bool set);
  bool IsMinimalResizeEnabled() const;

  nuiSimpleEventSource<0> OffsetsChanged;
  nuiSize GetXOffset() const;
  nuiSize GetYOffset() const;
  void SetXOffset(nuiSize offset);
  void SetYOffset(nuiSize offset);

  nuiSize GetXPos() const;
  nuiSize GetYPos() const;
  void SetXPos(nuiSize pos);
  void SetYPos(nuiSize pos);
  
  void SetForceNoSmartScroll(bool set);
  bool GetForceNoSmartScroll(bool set) const;

  bool GetEnableHorizontalScroll() const;
  void SetEnableHorizontalScroll(bool set);
  bool GetEnableVerticalScroll() const;
  void SetEnableVerticalScroll(bool set);
  
  void SetHideScrollBars(bool hide);
  bool GetHideScrollBars();
  
  void EnableDrag(bool enable);
  bool IsDragEnabled();
  
  void ActivateMobileMode();

  void ActivateHotRect(bool hset, bool vset);
  void ActivateHotRect(bool set);
  bool IsVerticalHotRectActive() const;
  bool IsHorizontalHotRectActive() const;

private:
  void InitAttributes();
  void Init(nuiScrollBar* pHorizontalScrollBar, nuiScrollBar* pVerticalScrollBar, bool Horizontal, bool Vertical);

  void OnChildAdded(const nuiEvent& rEvent);
  void OnChildRemoved(const nuiEvent& rEvent);
  void OnHotRectChanged(const nuiEvent& rEvent);
  bool mHorizontalHotRectActive;
  bool mVerticalHotRectActive;

  nuiRect mChildrenUnionRect;
  bool SetChildrenRect(nuiSize x, nuiSize y, nuiSize xx, nuiSize yy, nuiSize scrollv, nuiSize scrollh);

  nuiScrollBar* mpHorizontal;
  nuiScrollBar* mpVertical;

  bool mForceNoHorizontal;
  bool mForceNoVertical;
  bool mForceNoSmartScroll;

  bool mHorizontalIsExternal;
  bool mVerticalIsExternal;

  float mBarSize;

  bool mFillChildren;
  nuiSize mVIncrement, mHIncrement;

  void OnSmoothScrolling(const nuiEvent& rEvent);
  
  void OnHThumbPressed(const nuiEvent& rEvent);
  void OnHThumbDepressed(const nuiEvent& rEvent);
  void OnVThumbPressed(const nuiEvent& rEvent);
  void OnVThumbDepressed(const nuiEvent& rEvent);
  
  void ShowScrollBars(bool autoHide = false);
  void HideScrollBars();
  void OnHideTick(const nuiEvent& rEvent);
  
  bool mHThumbPressed;
  bool mVThumbPressed;
  bool mSmoothScrolling;
  nuiSize mXOffset; // Used for Smooth Scrolling
  nuiSize mYOffset;
  nuiTimer* mpSmoothTimer;
  bool mTimerOn;

  bool mMinimalResize;
  nuiRect mOldIdealRect;
  
  int mLeftClick;
  double mClickValueH;
  double mClickValueV;
  nuiSize mSpeedX;
  nuiSize mSpeedY;
  nuiSize mLastX;
  nuiSize mLastY;
  
  bool mDragEnabled;
  
  bool mHideScrollBars;
  nuiFadeInWidgetAnim* mpShowAnimH;
  nuiFadeInWidgetAnim* mpShowAnimV;
  nuiFadeOutWidgetAnim* mpHideAnimH;
  nuiFadeOutWidgetAnim* mpHideAnimV;
  nuiTimer* mpHideTimer;
  nglTime mLastTime;

  bool PreMouseClicked(const nglMouseInfo& rInfo);
  bool PreMouseUnclicked(const nglMouseInfo& rInfo);
  bool PreMouseMoved(const nglMouseInfo& rInfo);

  nglMouseInfo mTouch;
  bool mTouched;
  nuiEventSink<nuiScrollView> mSVSink;
};

#endif // __nuiScrollView_h__

