package com.mini.app.core;

import com.mini.app.taskcore.TaskMessage;

import android.view.View;

public interface IMiniWebAdapter {
	void loadUrl(String url);
    void initWebView(TaskMessage msg);
    void receiveData(TaskMessage msg);
    void loadFinishd(TaskMessage msg);
    void quitWebView();
    void keyEvent(TaskMessage msg);
    void draw();
    View getView();
    void quit();
}
