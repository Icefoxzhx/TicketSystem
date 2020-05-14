$(function(){
	$("#newcalc").ajaxForm(function(response_text)
	{
		swal("提示:共计字符" + response_text);
		document.getElementById('count').innerHTML = response_text;
	});
});