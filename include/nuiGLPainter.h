/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiGLPainter_h__
#define __nuiGLPainter_h__

//#include "nui.h"
#include "nuiDrawContext.h"

// Disable GL support on OSX Classic...
#ifdef __NGL_CFM__
//  #define __NUI_NO_GL__
#endif

#ifndef __NUI_NO_GL__

class nuiGLDebugGuard
{
public:
  nuiGLDebugGuard(const nglString& rString)
  {
#if defined DEBUG && defined _UIKIT_
    glPushGroupMarkerEXT(0, rString.GetChars());
#endif
  }

  nuiGLDebugGuard(const char* pString)
  {
#if defined DEBUG && defined _UIKIT_
    glPushGroupMarkerEXT(0, pString);
#endif
  }

  ~nuiGLDebugGuard()
  {
#if defined DEBUG && defined _UIKIT_
    glPopGroupMarkerEXT();
#endif
  }
};

class nuiGLPainter : public nuiPainter, public nuiCacheManager
{
public:
  nuiGLPainter(nglContext* pContext);
  virtual ~nuiGLPainter();
  
  virtual void SetSize(uint32 sizex, uint32 sizey);
  virtual void StartRendering();
  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false);
  virtual void DrawArray(nuiRenderArray* pArray);
  virtual void Clear(bool color, bool depth, bool stencil);
  virtual void BeginSession();
  virtual void EndSession();
  virtual void LoadMatrix(const nuiMatrix& rMatrix);
  virtual void MultMatrix(const nuiMatrix& rMatrix);
  virtual void PushMatrix();
  virtual void PopMatrix();
  virtual void LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix);
  virtual void MultProjectionMatrix(const nuiMatrix& rMatrix);
  virtual void PushProjectionMatrix();
  virtual void PopProjectionMatrix();
  virtual uint32 GetRectangleTextureSupport() const;

  virtual void CreateTexture(nuiTexture* pTexture);
  virtual void DestroyTexture(nuiTexture* pTexture);
  virtual void InvalidateTexture(nuiTexture* pTexture, bool ForceReload);

  virtual void CreateSurface(nuiSurface* pSurface);
  virtual void DestroySurface(nuiSurface* pSurface);
  virtual void ResizeSurface(nuiSurface* pSurface, int32 width, int32 height);

  
protected:
  virtual void ResetOpenGLState();
  void SetSurface(nuiSurface* pSurface);
  nglContext* mpContext;

  void ApplyState(const nuiRenderState& rState, bool ForceApply);
  nuiRenderState mFinalState;
  bool mForceApply;
  static uint32 mActiveContexts;

  virtual void ReleaseCacheObject(void* pHandle);

  uint32 mCanRectangleTexture;
  GLenum mTextureTarget;

  void ApplyTexture(const nuiRenderState& rState, bool ForceApply, int slot);
  
  class TextureInfo
  {
  public:
    TextureInfo();
    
    bool mReload;
    GLint mTexture;
  };
  std::map<nuiTexture*, TextureInfo> mTextures;

  GLenum GetTextureTarget(bool POT) const;
  void UploadTexture(nuiTexture* pTexture, int slot);

  class FramebufferInfo
  {
  public:
    FramebufferInfo();
    
    bool mReload;
    GLint mFramebuffer;
    GLint mTexture; ///< the framebuffer can be used to render to a texture
    GLint mRenderbuffer; ///< or a render buffer
    GLint mDepthbuffer;
    GLint mStencilbuffer;
  };
  std::map<nuiSurface*, FramebufferInfo> mFramebuffers;
  GLint mDefaultFramebuffer, mDefaultRenderbuffer;

  class VertexBufferInfo
  {
  public:
    VertexBufferInfo(nuiRenderArray* pRenderArray = NULL);
    VertexBufferInfo(const VertexBufferInfo& rInfo);

    nuiRenderArray* mpRenderArray;
    GLuint mVertexBuffer;
    std::vector<GLuint> mIndexBuffers;
    std::vector<GLuint> mStreamBuffers;

    void Create(nuiRenderArray* pRenderArray);
    void BindVertices() const;
    void BindStream(int index) const;
    void BindIndices(int index) const;
    void Draw() const;
    void Destroy();
  };
  
  std::map<nuiRenderArray*, VertexBufferInfo> mVertexBuffers;

  bool CheckFramebufferStatus();
  virtual void SetViewport();
  
  int32 mScissorX;
  int32 mScissorY;
  int32 mScissorW;
  int32 mScissorH;
  bool mScissorOn;

  void BlendFuncSeparate(GLenum src, GLenum dst, GLenum srcalpha = GL_ONE, GLenum dstalpha = GL_ONE);
  bool mTwoPassBlend;
  GLenum mSrcColor;
  GLenum mDstColor;
  GLenum mSrcAlpha;
  GLenum mDstAlpha;

  // Local cache:
  bool mClientVertex;
  bool mClientColor;
  bool mClientTexCoord;
  bool mMatrixChanged;
  float mR;
  float mG;
  float mB;
  float mA;
  GLenum mTexEnvMode;
  
  uint32 mViewPort[4];

  bool mUseShaders;

  // Only used for shaders:
  nglVector2f mTextureTranslate;
  nglVector2f mTextureScale;
};

bool nuiCheckForGLErrorsReal();
#define nuiCheckForGLErrors() { NGL_ASSERT(nuiCheckForGLErrorsReal()); }

#ifdef _OPENGL_ES_

#define glCheckFramebufferStatusNUI   glCheckFramebufferStatusOES
#define glFramebufferRenderbufferNUI  glFramebufferRenderbufferOES
#if defined _UIKIT_
#define glRenderbufferStorageNUI(A,B,C,D)      glRenderbufferStorageMultisampleAPPLE(A,4,B,C,D)
#else
#define glRenderbufferStorageNUI      glRenderbufferStorageOES
#endif
#define glGenFramebuffersNUI          glGenFramebuffersOES
#define glDeleteFramebuffersNUI       glDeleteFramebuffersOES
#define glBindFramebufferNUI          glBindFramebufferOES
#define glGenRenderbuffersNUI         glGenRenderbuffersOES
#define glDeleteRenderbuffersNUI      glDeleteRenderbuffersOES
#define glBindRenderbufferNUI         glBindRenderbufferOES
#define glFramebufferTexture2DNUI     glFramebufferTexture2DOES
#define glGetRenderbufferParameterivNUI glGetRenderbufferParameterivOES

#define GL_FRAMEBUFFER_NUI                                GL_FRAMEBUFFER_OES
#define GL_RENDERBUFFER_NUI                               GL_RENDERBUFFER_OES
#define GL_FRAMEBUFFER_BINDING_NUI                        GL_FRAMEBUFFER_BINDING_OES
#define GL_RENDERBUFFER_BINDING_NUI                       GL_RENDERBUFFER_BINDING_OES

// FBO attachement points
#define GL_STENCIL_ATTACHMENT_NUI                         GL_STENCIL_ATTACHMENT_OES
#define GL_DEPTH_ATTACHMENT_NUI                           GL_DEPTH_ATTACHMENT_OES
#define GL_COLOR_ATTACHMENT0_NUI                          GL_COLOR_ATTACHMENT0_OES

// FBO errors:
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_NUI          GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_NUI  GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_NUI          GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_NUI             GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES
#define GL_FRAMEBUFFER_UNSUPPORTED_NUI                    GL_FRAMEBUFFER_UNSUPPORTED_OES
#define GL_FRAMEBUFFER_COMPLETE_NUI                       GL_FRAMEBUFFER_COMPLETE_OES

#define GL_DEPTH_COMPONENT16                              GL_DEPTH_COMPONENT16_OES
#define GL_DEPTH_COMPONENT24                              GL_DEPTH_COMPONENT24_OES

#define GL_RENDERBUFFER_WIDTH_NUI                         GL_RENDERBUFFER_WIDTH_OES
#define GL_RENDERBUFFER_HEIGHT_NUI                        GL_RENDERBUFFER_HEIGHT_OES

//#elif defined(_OPENGL_)
#else

#ifdef _MACOSX_
#define glCheckFramebufferStatusNUI   glCheckFramebufferStatusEXT
#define glFramebufferRenderbufferNUI  glFramebufferRenderbufferEXT
#define glRenderbufferStorageNUI      glRenderbufferStorageEXT
#define glGenFramebuffersNUI          glGenFramebuffersEXT
#define glDeleteFramebuffersNUI       glDeleteFramebuffersEXT
#define glBindFramebufferNUI          glBindFramebufferEXT
#define glGenRenderbuffersNUI         glGenRenderbuffersEXT
#define glDeleteRenderbuffersNUI      glDeleteRenderbuffersEXT
#define glBindRenderbufferNUI         glBindRenderbufferEXT
#define glFramebufferTexture2DNUI     glFramebufferTexture2DEXT
#define glGetRenderbufferParameterivNUI glGetRenderbufferParameteriv
#else
#define glCheckFramebufferStatusNUI   mpContext->glCheckFramebufferStatusEXT
#define glFramebufferRenderbufferNUI  mpContext->glFramebufferRenderbufferEXT
#define glRenderbufferStorageNUI      mpContext->glRenderbufferStorageEXT
#define glGenFramebuffersNUI          mpContext->glGenFramebuffersEXT
#define glDeleteFramebuffersNUI       mpContext->glDeleteFramebuffersEXT
#define glBindFramebufferNUI          mpContext->glBindFramebufferEXT
#define glGenRenderbuffersNUI         mpContext->glGenRenderbuffersEXT
#define glDeleteRenderbuffersNUI      mpContext->glDeleteRenderbuffersEXT
#define glBindRenderbufferNUI         mpContext->glBindRenderbufferEXT
#define glFramebufferTexture2DNUI     mpContext->glFramebufferTexture2DEXT
#define glGetRenderbufferParameterivNUI mpContext->glGetRenderbufferParameteriv
#endif

#define GL_FRAMEBUFFER_NUI                                GL_FRAMEBUFFER_EXT
#define GL_RENDERBUFFER_NUI                               GL_RENDERBUFFER_EXT
#define GL_FRAMEBUFFER_BINDING_NUI                        GL_FRAMEBUFFER_BINDING_EXT
#define GL_RENDERBUFFER_BINDING_NUI                       GL_RENDERBUFFER_BINDING_EXT

// FBO attachement points
#define GL_STENCIL_ATTACHMENT_NUI                         GL_STENCIL_ATTACHMENT_EXT
#define GL_DEPTH_ATTACHMENT_NUI                           GL_DEPTH_ATTACHMENT_EXT
#define GL_COLOR_ATTACHMENT0_NUI                          GL_COLOR_ATTACHMENT0_EXT

// FBO errors:
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_NUI          GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_NUI  GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_NUI          GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_NUI             GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
#define GL_FRAMEBUFFER_UNSUPPORTED_NUI                    GL_FRAMEBUFFER_UNSUPPORTED_EXT
#define GL_FRAMEBUFFER_COMPLETE_NUI                       GL_FRAMEBUFFER_COMPLETE_EXT

#define GL_RENDERBUFFER_WIDTH_NUI                         GL_RENDERBUFFER_WIDTH
#define GL_RENDERBUFFER_HEIGHT_NUI                        GL_RENDERBUFFER_HEIGHT

//#else
//#error "bleh"
#endif


#endif //   #ifndef __NUI_NO_GL__

#endif //__nuiGLPainter_h__

