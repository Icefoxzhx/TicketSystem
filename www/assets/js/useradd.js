$(function(){
	$("#useradd").ajaxForm(function(response_text)
	{
		alert(response_text);
		if(response_text == 0)
			swal("Success","添加成功","success");
		else if(response_text == -1)
			swal("Oops","添加失败，请检查网络环境或联系管理员","error");
	});
});