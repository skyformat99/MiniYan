package com.mini.app.core;

import com.mini.app.core.MiniWebView.MiniWebViewTask;
import com.mini.app.taskcore.TaskManager;
import com.mini.app.taskcore.TaskMessage;
import com.mini.app.utils.MiniLog;
import com.mini.app.utils.TaskConst;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff.Mode;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

/*
 * surfaceview和view不同，view是谁后创建谁在上面
 * surfaceview则是谁先创建谁的surface在上面，但是
 * 其view的属性保持不变，使用setZOrderMediaOverlay
 * 为true后可以使这个view的surface保持在其他媒体surface
 * 之上。
 * surfaceview的view属性默认是在surface之上，也就是
 * surfaceview中ondraw绘制的图像会覆盖在surface上
 *
 * WebSurfaceAdapter
 * @Author Yan bo
 * @Time 2014-5-4
 * @Copyright Reserved
 * @Descrption OpenGL ES 2.0 FrameWork Construct By Yanbo
 */
public class WebSurfaceAdapter extends SurfaceView implements 
    IMiniWebAdapter, SurfaceHolder.Callback {
	private static final String TAG = WebSurfaceAdapter.class.getSimpleName();
	private MiniWebView mWebView = null;
	private boolean mIsWebViewDistroy = true;
	private boolean mEnableGL = true;
	private MiniGLHelper mGLHelper = null;
	private SurfaceHolder mHolder = null;
	private String mUrl = null;

	public WebSurfaceAdapter(Context context, MiniWebView webView) {
		super(context);
		mWebView = webView;
		init();
	}

	private void init() {
		getHolder().addCallback(this);
		getHolder().setFormat(PixelFormat.TRANSLUCENT);
		// 叠在其他surfaceview之上
		setZOrderOnTop(true);
		//setZOrderMediaOverlay(true);
		if (mEnableGL) {
			mGLHelper = new MiniGLHelper();
		}
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		MiniLog.d(TAG, "surfaceCreated");
		if (mIsWebViewDistroy) {
			MiniLog.d(TAG, " sendMiniWebViewTask");
			mHolder = holder;
			
			mWebView.mTask = new MiniWebViewTask(this);
			TaskManager.getInstance().sendTask(mWebView.mTask);

			TaskManager.sendTaskMessage(TaskMessage.obtain(
					TaskConst.WEBVIEW_TASK_ID,
					MiniWebViewTask.WEBVIEW_INIT, mUrl));
			
			mIsWebViewDistroy = false;
		} else {
			MiniLog.d(TAG, " MiniWebViewTask DRAW");
			TaskManager.sendTaskMessage(new TaskMessage(
					TaskConst.WEBVIEW_TASK_ID,
					MiniWebViewTask.WEBVIEW_DRAW, null));
		}
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
	}

	@Override
	public void initWebView(TaskMessage msg) {
		if (mEnableGL) {
			mGLHelper.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
			mGLHelper.initGLContext(this.mHolder);
		}
		
		mWebView.nativeInitWebView(mHolder.getSurfaceFrame().width(),
				mHolder.getSurfaceFrame().height(),
				mWebView.mGraphicsContext, 
				mWebView.mLoader, 
				mEnableGL, 
				MiniLog.ENABLE_LOG);

		mWebView.nativeLoadUrl((String) msg.mObject);
	}

	@Override
	public void loadUrl(String url) {
		mUrl = url;
	}

	@Override
	public void receiveData(TaskMessage msg) {
		MiniLog.d(TAG, "WEBVIEW_RECEIVE");
		String data = (String) msg.mObject;
		MiniLog.d(TAG, data);
		mWebView.nativeOnDataReceive(data);
	}

	@Override
	public void quitWebView() {
		mWebView.nativeDistroyWebView();
		if (mEnableGL) {
			mGLHelper.destoryGL();
		}
	}

	@Override
	public void keyEvent(TaskMessage msg) {
		synchronized (mHolder) {
			KeyEvent evt = (KeyEvent) msg.mObject;
			if (mEnableGL) {
				MiniLog.d(TAG, "WEBVIEW_KEYEVENT");
				mWebView.nativeHandleKeyEvent(evt.getKeyCode(), 0);
				mGLHelper.postToScreen();
			} else {
				Canvas canvas = mHolder.lockCanvas();
				if (canvas != null) {
					canvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
					canvas.drawColor(mWebView.mBackgroundColor);
					mWebView.mGraphicsContext.setCanvas(canvas);
					mWebView.nativeHandleKeyEvent(evt.getKeyCode(), 0);
					mHolder.unlockCanvasAndPost(canvas);
				}
			}
		}
	}

	@Override
	public void draw() {
		synchronized (mHolder) {
			if (!mEnableGL) {
				Canvas canvas = mHolder.lockCanvas();
				if (canvas != null) {
					MiniLog.d(TAG, "Canvas not null");
					canvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
					canvas.drawColor(mWebView.mBackgroundColor);
					mWebView.mGraphicsContext.setCanvas(canvas);

					mWebView.nativeWebViewDraw();
					MiniLog.d(TAG, "paint end");
					mHolder.unlockCanvasAndPost(canvas);
					MiniLog.d(TAG, "post to screen");
				}

			} else {
				mWebView.nativeWebViewDraw();
				mGLHelper.postToScreen();
			}
		}
	}

	@Override
	public View getView() {
		return this;
	}

	@Override
	public void loadFinishd(TaskMessage msg) {
		synchronized (mHolder) {
			if (!mEnableGL) {
				Canvas canvas = mHolder.lockCanvas();
				if (canvas != null) {
					MiniLog.d(TAG, "Canvas not null");
					canvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
					canvas.drawColor(mWebView.mBackgroundColor);
					mWebView.mGraphicsContext.setCanvas(canvas);

					mWebView.nativeOnDataFinished();
	
					MiniLog.d(TAG, "paint end");
					mHolder.unlockCanvasAndPost(canvas);
					MiniLog.d(TAG, "post to screen");
				}

			} else {
				mWebView.nativeOnDataFinished();
				mGLHelper.postToScreen();
			}
		}
	}
	
	public void quit() {
		TaskManager.sendTaskMessage(TaskMessage.obtain(
				TaskConst.WEBVIEW_TASK_ID, MiniWebViewTask.WEBVIEW_QUIT, null));

		mIsWebViewDistroy = true;
	}
}
