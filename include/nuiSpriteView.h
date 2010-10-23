/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

class nuiTexture;

class nuiSpriteFrame
{
public:
  nuiSpriteFrame();
  virtual ~nuiSpriteFrame();
  
  bool SetTexture(nuiTexture* pTexture, const nuiRect& rRect);
  bool SetTexture(const nglPath& rPath, const nuiRect& rRect);
  bool SetTexture(const nglPath& rPath);

  void SetRect(const nuiRect& rRect);
  void SetHandle(float x, float y);
  
  nuiTexture* GetTexture() const;
  const nuiRect& GetRect();
  
  float GetHandleX() const;
  float GetHandleY() const;

protected:
  nuiTexture* mpTexture;
  nuiRect mRect; ///< Rectangle inside mpTexture occupied by the sprite pixels
  float mX, mY; ///< Handle position
};

class nuiSpriteAnimation
{
public:
  nuiSpriteAnimation();
  virtual ~nuiSpriteAnimation();
  
  int32 GetFrameCount() const;
  void AddFrame(nuiSpriteFrame* pFrame);
  const nuiSpriteFrame* GetFrame(int32 index) const;
  
  void SetDuration(float seconds);
  void SetFPS(float FPS);
  float GetDuration() const;
  float GetFPS() const;
  
  void SetName(const nglString& rName);
  const nglString& GetName() const;
  
protected:
  nglString mName;
  std::vector<nuiSpriteFrame*> mpFrames;
  float mFPS;
};

class nuiSpriteDef : public nuiObject
{
public:
  nuiSpriteDef(const nglString& rObjectName);
  virtual ~nuiSpriteDef();
  
  void AddAnimation(nuiSpriteAnimation* pAnim);
  int32 GetAnimationCount() const;
  const nuiSpriteAnimation* GetAnimation(int32 index) const;
  
  static nuiSpriteDef* GetSprite(const nglString& rSpriteDefName);
  
protected:
  static std::map<nglString, nuiSpriteDef*> mSpriteMap;
  void Init();
  std::vector<nuiSpriteAnimation*> mpAnimations;
};

class nuiSprite : public nuiObject
{
public:
  nuiSprite(const nglString& rSpriteDefName);
  nuiSprite(nuiSpriteDef* pSpriteDef);
  virtual ~nuiSprite();

  const nuiSpriteDef* GetDefinition() const;

  /** @name Matrix Transformation Support */
  //@{
  void AddMatrixNode(nuiMatrixNode* pNode);
  void DelMatrixNode(uint32 index);
  int32 GetMatrixNodeCount() const;
  nuiMatrixNode* GetMatrixNode(uint32 index) const;

  void LoadIdentityMatrix();
  bool IsMatrixIdentity() const;
  void GetMatrix(nuiMatrix& rMatrix) const;
  nuiMatrix GetMatrix() const;
  void SetMatrix(const nuiMatrix& rMatrix);
  //@}
protected:
  void Init();
  static nuiMatrix mIdentityMatrix;
  std::vector<nuiMatrixNode*>* mpMatrixNodes;
  
  nuiSpriteDef* mpSpriteDef;
};


class nuiSpriteView : public nuiSimpleContainer
{
public:
  nuiSpriteView();
  virtual ~nuiSpriteView();
  
  void AddSprite(nuiSprite* pSprite);
  void DelSprite(nuiSprite* pSprite);
  
};