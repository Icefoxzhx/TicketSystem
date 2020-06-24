$(function(){
	$("#ticketbuy").ajaxForm(function(response_text)
	{
		if(response_text == "queue")
			swal("Success","您位于候补购票队列中！","success");
		else if(response_text == -1)
			swal("Oops","购票失败，余票不足！","error");
		else
			swal("Success","购票成功，总金额为"+response_text, "success");
	});
});
$(function(){
	var date_now = new Date();
	var year = date_now.getFullYear();
	var month = date_now.getMonth()+1 < 10 ? "0"+(date_now.getMonth()+1) : (date_now.getMonth()+1);
	var date = date_now.getDate() < 10 ? "0"+date_now.getDate() : date_now.getDate();
	$("#buy_date_select").attr("min", year+"-"+month+"-"+date);
});
$(function(){
	$("#buy_date_select").change(function(){
		if($(this).val() < "2020-06-01")
		{
			// swal("信息填写错误！", "日期错误！", "error");
			$(this).val("2020-06-01");
			$(this).addClass("input-focus");
		}
		else if($(this).val() > "2020-08-31")
		{
			// swal("信息填写错误！", "日期错误！", "error");
			$(this).val("2020-08-31");
			$(this).addClass("input-focus");
		}
		$("#buy_date").val($("#buy_date_select").val().substring(5));
	});
});