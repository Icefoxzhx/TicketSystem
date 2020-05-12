$(function(){
	$("#devicepropman_add").ajaxForm(function(response_text)
	{
		var zz = /success/i;
		if (zz.test(response_text))
			swal(response_text, "信息添加成功！", "success");
		else
			swal(response_text, "信息添加失败，请检查您的输入！", "error");
	});
});