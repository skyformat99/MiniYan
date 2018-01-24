/**
 * Created by yanbo on 2017/9/13.
 * 实现跨域功能，使用WEB服务器作为中转代理
 * 所有HTTP请求皆使用post
 */

String.prototype.compareNoCase = function(str) {
    //不区分大小写
    return this.toLowerCase() == str.toLowerCase();
};


// 创建命名空间
var yanbo = {};
yanbo.Request = function(listener) {
    // 成员变量应如下声明
    this.listener = listener;
};

yanbo.Request.prototype = {
    // 不可如此声明成员变量
    // 此声明类似java和C++中类静态成员
    request: function (url, method, params) {
        var httpObj = { method : method, url : url };
        if (method.compareNoCase("post") && params) {
            httpObj = $.extend(httpObj, params);
        }

        var that = this;
        $.post("JSHttpUtil.php",
            httpObj,
            function (data, status) {
                //alert("data="+data);
                if (that.listener) {
                    that.listener.onDataReceived(data);
                } else {
                    console.log("no listener");
                }
            }
        );
    }
};

yanbo.Request.create = function (listener) {
    return new yanbo.Request(listener);
};