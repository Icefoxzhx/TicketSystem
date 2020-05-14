$(function(){
	$("#traindelete").ajaxForm(function(response_text)
	{
		alert(response_text);
		if(response_text == 0)
			swal("Success","车次删除成功","success");
		else if(response_text == -1)
			swal("Oops","车次删除失败，可能未查到此车次、车次已经发布或请检查网络问题！","error");
	});
});