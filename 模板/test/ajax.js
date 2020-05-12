$(function(){
	$("#login").ajaxForm(function(data)
	{
		alert("post success." + data);
	});
});