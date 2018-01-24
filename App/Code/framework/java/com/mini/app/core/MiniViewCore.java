package com.mini.app.core;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.URL;
import java.util.zip.ZipFile;

import com.mini.app.MiniAppActivity;
import com.mini.app.MiniApplication;
import com.mini.app.taskcore.TaskBase;
import com.mini.app.taskcore.TaskManager;
import com.mini.app.taskcore.TaskMessage;
import com.mini.app.utils.MiniLog;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;

public class MiniViewCore {

	private Context mContext;
	private AssetManager mAssetManager = null;

	public MiniViewCore(Context c) {
		mContext = c;
		mAssetManager = mContext.getAssets();
		nativeGetAssetManager(mAssetManager);
	}

	public void updateWidthPatch() {
		TaskManager.getInstance().sendTask(new TaskBase() {
			@Override
			public void taskRun() {
				nativeUpdatePatch(MiniApplication.getInstance()
						.getApplicationContext().getPackageResourcePath(),
						Environment.getExternalStorageDirectory().getPath()
								+ "/tmp/MiniBabyTmp.apk", Environment
								.getExternalStorageDirectory().getPath()
								+ "/tmp/MiniBaby.patch");
			}
		});
	}

	public native void init(int w, int h);

	public native void step();

	public native void handleAndroidEvent(int type, float x, float y);

	public native void createShape();

	private native void nativeGetAssetManager(AssetManager assetManager);

	private native void nativeUpdatePatch(String oldApkPath, String newApkPath,
			String patchFilePath);
}
