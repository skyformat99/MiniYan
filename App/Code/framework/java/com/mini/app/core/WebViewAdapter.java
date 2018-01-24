package com.mini.app.core;

import com.mini.app.core.MiniWebView.MiniWebViewTask;
import com.mini.app.taskcore.TaskManager;
import com.mini.app.taskcore.TaskMessage;
import com.mini.app.utils.MiniLog;
import com.mini.app.utils.TaskConst;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Bitmap.Config;
import android.graphics.Paint;
import android.graphics.PorterDuff.Mode;
import android.graphics.Canvas;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;

// View 的双缓冲机制
public class WebViewAdapter extends View implements IMiniWebAdapter {
	private MiniWebView mWebView;
	private Bitmap mCacheBitmap = null;
	private Canvas mCacheCanvas = null;

	public WebViewAdapter(Context context, MiniWebView webView) {
		super(context);
		mWebView = webView;
		init();
	}

	private void init() {
		WindowManager wm = ((Activity) getContext()).getWindowManager();
		mWebView.mTask = new MiniWebViewTask(this);
		TaskManager.getInstance().sendTask(mWebView.mTask);
		mCacheBitmap = Bitmap.createBitmap(
				wm.getDefaultDisplay().getWidth(), 
				wm.getDefaultDisplay().getHeight(),
				Config.ARGB_8888);
		mCacheCanvas = new Canvas();
		mCacheCanvas.setBitmap(mCacheBitmap);
	}

	@Override
	public void onDraw(Canvas canvas) {
		synchronized (mCacheCanvas) {
			super.onDraw(canvas);
			canvas.drawBitmap(mCacheBitmap, 0, 0, new Paint());
		}
	}

	@Override
	public void loadUrl(String url) {
		TaskManager.sendTaskMessage(TaskMessage.obtain(
				TaskConst.WEBVIEW_TASK_ID, MiniWebViewTask.WEBVIEW_INIT, url));
	}

	@Override
	public void initWebView(TaskMessage msg) {
		mWebView.nativeInitWebView(mCacheBitmap.getWidth(), 
				mCacheBitmap.getHeight(),
				mWebView.mGraphicsContext, mWebView.mLoader, false,
				MiniLog.ENABLE_LOG);

		mWebView.nativeLoadUrl((String) msg.mObject);
	}

	@Override
	public void receiveData(TaskMessage msg) {
		MiniLog.d(MiniWebView.TAG, "WEBVIEW_RECEIVE");
		String data = (String) msg.mObject;
		MiniLog.d(MiniWebView.TAG, data);
		mWebView.nativeOnDataReceive(data);
	}

	@Override
	public void loadFinishd(TaskMessage msg) {
		synchronized (mCacheCanvas) {
			mCacheCanvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
			mCacheCanvas.drawColor(mWebView.mBackgroundColor);
			mWebView.mGraphicsContext.setCanvas(mCacheCanvas);

			mWebView.nativeOnDataFinished();
		}
	}

	@Override
	public void quitWebView() {
		mWebView.nativeDistroyWebView();
	}

	@Override
	public void keyEvent(TaskMessage msg) {
		synchronized (mCacheCanvas) {
			KeyEvent evt = (KeyEvent) msg.mObject;
			mCacheCanvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
			mCacheCanvas.drawColor(mWebView.mBackgroundColor);
			mWebView.mGraphicsContext.setCanvas(mCacheCanvas);
			mWebView.nativeHandleKeyEvent(evt.getKeyCode(), 0);
			
			((Activity) getContext()).runOnUiThread(
					new Runnable() {
						@Override
						public void run() {
							invalidate();
						}
					});
		}
		
	}

	@Override
	public void draw() {
		synchronized (mCacheCanvas) {
			mCacheCanvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
			mCacheCanvas.drawColor(mWebView.mBackgroundColor);
			mWebView.mGraphicsContext.setCanvas(mCacheCanvas);

			mWebView.nativeWebViewDraw();
		}
	}

	@Override
	public View getView() {
		return this;
	}

	@Override
	public void quit() {
	}

}
