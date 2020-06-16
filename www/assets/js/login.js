$(function(){
	$("#login").ajaxForm(function(response_text)
	{
		if(response_text == 0)
		{
			swal("Success","登录成功，2秒后跳转主页","success");
			setTimeout(function(){window.location.replace("../../index.html");},2000);
		}
		else if(response_text == -1)
			swal("Oops","用户名或密码错误","error");
	});
});