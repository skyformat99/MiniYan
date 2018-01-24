/**
 * Created by yanbo on 2017/8/11.
 */

$(document).ready(function() {
    // 点击load按钮之后获取数据
    $("#load").click(function () {
        var n = $("input[name='name']").val();
        var p = $("input[name='pwd']").val();
        console.log("n="+n);
        console.log("p="+p);
        $.post(
            "UserInfo.php?type=1",
            {name : n, pwd : p},
            function (data, status) {
                $("#right").html(data);
            }
        );

        // 测试HTTP功能
        yanbo.Request.create({
            onDataReceived: function (data) {
                alert(data);
            }
        }).request("http://m.hao123.com", "get", null);
    });
    // div.header标签自动获取服务器地址文本
    $.get("UserInfo.php?type=0",
        function (data, status) {
            $("div.header").text("服务器地址：" + data);
        }
    );
    $("#right").on("click", ".cell", function () {
        var that = $(this);
        var text = that.text();
        $(this).html(
            "<div class='modify_content'>" +
            "<input name='modify' " +
            "type='text' />" +
            "</div>");
        var input = $("input[name='modify']");
        input.val(text);
        input.blur(function() {
            $(that).html("");
            //that.text(input.val() ? input.val() : " ");
        });
        input.focus();
    });
});
