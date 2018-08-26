prettyPrint();
layui.use(['form', 'layedit'], function () {
    var form = layui.form();
    var $ = layui.jquery;
    var layedit = layui.layedit;

    //评论和留言的编辑器
    var editIndex = layedit.build('remarkEditor', {
        height: 150,
        tool: [
		  'strong' //加粗
		  ,'italic' //斜体
		  ,'underline' //下划线
		  ,'del' //删除线
		  
		  ,'|' //分割线
		  
		  ,'left' //左对齐
		  ,'center' //居中对齐
		  ,'right' //右对齐
		  ,'link' //超链接
		  ,'unlink' //清除链接
		  ,'face' //表情
		  ,'code' //code
		],
    });
    //评论和留言的编辑器的验证
    layui.form().verify({
        content: function (value) {
            value = $.trim(layedit.getText(editIndex));
            if (value == "") return " 至少得有一个字吧";
            layedit.sync(editIndex);
        }
    });

    //监听评论提交
    form.on('submit(formRemark)', function (data) {
        data.form.submit();
        return false;
    });
});
