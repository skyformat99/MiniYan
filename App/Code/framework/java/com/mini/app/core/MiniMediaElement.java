package com.mini.app.core;

import java.io.IOException;

import com.mini.app.taskcore.TaskMessage;
import com.mini.app.utils.MiniLog;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnBufferingUpdateListener;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnInfoListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnSeekCompleteListener;
import android.media.MediaPlayer.OnVideoSizeChangedListener;
import android.net.Uri;
import android.opengl.GLES20;
import android.view.Surface;

public class MiniMediaElement implements OnBufferingUpdateListener,
        OnCompletionListener, 
        OnPreparedListener, 
        OnVideoSizeChangedListener, 
        OnErrorListener,
        OnSeekCompleteListener, 
        OnInfoListener {
    private MediaPlayer mPlayer                        = null;
    private SurfaceTexture mTexture                    = null;
    private int[] mTextureNames                        = null;
    private Context mContext                           = null;
    private Uri mPlayerUri                             = null;
    private OnFrameAvailableListener mFrameListener    = null;
    
    public MiniMediaElement(Context context, OnFrameAvailableListener frameListener) {    
    	mContext = context;
    	mFrameListener = frameListener;
    }
    
    public void startPlayer(String url) {
    	if (mPlayerUri == null) {
    		mPlayerUri = Uri.parse(url);
    	}
    	
    	if (mPlayer != null) {
    		prepare();
    	}
    }
    
    public void initPlayer() {
        mTextureNames = new int[1];
        GLES20.glGenTextures(1, mTextureNames, 0);
        mTexture = new SurfaceTexture(mTextureNames[0]);
        mTexture.setOnFrameAvailableListener(mFrameListener);
    	mPlayer = new MediaPlayer();
    	mPlayer.setSurface(new Surface(mTexture));
    }
    
    public void prepare() {
    	initListener();
    	
    	try {
			mPlayer.setDataSource(mContext, mPlayerUri);
	    	mPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC); 
	    	mPlayer.setScreenOnWhilePlaying(true);
	    	mPlayer.prepareAsync();  
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (IllegalStateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

    }
    
    private void initListener() {
    	mPlayer.setOnBufferingUpdateListener(this);
    	mPlayer.setOnCompletionListener(this);
    	mPlayer.setOnPreparedListener(this);
    	mPlayer.setOnVideoSizeChangedListener(this);
    	mPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
    	mPlayer.setOnErrorListener(this);
    	mPlayer.setOnSeekCompleteListener(this);
    	mPlayer.setOnInfoListener(this);
    }
    
    public SurfaceTexture getTexture() {
    	return mTexture;
    }
    
    public int getTextureId() {
    	return mTextureNames[0];
    }

	@Override
	public boolean onInfo(MediaPlayer mp, int what, int extra) {
		return false;
	}

	@Override
	public void onSeekComplete(MediaPlayer mp) {
		
	}

	@Override
	public boolean onError(MediaPlayer mp, int what, int extra) {
		return false;
	}

	@Override
	public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {
		
	}

	@Override
	public void onPrepared(MediaPlayer mp) {
		MiniLog.d("FontGLSurfaceView", "onPrepared");
		mp.start();
	}

	@Override
	public void onCompletion(MediaPlayer mp) {
		
	}

	@Override
	public void onBufferingUpdate(MediaPlayer mp, int percent) {
	}
}
