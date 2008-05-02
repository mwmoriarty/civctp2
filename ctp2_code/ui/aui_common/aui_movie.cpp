//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Activision User Interface movie window
// Id           : $Id$
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2 
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Initialized local variables. (Sep 9th 2005 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "c3.h"
#include "aui_ui.h"
#include "aui_surface.h"
#include "aui_rectangle.h"
#include "aui_mouse.h"

#include "aui_movie.h"

#ifdef USE_SDL
#include "SDL.h"
#include "SDL_ffmpeg.h"
#include "SDL_syswm.h"
#include "soundmanager.h"		// g_soundManager
#include "aui_sdlsurface.h"

SDL_ffmpegFile* film;

#endif

WNDPROC aui_Movie::m_windowProc = NULL;
aui_Movie *aui_Movie::m_onScreenMovie = NULL;



aui_Movie::aui_Movie(
	AUI_ERRCODE *retval,
	const MBCHAR *filename )
	:
	aui_Base(){
	*retval = InitCommon( filename );
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;
}



AUI_ERRCODE aui_Movie::InitCommon( const MBCHAR *filename )
{
	m_format = NULL;
	m_surface = NULL;
	m_isOpen = FALSE;
	m_isPlaying = FALSE;
	m_isFinished = FALSE;
	m_isPaused = FALSE;
	m_flags = 0;
	m_timePerFrame = 0;
	m_lastFrameTime = 0;

	m_windowSurface = NULL;
	memset( &m_rect, 0, sizeof( m_rect ) );
	memset(&m_windowRect, 0, sizeof(m_windowRect));

#ifndef USE_SDL
	m_aviFile = NULL;
	m_aviStream = NULL;
	memset( &m_aviFileInfo, 0, sizeof( m_aviFileInfo ) );
	memset( &m_aviStreamInfo, 0, sizeof( m_aviStreamInfo ) );
	m_getFrame = NULL;
#endif
	m_curFrame = 0;

	AUI_ERRCODE errcode = SetFilename( filename );
	Assert( AUI_SUCCESS(errcode) );
	if ( !AUI_SUCCESS(errcode) ) return errcode;

#ifndef USE_SDL
	AVIFileInit();
#endif

	return AUI_ERRCODE_OK;
}



aui_Movie::~aui_Movie()
{
	Unload();

#ifndef USE_SDL
	AVIFileExit();
#endif
}



AUI_ERRCODE aui_Movie::SetFilename( const MBCHAR *filename )
{
	
	Unload();

	memset( m_filename, '\0', sizeof( m_filename ) );

	if ( !filename ) return AUI_ERRCODE_INVALIDPARAM;

	strncpy( m_filename, filename, MAX_PATH );

	
	m_format = (aui_MovieFormat *)
		g_ui->TheMemMap()->GetFileFormat( m_filename );
	Assert( m_format != NULL );
	if ( !m_format ) return AUI_ERRCODE_MEMALLOCFAILED;
	
	return AUI_ERRCODE_OK;
}



AUI_ERRCODE aui_Movie::Load( void )
{
	Assert( m_format != NULL );
	if ( !m_format ) 
            return AUI_ERRCODE_INVALIDPARAM;
	return m_format->Load( m_filename, this );
}



AUI_ERRCODE aui_Movie::Unload( void )
{
	Close();

	if ( m_format )
	{
		g_ui->TheMemMap()->ReleaseFileFormat( m_format );
		m_format = NULL;
	}

	return AUI_ERRCODE_OK;
}



aui_Surface *aui_Movie::SetDestSurface( aui_Surface *surface )
{
	aui_Surface *prevSurface = m_surface;

	if ((m_surface = surface))
	{
		RECT surfRect =
		{
			0,
			0,
			m_surface->Width(),
			m_surface->Height()
		};

		Rectangle_Clip( &m_rect, &surfRect );
	}

	return prevSurface;
}



void aui_Movie::SetDestRect( RECT *rect )
{
	if ( rect )
		SetDestRect( rect->left, rect->top, rect->right, rect->bottom );
	else
		SetDestRect( 0, 0, 0, 0 );
}



void aui_Movie::SetDestRect(
	sint32 left,
	sint32 top,
	sint32 right,
	sint32 bottom )
{
	m_rect.left = left;
	m_rect.top = top;
	m_rect.right = right;
	m_rect.bottom = bottom;

	if ( m_surface )
	{
		RECT surfRect =
		{
			0,
			0,
			m_surface->Width(),
			m_surface->Height()
		};

		Rectangle_Clip( &m_rect, &surfRect );
	}
}



void aui_Movie::GetDestRect(
	sint32 *left,
	sint32 *top,
	sint32 *right,
	sint32 *bottom ) const
{
	*left = m_rect.left;
	*top = m_rect.top;
	*right = m_rect.right;
	*bottom = m_rect.bottom;
}



uint32 aui_Movie::SetTimePerFrame( uint32 timePerFrame )
{
	uint32 prevTimePerFrame = m_timePerFrame;
	m_timePerFrame = timePerFrame;
	return prevTimePerFrame;
}

#ifdef USE_SDL

void audioCallback(void *udata, Uint8 *stream, int len) {

    /* unpack our void pointer */
    SDL_ffmpegFile *file = (SDL_ffmpegFile*)udata;

    int bytesUsed;
    int offset = 0;
    SDL_ffmpegAudioFrame *frame = SDL_ffmpegGetAudioFrame(file);
    if(!frame) return;

    while(len > 0) {

        /* check if we need a new frame */
        if(!frame->size) {
            frame = SDL_ffmpegGetAudioFrame(file);
            if(!frame) return;
        }

        if(frame->size <= len) {
            /* this frame is smaller or equal to the amount of data needed. */
            bytesUsed = frame->size;
        } else {
            /* this frame has more data than needed */
            bytesUsed = len;
        }

        /* copy the correct amount of data */
        memcpy(stream+offset, frame->buffer, bytesUsed);
        /* adjust the needed length accordingly */
        len -= bytesUsed;
        offset += bytesUsed;

        /* we release our audio data, so the decode thread can fill it again */
        /* we also inform this function of the amount of bytes we used, so it can */
        /* move the buffer accordingly */
        /* important! this call is paired with SDL_ffmpegGetAudio */
        SDL_ffmpegReleaseAudio(file, frame, bytesUsed);
    }

    return;
}

#endif


AUI_ERRCODE aui_Movie::Open(
	uint32 flags,
	aui_Surface *surface,
	RECT *rect )
{
if ( !m_isOpen )
    {
    m_flags = flags;

    if ( surface )
        SetDestSurface( surface );
    if ( rect )
        SetDestRect( rect );

    uint32 err;
#ifndef USE_SDL
    err = AVIFileOpen(
        &m_aviFile,
        m_filename,
        OF_READ,
        NULL );
    Assert( err == 0 );
    if ( err ) return AUI_ERRCODE_HACK;

		

    err = AVIFileInfo(
        m_aviFile,
        &m_aviFileInfo,
        sizeof( m_aviFileInfo ) );
    Assert( err == 0 );
    if ( err ) return AUI_ERRCODE_HACK;

    err = AVIFileGetStream(
        m_aviFile,
        &m_aviStream,
        streamtypeVIDEO,
        0 );
    Assert( err == 0 );
    if ( err ) return AUI_ERRCODE_HACK;

		

    err = AVIStreamInfo(
        m_aviStream,
        &m_aviStreamInfo,
        sizeof( m_aviStreamInfo ) );
    Assert( err == 0 );
    if ( err ) return AUI_ERRCODE_HACK;

    BITMAPINFOHEADER bih;
    memset( &bih, 0, sizeof( bih ) );
    bih.biSize = sizeof( bih );
    bih.biWidth = m_aviFileInfo.dwWidth;
    bih.biHeight = m_aviFileInfo.dwHeight;
    bih.biPlanes = 1;
    bih.biBitCount = (uint16)g_ui->Primary()->BitsPerPixel();
    bih.biCompression = BI_RGB;

    m_getFrame = AVIStreamGetFrameOpen(
        m_aviStream,
        &bih );
    Assert( m_getFrame != NULL );
    if ( !m_getFrame ) return AUI_ERRCODE_HACK;

		
    m_timePerFrame =
        m_aviStreamInfo.dwScale * 1000 / m_aviStreamInfo.dwRate;

		
    m_curFrame = m_aviStreamInfo.dwStart;

		
    m_rect.right = m_rect.left + m_aviStreamInfo.rcFrame.right;
    m_rect.bottom = m_rect.top + m_aviStreamInfo.rcFrame.bottom;
#else //replaces a aui_sdlmovie::Open(

    //stop game sound
    g_soundManager->ReleaseSoundDriver();

//code from SDL_ffmpeg example:

    film= SDL_ffmpegOpen(m_filename);
    if (film){
        printf("%s L%d: Opened %s!\n", __FILE__, __LINE__, m_filename);
        SDL_ffmpegSelectVideoStream(film, 0);
        //SDL_ffmpegSelectAudioStream(film, 0);

        //get the audiospec which fits the selected videostream, if no videostream
        // is selected, default values are used (2 channel, 48Khz)
        //SDL_AudioSpec *specs = SDL_ffmpegGetAudioSpec(film, 512, audioCallback);

        int w,h;
        // we get the size from our active video stream
        if(SDL_ffmpegGetVideoSize(film, &w, &h))
            printf("%s L%d: Could not determin movie size!\n", __FILE__, __LINE__);
        else{
            m_rect.right = m_rect.left + w;
            m_rect.bottom = m_rect.top + h;
            printf("%s L%d: Movie size: %dx%d!\n", __FILE__, __LINE__, w, h);
            }
        //m_timePerFrame = 
        //m_curFrame = 
        }
    else
        printf("%s L%d: Could not open %s!\n", __FILE__, __LINE__, m_filename);

    // Audio already opened by Mix_OpenAudio in the soundmanager
/*    if( SDL_OpenAudio(specs, 0) < 0 ) {
      printf("%s L%d: Couldn't open audio: %s\n", __FILE__, __LINE__, SDL_GetError());
*/
 
#endif
    m_isOpen = TRUE; //even if open failed, loops otherwise
    m_isPlaying = FALSE;
    m_isPaused = FALSE;
    }
	
return AUI_ERRCODE_OK;
}



AUI_ERRCODE aui_Movie::Close( void )
{
	if ( m_isOpen )
	{
		
		Stop();

#ifndef USE_SDL
		if ( m_getFrame )
		{
			AVIStreamGetFrameClose( m_getFrame );
			m_getFrame = NULL;
		}

		if ( m_aviStream )
		{
			AVIStreamRelease( m_aviStream );
			m_aviStream = NULL;
		}

		if ( m_aviFile )
		{
			AVIFileRelease( m_aviFile );
			m_aviFile = NULL;
		}
#else
                if(film){
                    SDL_ffmpegFree(film);
                    printf("%s L%d: Movie closed!\n", __FILE__, __LINE__);
                    }
#endif

		m_isOpen = FALSE;
	}

	return AUI_ERRCODE_OK;
}



AUI_ERRCODE aui_Movie::Play( void ){
    if ( !m_isPlaying )
        {
		
        Open();

#ifndef USE_SDL
        uint32 err = AVIStreamBeginStreaming(
            m_aviStream,
            0,
            m_aviStreamInfo.dwLength,
            1000 );
        Assert( err == 0 );
        if ( err ) return AUI_ERRCODE_HACK;
#else

/*
//mplayer "movie manager"
  SDL_SysWMinfo info;
  char cmd[1024];
  cmd[0]=0;

  SDL_VERSION(&info.version); // this is important!
  if (SDL_GetWMInfo(&info))
  snprintf(cmd, sizeof(cmd), "mplayer -vo xv -wid 0x%lx /media/cdrom/Setup/data/Max/ctp2_data/default/videos/%s", info.info.x11.window, m_filename);
  else
  snprintf(cmd, sizeof(cmd), "mplayer -vo sdl /media/cdrom/Setup/data/Max/ctp2_data/default/videos/%s", m_filename);
  printf("%s L%d: Trying to execute: %s!\n", __FILE__, __LINE__, cmd);
  printf("%s L%d: The path to the movies is hard coded! Solve it! ;)\n", __FILE__, __LINE__);
  g_soundManager->ReleaseSoundDriver(); 
  system(cmd);
  g_soundManager->ReacquireSoundDriver();//no function with SDL, see soundmanager.cpp
*/

        // we start our decode thread, this always tries to buffer in some frames
        // so we can enjoy smooth playback 
        if(film){
            SDL_ffmpegStartDecoding(film); //returns always 0!
            SDL_ffmpegPause(film, 0);//unpause film
            }
#endif
        if ( m_flags & k_AUI_MOVIE_PLAYFLAG_ONSCREEN )
            PlayOnScreenMovie();
        }
    m_isPlaying = TRUE;//regardless if open failed, loops otherwise
    m_isPaused = FALSE;

    return AUI_ERRCODE_OK;
    }





AUI_ERRCODE aui_Movie::PlayOnScreenMovie( void )
{
	aui_Mouse *mouse = g_ui->TheMouse();
	sint32 numEvents;
	static aui_MouseEvent mouseEvents[ k_MOUSE_MAXINPUT ];
	aui_MouseEvent *mouseState = NULL;

	if (mouse) {
		
	
		numEvents = mouse->ManipulateInputs( mouseEvents, FALSE );
			
		mouseState = numEvents ?
			mouseEvents + numEvents - 1 :
			mouse->GetLatestMouseEvent();

		mouse->Hide();
	}

#ifndef USE_SDL
	MSG msg;
	m_windowProc = (WNDPROC)GetWindowLong( g_ui->TheHWND(), GWL_WNDPROC );
	SetWindowLong( g_ui->TheHWND(), GWL_WNDPROC, (LONG)OnScreenMovieWindowProc );

	m_onScreenMovie = this;

	
	while ( !m_isFinished && m_isPlaying )
	{
		
		while ( ShowCursor( FALSE ) >= 0 )
			;

		Process();

		
		
		if ( PeekMessage( &msg, g_ui->TheHWND(), 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		
		if (mouse) {
			numEvents = mouse->ManipulateInputs( mouseEvents, FALSE );
			aui_MouseEvent *curEvent = mouseEvents;
			for ( sint32 k = numEvents; k; k--, curEvent++ )
			{
				if ( curEvent->lbutton != mouseState->lbutton
				||   curEvent->rbutton != mouseState->rbutton )
				{
					Stop();
					break;
				}
			}
		}
	}

	m_onScreenMovie = NULL;

	SetWindowLong( g_ui->TheHWND(), GWL_WNDPROC, (LONG)m_windowProc );
	m_windowProc = NULL;
#else
        printf("%s L%d: SDL mouse code is missing here!\n", __FILE__, __LINE__);
	//m_onScreenMovie = this;

	
	while ( !m_isFinished && m_isPlaying ){
            Process();
            }
	//m_onScreenMovie = NULL;

#endif

	if (mouse)
		mouse->Show();

	
	g_ui->AddDirtyRect( &m_rect ); //does this set the rect that needs to be repainted later on?

	
	return Stop();
}



AUI_ERRCODE aui_Movie::Stop( void ){
    if ( m_isPlaying )
	{
#ifndef USE_SDL
        uint32 err = AVIStreamEndStreaming( m_aviStream );
        Assert( err == 0 );
        if ( err ) return AUI_ERRCODE_HACK;
#else
        if(film)
            SDL_ffmpegPause(film, 1);//pause film
        //g_soundManager->ReacquireSoundDriver(); //no function with SDL, see soundmanager.cpp
#endif
        }
    m_isPlaying = FALSE;
    m_isPaused = FALSE;
    return AUI_ERRCODE_OK;
    }



AUI_ERRCODE aui_Movie::Pause( void )
{
	if ( m_isPlaying && !m_isPaused )
	{
#ifdef USE_SDL
        if(film)
            SDL_ffmpegPause(film, 1);//pause film
#endif
		m_isPaused = TRUE;
	}

	return AUI_ERRCODE_OK;
}



AUI_ERRCODE aui_Movie::Resume( void )
{
	if ( m_isPlaying && m_isPaused )
	{
#ifdef USE_SDL
        if(film)
            SDL_ffmpegPause(film, 0);//pause film
#endif
		m_isPaused = FALSE;
	}

	return AUI_ERRCODE_OK;
}



AUI_ERRCODE aui_Movie::Process( void ){
    AUI_ERRCODE retval = AUI_ERRCODE_UNHANDLED;

#ifndef USE_SDL
    if ( m_isPlaying && !m_isPaused )
        {
        uint32 time = GetTickCount();
        if ( time - m_lastFrameTime > m_timePerFrame )
            {
			
            aui_Surface *surface =
                ( m_flags & k_AUI_MOVIE_PLAYFLAG_ONSCREEN ) ?
                g_ui->Primary() :
                m_surface;
            //each frame is displayed as a bitmap!
            uint8 *frame = (uint8 *)AVIStreamGetFrame( m_getFrame, m_curFrame );
            Assert( frame != NULL );
            if ( !frame ) return AUI_ERRCODE_HACK;

            BITMAPINFOHEADER *bih = (BITMAPINFOHEADER *)frame;

            sint32 bmpHeight = bih->biHeight;
            sint32 bmpByteWidth = bih->biWidth * ( bih->biBitCount >> 3 );
            sint32 bmpPitch = bmpByteWidth + ( uint32(-bmpByteWidth) & 0x3 );

			
            frame += sizeof( BITMAPINFOHEADER );

			
            uint8 *buffer = NULL;
            AUI_ERRCODE errcode = surface->Lock( &m_rect, (LPVOID *)&buffer, 0 );
            Assert( AUI_SUCCESS(errcode) );
            if ( !AUI_SUCCESS(errcode) ) return AUI_ERRCODE_SURFACELOCKFAILED;

            const uint8 *origBuffer = buffer;
            const sint32 pitch = surface->Pitch();

			
            if ( bmpHeight > 0 )
                {
                frame += bmpPitch * ( bmpHeight - 1 );

                for ( sint32 i = bmpHeight; i; i-- )
                    {
                    memcpy( buffer, frame, bmpByteWidth );
                    buffer += pitch;
                    frame -= bmpPitch;
                    }
                }
            else
                {
				
				

                bmpHeight = -bmpHeight;

                for ( sint32 i = bmpHeight; i; i-- )
                    {
                    memcpy( buffer, frame, bmpByteWidth );
                    buffer += pitch;
                    frame += bmpPitch;
                    }
                }

            errcode = surface->Unlock( (LPVOID)origBuffer );
            Assert( AUI_SUCCESS(errcode) );
            if ( !AUI_SUCCESS(errcode) ) return AUI_ERRCODE_SURFACEUNLOCKFAILED;

            if ( ++m_curFrame == m_aviStreamInfo.dwLength )
                {
                m_curFrame = 0;
                if ( !(m_flags & k_AUI_MOVIE_PLAYFLAG_LOOP) )
                    m_isFinished = TRUE;
                }

            m_lastFrameTime = time;

            retval = AUI_ERRCODE_HANDLED;
            }
        }
#else
/*
  create aui_sdlmovie.cpp containing:
  aui_sdlmovie::Open(uint32 flags,aui_Surface *surface,RECT *rect ){
  SDL_ffmpegFile* sff= SDL_ffmpegOpen(filname);
  }

  implement actual displaying of frames here:
  SDL_ffmpegVideoFrame* svf= SDL_ffmpegGetVideoFrame(sff);
  blit svf onto surface... (get clues from above;)
*/

//from SDL_ffmpeg example:
    if ( m_isPlaying && !m_isPaused )
        {

        if(film){
            aui_Surface *surface = ( m_flags & k_AUI_MOVIE_PLAYFLAG_ONSCREEN ) ?
                g_ui->Primary() :
                m_surface;

            int s;
            SDL_ffmpegStream *str;

/*
// print some info on detected stream to output 
for(s = 0; s<film->AStreams; s++) {
str = SDL_ffmpegGetAudioStream(film, s);

printf("Info on audiostream #%i:\n", s);
printf("\tChannels: %i\n",      str->channels);
if(strlen(str->language)) printf("\tLanguage: %s\n",      str->language);
printf("\tSampleRate: %i\n",    str->sampleRate);
}



for(s = 0; s<film->VStreams; s++) {
str = SDL_ffmpegGetVideoStream(film, s);

printf("Info on videostream #%i:\n", s);
if(strlen(str->language)) printf("\tLanguage: %s\n",      str->language);
printf("\tFrame: %ix%i\n",  str->width, str->height);
printf("\tFrameRate: %.2ffps\n",  1.0 / (str->frameRate[0] / str->frameRate[1]));
}
*/
            /* we unpause the audio so our audiobuffer gets read */
            //SDL_PauseAudio(0);
            //SDL_ffmpegPause(film, 0);//unpause film

            SDL_ffmpegVideoFrame* frame= 0;
            SDL_Rect sdl_rect = { m_rect.left, m_rect.top, m_rect.right-m_rect.left, m_rect.bottom-m_rect.top };
            aui_SDLSurface* sdl_surf = static_cast<aui_SDLSurface*>(surface);

            int64_t duration= SDL_ffmpegGetDuration(film);
           
            //if(!m_isFinished) {//don't wait until frame is ready to be shown
            while(!frame && !m_isFinished){ //wait until there is a frame to be shown
                frame = SDL_ffmpegGetVideoFrame(film);
                SDL_Delay(5); //too much for 25 fps but here we have only 15 fps

                if(frame){
                    //printf("%s L%d: Got frame!\n", __FILE__, __LINE__);

                    // we got a frame, so we better show this one 
                    SDL_BlitSurface(frame->buffer, 0, sdl_surf->DDS(), &sdl_rect);

                    // After releasing this frame, you can no longer use it. 
                    // you should call this function every time you get a frame! 
                    //not in svn sdl_ffmpeg!
                    //SDL_ffmpegReleaseVideo(film, frame);

                    // we flip the double buffered screen so we might actually see something 
                    SDL_Flip(sdl_surf->DDS());
                    }
                m_isFinished= SDL_ffmpegGetPosition(film) >= duration;
                }
            
            if(m_isFinished){
                printf("%s L%d: Done displaying movie!\n", __FILE__, __LINE__);
                m_isPlaying= FALSE;
                }
            }
        else
            m_isFinished= TRUE;
        }
    retval = AUI_ERRCODE_HANDLED;
#endif

    return retval;
    }



#ifndef USE_SDL
LRESULT CALLBACK OnScreenMovieWindowProc(
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam )
{
	if ( aui_Movie::m_onScreenMovie )
	{
		g_ui->HandleWindowsMessage( hwnd, message, wParam, lParam );

		switch ( message )
		{
		
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
			aui_Movie::m_onScreenMovie->Stop();
			break;

		case WM_CLOSE:
			if ( hwnd != g_ui->TheHWND() ) break;

			
			aui_Movie::m_onScreenMovie->Close();

			PostMessage( g_ui->TheHWND(), WM_CLOSE, 0, 0 );
			
			return 0;
		}
	}

	LRESULT lr = CallWindowProc(
		(long(__stdcall *)(HWND, unsigned int, unsigned int, long))aui_Movie::m_windowProc,
		hwnd, message, wParam, lParam );

	return lr;
}
#endif
