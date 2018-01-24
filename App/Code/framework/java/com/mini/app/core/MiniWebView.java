package com.mini.app.core;

import java.io.UnsupportedEncodingException;

import com.mini.app.http.HttpUtil;
import com.mini.app.loader.ILoaderCallback;
import com.mini.app.loader.Loader;
import com.mini.app.taskcore.TaskBase;
import com.mini.app.taskcore.TaskManager;
import com.mini.app.taskcore.TaskMessage;
import com.mini.app.utils.MiniLog;
import com.mini.app.utils.TaskConst;

import android.content.Context;
import android.graphics.Color;

import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.View;


public class MiniWebView implements ILoaderCallback {
	protected static final String TAG = "MiniWebView";
	protected MiniGraphicsContext mGraphicsContext = null;
	protected Loader mLoader = null;
	protected MiniWebViewTask mTask = null;
	protected int mBackgroundColor = Color.WHITE;
	
	private IMiniWebAdapter mAdapter = null;

	public MiniWebView(Context context) {
		mGraphicsContext = new MiniGraphicsContext();
		mLoader = new Loader(this);
		mAdapter = new WebSurfaceAdapter(context, this);
	}
	
	public void setBackgroundColor(int color) {
		mBackgroundColor = color;
	}

	public native void nativeInitWebView(int width, int height,
			MiniGraphicsContext gc, Loader engine, boolean enableGL, boolean isDebug);

	public native void nativeOnDataReceive(String data);

	public native void nativeOnDataFinished();

	public native void nativeLoadUrl(String url);
	
	public native void nativeWebViewDraw();

	public native void nativeHandleKeyEvent(int keyCode, int isDown);
	
	public native void nativeDistroyWebView();

	public void loadUrl(String url) {
		mAdapter.loadUrl(url);
	}

	public void handleKeyEvent(KeyEvent evt) {
		TaskManager.sendTaskMessage(TaskMessage.obtain(
				TaskConst.WEBVIEW_TASK_ID,
				MiniWebViewTask.WEBVIEW_KEYEVENT, evt));
	}

	public static class MiniWebViewTask extends TaskBase {
		private static final String TAG = "MiniWebViewTask";
		public static final int WEBVIEW_WAIT = 0x0;
		public static final int WEBVIEW_INIT = 0x1;
		public static final int WEBVIEW_DRAW = 0x2;
		public static final int WEBVIEW_QUIT = 0x3;
		public static final int WEBVIEW_RECEIVE = 0x4;
		public static final int WEBVIEW_KEYEVENT = 0x5;
		public static final int WEBVIEW_LOAD_FINISHED = 0x6;
		private IMiniWebAdapter mAdapter;

		public MiniWebViewTask(IMiniWebAdapter adapter) {
			setContinue(true);

			this.setTaskID(TaskConst.WEBVIEW_TASK_ID);
			mAdapter = adapter;
		}

		@Override
		public TaskMessage handleMessage() {
			TaskMessage msg = getMessage();

			if (msg == null) {
				return super.handleMessage();
			}

			switch (msg.mStatus) {
			case WEBVIEW_INIT:
				MiniLog.d(TAG, "WEBVIEW_INIT");
				mAdapter.initWebView(msg);
				break;
			case WEBVIEW_LOAD_FINISHED:
				MiniLog.d(TAG, "WEBVIEW_LOAD_FINISHED");
				mAdapter.loadFinishd(msg);
				break;
			case WEBVIEW_DRAW: 
				MiniLog.d(TAG, "WEBVIEW_DRAW");
				mAdapter.draw();
				break;
			case WEBVIEW_WAIT:
				MiniLog.d(TAG, "WEBVIEW_WAIT");
				break;
			case WEBVIEW_RECEIVE:
				MiniLog.d(TAG, "WEBVIEW_RECEIVE");
				mAdapter.receiveData(msg);
				break;
			case WEBVIEW_QUIT:
				MiniLog.d(TAG, "WEBVIEW_QUIT");
				mAdapter.quitWebView();
				setContinue(false);
				break;
			case WEBVIEW_KEYEVENT:
				mAdapter.keyEvent(msg);
				break;
			default:
				break;
			}

			return msg;
		}
	}

	@Override
	public void onLoaderFinished(byte[] data) {
		try {
			TaskManager.sendTaskMessage(TaskMessage.obtain(
					TaskConst.WEBVIEW_TASK_ID, MiniWebViewTask.WEBVIEW_RECEIVE,
					new String(data, HttpUtil.HTTP_CHARSET_UTF8)));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

		TaskManager.sendTaskMessage(TaskMessage.obtain(
				TaskConst.WEBVIEW_TASK_ID, MiniWebViewTask.WEBVIEW_LOAD_FINISHED,
				null));
	}

	@Override
	public void onLoaderError(String error) {
		MiniLog.d(TAG, "load error=" + error);
	}

	@Override
	public void onLoaderStart() {
	}

	@Override
	public void onLoaderSizeReturn(int size) {
	}

	@Override
	public void onLoaderDataReceive(byte[] data, int length) {
	}

	@Override
	public void onLoaderRedirectUrl(String redirectUrl) {
	}
	
	public View getView() {
		return mAdapter.getView();
	}
	
	public void quitWebView() {
		mAdapter.quit();
	}

}
