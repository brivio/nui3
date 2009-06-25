/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLImage.h"
#include "nuiHTTP.h"

/////////////
//class nuiHTMLImage : public nuiHTMLItem
nuiHTMLImage::nuiHTMLImage(nuiHTMLNode* pNode)
: nuiHTMLItem(pNode, true), mpTexture(NULL), mWidth(0), mHeight(0)
{
  const nuiHTMLAttrib* pSrc = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_SRC);
  const nuiHTMLAttrib* pAlt = pNode->GetAttribute(nuiHTMLAttrib::eAttrib_ALT);
  
  if (!pSrc || !pAlt)
    return;
  
  nglString url = pSrc->GetValue();
  printf("incomming url: %ls\n", url.GetChars());
  url = GetAbsoluteURL(url);
  
  printf("url: %ls\n", url.GetChars());
  
  // First look up the cache:
  mpTexture = nuiTexture::GetTexture(nglString(url));
  if (!mpTexture)
  {
    mpTexture = nuiTexture::GetTexture(url, NULL);
    if (!mpTexture->IsValid())
    {
      mpTexture->Release();
      mpTexture = NULL;
      nuiHTTPRequest request(url);
      nuiHTTPResponse* pResponse = request.SendRequest();
      if (!pResponse)
        return;
      
      nglIMemory mem(&pResponse->GetBody()[0], pResponse->GetBody().size());
      mpTexture = nuiTexture::GetTexture(&mem);
      delete pResponse;
    }
  }
  
  mpTexture->SetSource(url);
  
  mWidth = mpTexture->GetWidth();
  mHeight = mpTexture->GetHeight();
}

nuiHTMLImage::~nuiHTMLImage()
{
  
}

void nuiHTMLImage::Draw(nuiDrawContext* pContext)
{
  if (!mpTexture || !mpTexture->IsValid())
    return;
  
  pContext->PushState();
  pContext->SetTexture(mpTexture);  
  pContext->SetFillColor(nuiColor(255, 255, 255));
  pContext->DrawImage(mIdealRect.Size(), nuiRect((float)mpTexture->GetWidth(), (float)mpTexture->GetHeight()));
  pContext->PopState();
}

void nuiHTMLImage::Layout(nuiHTMLContext& rContext)
{    
  if (!mpTexture)
    return;
  mIdealRect.Set(0.0f, 0.0f, mWidth, mHeight);
}