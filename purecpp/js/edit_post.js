layui.config({base: './purecpp/plug/laymd/'}).use(['layer', 'laymd'], function(){
    var $ = layui.$,
        layer = layui.layer,
        laymd = layui.laymd;

    //实例化编辑器,可以多个实例
    var md = laymd.init('laymd_content', {});

    //内容改变事件
    md.on('change', function () {
        //这里借用marked.js解析效率比HyperDown快,用户可自行找解析器
        this.setPreview(marked(this.getText()));
    });

    //初始化数据预览
    md.do('change');
});