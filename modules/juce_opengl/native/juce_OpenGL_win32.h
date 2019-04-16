/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

extern ComponentPeer* createNonRepaintingEmbeddedWindowsPeer (Component&, void* parent);
extern bool shouldScaleGLWindow (void* hwnd);


// SMODE add debug api
#ifdef JUCE_DEBUG
# define GL_DEBUG_SOURCE_API_ARB 0x8246
# define GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB 0x8247
# define GL_DEBUG_SOURCE_SHADER_COMPILER_ARB 0x8248
# define GL_DEBUG_SOURCE_THIRD_PARTY_ARB 0x8249
# define GL_DEBUG_SOURCE_APPLICATION_ARB 0x824A
# define GL_DEBUG_SOURCE_OTHER_ARB 0x824B
# define GL_DEBUG_TYPE_ERROR_ARB 0x824C
# define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB 0x824D
# define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB 0x824E
# define GL_DEBUG_TYPE_PORTABILITY_ARB 0x824F
# define GL_DEBUG_TYPE_PERFORMANCE_ARB 0x8250
# define GL_DEBUG_TYPE_OTHER_ARB 0x8251
# define GL_DEBUG_SEVERITY_HIGH_ARB 0x9146
# define GL_DEBUG_SEVERITY_MEDIUM_ARB 0x9147
# define GL_DEBUG_SEVERITY_LOW_ARB 0x9148
# define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242

typedef void (APIENTRY *DEBUGPROC)(GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar *message,
  const void *userParam);

#define JUCE_GL_DEBUG_FUNCTIONS(USE_FUNCTION) \
  USE_FUNCTION (glDebugMessageControl, void, (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled), (source, type, severity, count, ids, enabled)) \
  USE_FUNCTION (glDebugMessageCallback, void, (DEBUGPROC callback, void * userParam), (callback, userParam))

static void APIENTRY juceDebugCallback(GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar *message,
  const void *userParam)
{
  String debugSource, debugType, debugSeverity;
  switch (source)
  {
  case GL_DEBUG_SOURCE_API_ARB: debugSource = "OpenGL"; break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: debugSource = "Windows"; break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: debugSource = "Shader Compiler"; break;
  case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: debugSource = "Third Party"; break;
  case GL_DEBUG_SOURCE_APPLICATION_ARB: debugSource = "Application"; break;
  case GL_DEBUG_SOURCE_OTHER_ARB:
  default:
    debugSource = "Other";
  }

  switch (type)
  {
  case GL_DEBUG_TYPE_ERROR_ARB: debugType = "Error"; break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: debugType = "Deprecated behavior"; break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: debugType = "Undefined behavior"; break;
  case GL_DEBUG_TYPE_PORTABILITY_ARB: debugType = "Portability"; break;
  case GL_DEBUG_TYPE_PERFORMANCE_ARB: debugType = "Performance"; break;
  case GL_DEBUG_TYPE_OTHER_ARB:
  default:
    debugType = "Other"; break;
  }

  switch (severity)
  {
  case GL_DEBUG_SEVERITY_HIGH_ARB: debugSeverity = "High"; break;
  case GL_DEBUG_SEVERITY_MEDIUM_ARB: debugSeverity = "Medium"; break;
  case GL_DEBUG_SEVERITY_LOW_ARB: debugSeverity = "Low"; break;
  default:
    debugSeverity = "Other"; break;
  }

  juce::Logger::outputDebugString(debugSource + " " + debugType + " " + debugSeverity + " " + String(message));
  if (type == GL_DEBUG_TYPE_ERROR_ARB || type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
  {
    jassertfalse;
  }
};
#endif // JUCE_DEBUG

#if JUCE_MODULE_AVAILABLE_juce_audio_plugin_client && (JucePlugin_Build_VST || JucePlugin_Build_VST3)
 bool juce_shouldDoubleScaleNativeGLWindow();
#else
 bool juce_shouldDoubleScaleNativeGLWindow()  { return false; }
#endif

//==============================================================================
class OpenGLContext::NativeContext
   #if JUCE_WIN_PER_MONITOR_DPI_AWARE
    : private Timer
   #endif
{
public:
    NativeContext (Component& component,
                   const OpenGLPixelFormat& pixelFormat,
                   void* contextToShareWith,
                   bool /*useMultisampling*/,
                   OpenGLVersion version)
    {
        dummyComponent.reset (new DummyComponent (*this));
        createNativeWindow (component);

        PIXELFORMATDESCRIPTOR pfd;
        initialisePixelFormatDescriptor (pfd, pixelFormat);

        auto pixFormat = ChoosePixelFormat (dc, &pfd);

        if (pixFormat != 0)
            SetPixelFormat (dc, pixFormat, &pfd);

        renderContext = wglCreateContext (dc);

        if (renderContext != 0)
        {
            makeActive();
            initialiseGLExtensions();

            auto wglFormat = wglChoosePixelFormatExtension (pixelFormat);
            deactivateCurrentContext();

#ifndef JUCE_OPENGL3 // SMODE: enforce delete old context and create a new one according to specified version
            if (wglFormat != pixFormat && wglFormat != 0)
#endif  // JUCE_OPENGL3 SMODE
            {
                // can't change the pixel format of a window, so need to delete the
                // old one and create a new one..
                releaseDC();
                nativeWindow = nullptr;
                createNativeWindow (component);

                if (SetPixelFormat (dc, wglFormat, &pfd))
                {
                    deleteRenderContext();
                    renderContext = createVersionnedContext(dc, (HGLRC)contextToShareWith, version); /* SMODE */
                }
            }

#ifdef JUCE_OPENGL3 // SMODE createVersionnedContext could manage context sharing
            if (version != openGL3_2)
#endif // JUCE_OPENGL3 
            if (contextToShareWith != nullptr)
                wglShareLists ((HGLRC) contextToShareWith, renderContext);

            component.getTopLevelComponent()->repaint();
            component.repaint();
        }
    }

    ~NativeContext()
    {
        deleteRenderContext();
        releaseDC();
    }

#ifdef JUCE_DEBUG 
#define JUCE_DECLARE_GL_FUNCTION(name, returnType, params, callparams)      typedef returnType (__stdcall *type_ ## name) params; type_ ## name name;
    JUCE_GL_DEBUG_FUNCTIONS(JUCE_DECLARE_GL_FUNCTION)
#endif // JUCE_DEBUG

    bool initialiseOnRenderThread (OpenGLContext& c)
    {
        context = &c;
        
#ifdef JUCE_DEBUG // SMODE add debug api
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE); // activate all debug output
        glDebugMessageCallback(juceDebugCallback, nullptr);
#endif // JUCE_DEBUG  

        return true;
    }

    void shutdownOnRenderThread()           
    { 
      
#ifdef JUCE_DEBUG // SMODE add debug api
      glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(nullptr, nullptr);
#endif // JUCE_DEBUG

      deactivateCurrentContext(); 
      context = nullptr; 
    }

    static void deactivateCurrentContext()  { wglMakeCurrent (0, 0); }
    bool makeActive() const noexcept        
    { 
      if (isActive())
        return true;

      bool res = wglMakeCurrent(dc, renderContext) != FALSE;
      if (!res) // SMODE get the last error just in case
      {
        DWORD lastError = GetLastError();
        jassert(lastError == ERROR_SUCCESS); // should assert
      }
      return res;
    }
    bool isActive() const noexcept          { return wglGetCurrentContext() == renderContext; }
    void swapBuffers() const noexcept       { SwapBuffers (dc); }

    bool setSwapInterval (int numFramesPerSwap)
    {
        jassert (isActive()); // this can only be called when the context is active..
        return wglSwapIntervalEXT != nullptr && wglSwapIntervalEXT (numFramesPerSwap) != FALSE;
    }

    int getSwapInterval() const
    {
        jassert (isActive()); // this can only be called when the context is active..
        return wglGetSwapIntervalEXT != nullptr ? wglGetSwapIntervalEXT() : 0;
    }

    void updateWindowPosition (Rectangle<int> bounds)
    {
        if (nativeWindow != nullptr)
        {
            if (! approximatelyEqual (nativeScaleFactor, 1.0))
                bounds = (bounds.toDouble() * nativeScaleFactor).toNearestInt();

            SetWindowPos ((HWND) nativeWindow->getNativeHandle(), 0,
                          bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                          SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER);
        }
    }

    bool createdOk() const noexcept                 { return getRawContext() != nullptr; }
    void* getRawContext() const noexcept            { return renderContext; }
    unsigned int getFrameBufferID() const noexcept  { return 0; }

    void triggerRepaint()
    {
        if (context != nullptr)
            context->triggerRepaint();
    }

    struct Locker { Locker (NativeContext&) {} };

    double getWindowScaleFactor (const Rectangle<int>& screenBounds)
    {
        if (nativeWindow != nullptr && shouldScaleGLWindow (nativeWindow->getNativeHandle()))
            return Desktop::getInstance().getDisplays().findDisplayForRect (screenBounds).scale;

        return Desktop::getInstance().getGlobalScaleFactor();
    }

private:
    struct DummyComponent  : public Component
    {
        DummyComponent (NativeContext& c) : context (c) {}

        // The windowing code will call this when a paint callback happens
        void handleCommandMessage (int) override   { context.triggerRepaint(); }

        NativeContext& context;
    };

    std::unique_ptr<DummyComponent> dummyComponent;
    std::unique_ptr<ComponentPeer> nativeWindow;
    HGLRC renderContext;
    HDC dc;
    OpenGLContext* context = {};

    double nativeScaleFactor = 1.0;

   #if JUCE_WIN_PER_MONITOR_DPI_AWARE
    Component::SafePointer<Component> safeComponent;
   #endif

    #define JUCE_DECLARE_WGL_EXTENSION_FUNCTION(name, returnType, params) \
        typedef returnType (__stdcall *type_ ## name) params; type_ ## name name;

    JUCE_DECLARE_WGL_EXTENSION_FUNCTION (wglChoosePixelFormatARB,  BOOL, (HDC, const int*, const FLOAT*, UINT, int*, UINT*))
    JUCE_DECLARE_WGL_EXTENSION_FUNCTION (wglSwapIntervalEXT,       BOOL, (int))
    JUCE_DECLARE_WGL_EXTENSION_FUNCTION (wglGetSwapIntervalEXT,    int, ())
    JUCE_DECLARE_WGL_EXTENSION_FUNCTION (wglCreateContextAttribsARB, HGLRC, (HDC, HGLRC, const int *)) /**SMODE*/
    #undef JUCE_DECLARE_WGL_EXTENSION_FUNCTION

   #if JUCE_WIN_PER_MONITOR_DPI_AWARE
    void timerCallback() override
    {
        if (safeComponent != nullptr)
        {
            if (auto* peer = safeComponent->getTopLevelComponent()->getPeer())
            {
                auto newScale = peer->getPlatformScaleFactor();

                if (juce_shouldDoubleScaleNativeGLWindow())
                    newScale *= newScale;

                if (! approximatelyEqual (newScale, nativeScaleFactor))
                {
                    nativeScaleFactor = newScale;
                    updateWindowPosition (peer->getAreaCoveredBy (*safeComponent));
                }
            }
        }
    }
   #endif

    void initialiseGLExtensions()
    {
        #define JUCE_INIT_WGL_FUNCTION(name)    name = (type_ ## name) OpenGLHelpers::getExtensionFunction (#name);
        JUCE_INIT_WGL_FUNCTION (wglChoosePixelFormatARB);
        JUCE_INIT_WGL_FUNCTION (wglSwapIntervalEXT);
        JUCE_INIT_WGL_FUNCTION (wglGetSwapIntervalEXT);
        JUCE_INIT_WGL_FUNCTION (wglCreateContextAttribsARB); /**SMODE*/
        #undef JUCE_INIT_WGL_FUNCTION

        // SMODE debug api
#ifdef JUCE_DEBUG
        #define JUCE_INIT_GL_FUNCTION(name, returnType, params, callparams) \
          name = (type_ ## name) OpenGLHelpers::getExtensionFunction (#name);

        JUCE_GL_DEBUG_FUNCTIONS(JUCE_INIT_GL_FUNCTION)
        #undef JUCE_INIT_GL_FUNCTION
#endif // JUCE_DEBUG
    }

    void createNativeWindow (Component& component)
    {
        auto* topComp = component.getTopLevelComponent();
        nativeWindow.reset (createNonRepaintingEmbeddedWindowsPeer (*dummyComponent, topComp->getWindowHandle()));

        if (auto* peer = topComp->getPeer())
        {
           #if JUCE_WIN_PER_MONITOR_DPI_AWARE
            safeComponent = Component::SafePointer<Component> (&component);
            nativeScaleFactor = peer->getPlatformScaleFactor();

            if (juce_shouldDoubleScaleNativeGLWindow())
                nativeScaleFactor *= nativeScaleFactor;

            startTimer (50);
           #endif

            updateWindowPosition (peer->getAreaCoveredBy (component));
        }

        nativeWindow->setVisible (true);
        dc = GetDC ((HWND) nativeWindow->getNativeHandle());
    }

    HGLRC createVersionnedContext(HDC dc, HGLRC contextToShareWith, OpenGLVersion version) // SMODE
    {
#ifdef JUCE_OPENGL3
      if (version == openGL3_2)
      {
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_FLAGS_ARB 0x2094
        int attribs[64];
        int numAttribs = 0;

        attribs[numAttribs++] = WGL_CONTEXT_MAJOR_VERSION_ARB; attribs[numAttribs++] = 3;
        attribs[numAttribs++] = WGL_CONTEXT_MINOR_VERSION_ARB; attribs[numAttribs++] = 3;
        attribs[numAttribs++] = WGL_CONTEXT_PROFILE_MASK_ARB; attribs[numAttribs++] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
        attribs[numAttribs++] = WGL_CONTEXT_FLAGS_ARB; attribs[numAttribs++] = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#ifdef JUCE_DEBUG
          | WGL_CONTEXT_DEBUG_BIT_ARB
#endif //JUCE_DEBUG
          ;
        attribs[numAttribs++] = 0;
        HGLRC arbRC = wglCreateContextAttribsARB(dc, contextToShareWith, attribs);
        jassert(arbRC != NULL);
        return arbRC;
      }
      else
#endif // JUCE_OPENGL3
        return wglCreateContext (dc);
    }

    void deleteRenderContext()
    {
        if (renderContext != 0)
        {
            wglDeleteContext (renderContext);
            renderContext = 0;
        }
    }

    void releaseDC()
    {
        ReleaseDC ((HWND) nativeWindow->getNativeHandle(), dc);
    }

    static void initialisePixelFormatDescriptor (PIXELFORMATDESCRIPTOR& pfd, const OpenGLPixelFormat& pixelFormat)
    {
        zerostruct (pfd);
        pfd.nSize           = sizeof (pfd);
        pfd.nVersion        = 1;
        pfd.dwFlags         = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        // SMODE 
        pfd.dwFlags |= PFD_SWAP_EXCHANGE; // supposed to be faster on NV SLI but may be slower/problematic on ATI
        // SMODE
        pfd.iPixelType      = PFD_TYPE_RGBA;
        pfd.iLayerType      = PFD_MAIN_PLANE;
        pfd.cColorBits      = (BYTE) (pixelFormat.redBits + pixelFormat.greenBits + pixelFormat.blueBits);
        pfd.cRedBits        = (BYTE) pixelFormat.redBits;
        pfd.cGreenBits      = (BYTE) pixelFormat.greenBits;
        pfd.cBlueBits       = (BYTE) pixelFormat.blueBits;
        pfd.cAlphaBits      = (BYTE) pixelFormat.alphaBits;
        pfd.cDepthBits      = (BYTE) pixelFormat.depthBufferBits;
        pfd.cStencilBits    = (BYTE) pixelFormat.stencilBufferBits;
        pfd.cAccumBits      = (BYTE) (pixelFormat.accumulationBufferRedBits + pixelFormat.accumulationBufferGreenBits
                                        + pixelFormat.accumulationBufferBlueBits + pixelFormat.accumulationBufferAlphaBits);
        pfd.cAccumRedBits   = (BYTE) pixelFormat.accumulationBufferRedBits;
        pfd.cAccumGreenBits = (BYTE) pixelFormat.accumulationBufferGreenBits;
        pfd.cAccumBlueBits  = (BYTE) pixelFormat.accumulationBufferBlueBits;
        pfd.cAccumAlphaBits = (BYTE) pixelFormat.accumulationBufferAlphaBits;
    }

    int wglChoosePixelFormatExtension (const OpenGLPixelFormat& pixelFormat) const
    {
        int format = 0;

        if (wglChoosePixelFormatARB != nullptr)
        {
            int atts[64];
            int n = 0;

            atts[n++] = WGL_DRAW_TO_WINDOW_ARB;   atts[n++] = GL_TRUE;
            atts[n++] = WGL_SUPPORT_OPENGL_ARB;   atts[n++] = GL_TRUE;
            atts[n++] = WGL_DOUBLE_BUFFER_ARB;    atts[n++] = GL_TRUE;
            atts[n++] = WGL_PIXEL_TYPE_ARB;       atts[n++] = WGL_TYPE_RGBA_ARB;
            atts[n++] = WGL_ACCELERATION_ARB;
            atts[n++] = WGL_FULL_ACCELERATION_ARB;

            atts[n++] = WGL_COLOR_BITS_ARB;  atts[n++] = pixelFormat.redBits + pixelFormat.greenBits + pixelFormat.blueBits;
            atts[n++] = WGL_RED_BITS_ARB;    atts[n++] = pixelFormat.redBits;
            atts[n++] = WGL_GREEN_BITS_ARB;  atts[n++] = pixelFormat.greenBits;
            atts[n++] = WGL_BLUE_BITS_ARB;   atts[n++] = pixelFormat.blueBits;
            atts[n++] = WGL_ALPHA_BITS_ARB;  atts[n++] = pixelFormat.alphaBits;
            atts[n++] = WGL_DEPTH_BITS_ARB;  atts[n++] = pixelFormat.depthBufferBits;

            atts[n++] = WGL_STENCIL_BITS_ARB;       atts[n++] = pixelFormat.stencilBufferBits;
            atts[n++] = WGL_ACCUM_RED_BITS_ARB;     atts[n++] = pixelFormat.accumulationBufferRedBits;
            atts[n++] = WGL_ACCUM_GREEN_BITS_ARB;   atts[n++] = pixelFormat.accumulationBufferGreenBits;
            atts[n++] = WGL_ACCUM_BLUE_BITS_ARB;    atts[n++] = pixelFormat.accumulationBufferBlueBits;
            atts[n++] = WGL_ACCUM_ALPHA_BITS_ARB;   atts[n++] = pixelFormat.accumulationBufferAlphaBits;

            if (pixelFormat.multisamplingLevel > 0
                  && OpenGLHelpers::isExtensionSupported ("GL_ARB_multisample"))
            {
                atts[n++] = WGL_SAMPLE_BUFFERS_ARB;
                atts[n++] = 1;
                atts[n++] = WGL_SAMPLES_ARB;
                atts[n++] = pixelFormat.multisamplingLevel;
            }

            // SMODE
            atts[n++] = WGL_SWAP_METHOD_ARB;   atts[n++] = 0x2028; /* WGL_SWAP_EXCHANGE_ARB */; // supposed to be faster on NV SLI but may be slower/problematic on ATI
            // SMODE

            atts[n++] = 0;
            jassert (n <= numElementsInArray (atts));

            UINT formatsCount = 0;
            wglChoosePixelFormatARB (dc, atts, nullptr, 1, &format, &formatsCount);
        }

        return format;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext)
};


//==============================================================================
bool OpenGLHelpers::isContextActive()
{
    return wglGetCurrentContext() != 0;
}

} // namespace juce
