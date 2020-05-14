$(function(){
	$("#test").ajaxForm(function(response_text)
	{
		swal("Answer is:" + response_text);
	});
});