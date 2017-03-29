#include "AndKKPlayerUI.h"
#include "libavutil/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <linux/videodev2.h>
#include "GlEs2Render.h"
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
CAndKKPlayerUI::CAndKKPlayerUI(int RenderView):m_player(this,&m_Audio)
,m_nRenderType(RenderView),m_playerState(-1)///播放器未启动
,m_pRender(0)
{
    
    m_player.InitSound();
    m_player.SetWindowHwnd(0);
    m_bNeedReconnect= false; 
    if(m_nRenderType==0)	
	    m_pRender = new GlEs2Render(&m_player);
}



CAndKKPlayerUI::~CAndKKPlayerUI()
{
    m_player.CloseMedia();
    m_Audio.CloseAudio();
	
	if(m_nRenderType==0){
	   LOGE("GlEs2Render Del \n");
	   m_RenderLock.Lock();
	   if(m_pRender!=NULL){
       delete m_pRender;
	   m_pRender=NULL;
	   }
	   m_RenderLock.Unlock();
	}
    LOGI("~CAndKKPlayerUI\n");
}

int CAndKKPlayerUI::Init()
{
	if(m_nRenderType==0){
     
		m_pRender-> init(0);
	}
	return 0;
}
		
int CAndKKPlayerUI::OnSize(int w,int h)
{
	
	if(m_nRenderType==0){
        m_pRender->resize(w,h);
	}
	return 0;
}
void CAndKKPlayerUI::SetKeepRatio(int KeepRatio)
{
	m_RenderLock.Lock();
	if(m_pRender!=NULL){
	   GlEs2Render*  pRender =(GlEs2Render* )m_pRender;
	    if(KeepRatio==1)
	        pRender->SetKeepRatio(true);
	    else
			pRender->SetKeepRatio(false);
	}
	m_RenderLock.Unlock();
}
void  CAndKKPlayerUI::Pause()
{
    m_player.Pause();
}
void  CAndKKPlayerUI::Seek(int value)
{
    m_player.AVSeek(value);
}





MEDIA_INFO CAndKKPlayerUI::GetMediaInfo()
{
    return m_player.GetMediaInfo();
}
int  CAndKKPlayerUI::OpenMedia(char *str)
{
    CloseMedia();
    m_Audio.CloseAudio();
    LOGI(" CAndKKPlayerUI %s,%d\n",str, m_playerState);
    if( m_playerState<=-1)
    {
        m_bNeedReconnect=false;
        m_player.CloseMedia();
        m_playerState=m_player.OpenMedia(str);
    }
    return m_playerState;
}


void CAndKKPlayerUI::renderFrame(ANativeWindow* surface)
{
   if(m_nRenderType==0){
	   GlViewRender();
   }else if(m_nRenderType==1){
	      SurfaceViewRender(surface);
   }
}

void CAndKKPlayerUI::SurfaceViewRender(ANativeWindow* surface)
{
	
}
void CAndKKPlayerUI::GlViewRender()
{
	m_RenderLock.Lock();
	if(m_pRender!=NULL){
	GlEs2Render*  pRender =(GlEs2Render* )m_pRender;
	pRender->GlViewRender();
	}
	m_RenderLock.Unlock();
}
bool CAndKKPlayerUI::GetNeedReconnect()
{
    return  m_bNeedReconnect;
}
int CAndKKPlayerUI::GetPlayerState()
{
    return m_playerState;
}


unsigned char* CAndKKPlayerUI::GetWaitImage(int &length,int curtime)
{
    return NULL;
}
unsigned char*  CAndKKPlayerUI::GetCenterLogoImage(int &length)
{
    return NULL;
}
unsigned char* CAndKKPlayerUI::GetErrImage(int &length,int ErrType)
{
	length=0;
	return NULL;
}


unsigned char* CAndKKPlayerUI::GetBkImage(int &length)
{
    return NULL;
}
void CAndKKPlayerUI::OpenMediaFailure(char *strURL,int err)
{
    LOGE("Open Err");
    m_playerState=-2;
    return;
}
//得到延迟
int  CAndKKPlayerUI::GetRealtimeDelay()
{
    return m_player.GetRealtimeDelay();
}
int  CAndKKPlayerUI::SetMinRealtimeDelay(int value)
{
    return  m_player.SetMaxRealtimeDelay(value);
}
//强制刷新Que
void  CAndKKPlayerUI::ForceFlushQue()
{
    m_player.ForceFlushQue();
}
void  CAndKKPlayerUI::AutoMediaCose(int Stata)
{
     if(Stata==-1)
     {
         m_bNeedReconnect=true;
     }
    m_playerState=-3;
}
void CAndKKPlayerUI::AVRender()
{
	
}


int  CAndKKPlayerUI::CloseMedia()
{
    m_player.CloseMedia();
    m_playerState=-1;
    return 0;
}





int CAndKKPlayerUI::GetIsReady()
{
    return  m_player.GetIsReady();
}
int CAndKKPlayerUI::GetRealtime()
{
    return  m_player.GetRealtime();
}
