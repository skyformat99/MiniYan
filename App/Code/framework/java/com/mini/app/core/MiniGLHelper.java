package com.mini.app.core;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

import com.mini.app.utils.MiniLog;

import android.view.SurfaceHolder;

public class MiniGLHelper {
	private static final String TAG = "MiniGLHelper";
	
	private static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
	private int mEGLContextClientVersion = 2;
	private EGL10 mEgl;
	private EGLDisplay mEglDisplay;
	private EGLSurface mEglSurface;
	private EGLConfig mEglConfig;
	private EGLContext mEglContext;
	private EGLConfigChooser mEGLConfigChooser = null;

	public MiniGLHelper() {
	}

	// 初始化EGL环境
	public void initGLContext(SurfaceHolder holder) {
		// 创建一个EGL实例
        if (mEGLConfigChooser == null) {
            mEGLConfigChooser = new SimpleEGLConfigChooser(true);
        }
        
		mEgl = (EGL10) EGLContext.getEGL();
		mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
		// 初始化EGLDisplay
		int[] version = new int[2];
        if (!mEgl.eglInitialize(mEglDisplay, version)) {
            throw new RuntimeException("eglInitialize failed");
        }
        mEglConfig = mEGLConfigChooser.chooseConfig(mEgl, mEglDisplay);
        

		int[] attrib_list = { 
				EGL_CONTEXT_CLIENT_VERSION,
				mEGLContextClientVersion, 
				EGL10.EGL_NONE };

		mEglContext = mEgl.eglCreateContext(
				mEglDisplay, 
				mEglConfig,
				EGL10.EGL_NO_CONTEXT, 
				mEGLContextClientVersion != 0 ? attrib_list : null);

		mEglSurface = mEgl.eglCreateWindowSurface(mEglDisplay, mEglConfig,
				holder, null);
		mEgl.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext);

	}

	// 绘制完后调用，刷新屏幕
	public void postToScreen() {
		mEgl.eglSwapBuffers(mEglDisplay, mEglSurface);
		if (mEgl.eglGetError() == EGL11.EGL_CONTEXT_LOST) {
			MiniLog.d(TAG, "EGL_CONTEXT_LOST");
		}
	}

	// 销毁EGL环境
	public void destoryGL() {
		mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
				EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
		mEgl.eglDestroySurface(mEglDisplay, mEglSurface);
		mEgl.eglDestroyContext(mEglDisplay, mEglContext);
		mEgl.eglTerminate(mEglDisplay);
	}
	
    private void setEGLConfigChooser(EGLConfigChooser configChooser) {
        mEGLConfigChooser = configChooser;
    }
	
    public void setEGLConfigChooser(int redSize, int greenSize, int blueSize,
            int alphaSize, int depthSize, int stencilSize) {
        setEGLConfigChooser(new ComponentSizeChooser(redSize, greenSize,
                blueSize, alphaSize, depthSize, stencilSize));
    }

	public interface EGLConfigChooser {
		/**
		 * Choose a configuration from the list. Implementors typically
		 * implement this method by calling {@link EGL10#eglChooseConfig} and
		 * iterating through the results. Please consult the EGL specification
		 * available from The Khronos Group to learn how to call
		 * eglChooseConfig.
		 * 
		 * @param egl
		 *            the EGL10 for the current display.
		 * @param display
		 *            the current display.
		 * @return the chosen configuration.
		 */
		EGLConfig chooseConfig(EGL10 egl, EGLDisplay display);
	}

	private abstract class BaseConfigChooser implements EGLConfigChooser {
		public BaseConfigChooser(int[] configSpec) {
			mConfigSpec = filterConfigSpec(configSpec);
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
			int[] num_config = new int[1];
			if (!egl.eglChooseConfig(display, mConfigSpec, null, 0, num_config)) {
				throw new IllegalArgumentException("eglChooseConfig failed");
			}

			int numConfigs = num_config[0];

			if (numConfigs <= 0) {
				throw new IllegalArgumentException(
						"No configs match configSpec");
			}

			EGLConfig[] configs = new EGLConfig[numConfigs];
			if (!egl.eglChooseConfig(display, mConfigSpec, configs, numConfigs,
					num_config)) {
				throw new IllegalArgumentException("eglChooseConfig#2 failed");
			}
			EGLConfig config = chooseConfig(egl, display, configs);
			if (config == null) {
				throw new IllegalArgumentException("No config chosen");
			}
			return config;
		}

		abstract EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
				EGLConfig[] configs);

		protected int[] mConfigSpec;

		private int[] filterConfigSpec(int[] configSpec) {
			if (mEGLContextClientVersion != 2) {
				return configSpec;
			}
			/*
			 * We know none of the subclasses define EGL_RENDERABLE_TYPE. And we
			 * know the configSpec is well formed.
			 */
			int len = configSpec.length;
			int[] newConfigSpec = new int[len + 2];
			System.arraycopy(configSpec, 0, newConfigSpec, 0, len - 1);
			newConfigSpec[len - 1] = EGL10.EGL_RENDERABLE_TYPE;
			newConfigSpec[len] = 4; /* EGL_OPENGL_ES2_BIT */
			newConfigSpec[len + 1] = EGL10.EGL_NONE;
			return newConfigSpec;
		}
	}

	/**
	 * Choose a configuration with exactly the specified r,g,b,a sizes, and at
	 * least the specified depth and stencil sizes.
	 */
	private class ComponentSizeChooser extends BaseConfigChooser {
		public ComponentSizeChooser(int redSize, int greenSize, int blueSize,
				int alphaSize, int depthSize, int stencilSize) {
			super(new int[] {
					EGL10.EGL_RED_SIZE,
					redSize,
					EGL10.EGL_GREEN_SIZE,
					greenSize,
					EGL10.EGL_BLUE_SIZE,
					blueSize,
					EGL10.EGL_ALPHA_SIZE,
					alphaSize,
					EGL10.EGL_DEPTH_SIZE,
					depthSize,
					EGL10.EGL_STENCIL_SIZE,
					stencilSize,
					EGL10.EGL_NONE });
			mValue = new int[1];
			mRedSize = redSize;
			mGreenSize = greenSize;
			mBlueSize = blueSize;
			mAlphaSize = alphaSize;
			mDepthSize = depthSize;
			mStencilSize = stencilSize;
		}

		@Override
		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
				EGLConfig[] configs) {
			for (EGLConfig config : configs) {
				int d = findConfigAttrib(egl, display, config,
						EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config,
						EGL10.EGL_STENCIL_SIZE, 0);
				if ((d >= mDepthSize) && (s >= mStencilSize)) {
					int r = findConfigAttrib(egl, display, config,
							EGL10.EGL_RED_SIZE, 0);
					int g = findConfigAttrib(egl, display, config,
							EGL10.EGL_GREEN_SIZE, 0);
					int b = findConfigAttrib(egl, display, config,
							EGL10.EGL_BLUE_SIZE, 0);
					int a = findConfigAttrib(egl, display, config,
							EGL10.EGL_ALPHA_SIZE, 0);
					if ((r == mRedSize) && (g == mGreenSize)
							&& (b == mBlueSize) && (a == mAlphaSize)) {
						return config;
					}
				}
			}
			return null;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display,
				EGLConfig config, int attribute, int defaultValue) {

			if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
				return mValue[0];
			}
			return defaultValue;
		}

		private int[] mValue;
		// Subclasses can adjust these values:
		protected int mRedSize;
		protected int mGreenSize;
		protected int mBlueSize;
		protected int mAlphaSize;
		protected int mDepthSize;
		protected int mStencilSize;
	}
	
    /**
     * This class will choose a RGB_565 surface with
     * or without a depth buffer.
     *
     */
    private class SimpleEGLConfigChooser extends ComponentSizeChooser {
        public SimpleEGLConfigChooser(boolean withDepthBuffer) {
            super(5, 6, 5, 0, withDepthBuffer ? 16 : 0, 0);
        }
    }
}
