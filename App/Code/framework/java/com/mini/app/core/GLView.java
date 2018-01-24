package com.mini.app.core;

import android.content.Context;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Created by yanbo on 2017/6/2.
 */

public class GLView extends SurfaceView implements SurfaceHolder.Callback {
    private static final int INIT_GL = 0x1;
    private static final int SURFACE_CHANGE = 0x2;

    private boolean mEnableGL = true;
    private HandlerThread mGLThread = null;
    private MiniGLHelper mGLHelper = null;
    private GLHandler mHandler = null;

    public GLView(Context context) {
        super(context);
        initView();
    }

    public void setGL(boolean isGL) {
        mEnableGL = isGL;
    }

    private void initView() {
        if (mEnableGL) {
            mGLThread = new HandlerThread("GL_THREAD");
            mGLThread.start();

            mHandler = new GLHandler(mGLThread.getLooper());
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
                               int height) {
        Message msg = Message.obtain();
        msg.what = SURFACE_CHANGE;
        msg.obj = holder;
        msg.arg1 = width;
        msg.arg2 = height;
        mHandler.sendMessage(msg);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Message msg = Message.obtain();
        msg.what = INIT_GL;
        msg.obj = holder;
        mHandler.sendMessage(msg);
    }

    private class GLHandler extends Handler {
        public GLHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case INIT_GL:
                    SurfaceHolder holder = (SurfaceHolder) msg.obj;
                    mGLHelper.initGLContext(holder);
                    break;
            }
        }
    }
}
