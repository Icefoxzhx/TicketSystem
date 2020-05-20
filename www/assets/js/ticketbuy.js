$(function(){
	$("#ticketbuy").ajaxForm(function(response_text)
	{
		alert(response_text);
		if(response_text == "queue")
			swal("Success","您位于候补购票队列中！","success");
		else if(response_text == -1)
			swal("Oops","购票失败，余票不足！","error");
		else
			swal("Success","购票成功，总金额为"+response_text, "success");
	});
});