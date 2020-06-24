$(function(){
	$("#index_ticket_inquire").click(function(){
		var tmpstr = "ticketinquire.html";
		tmpstr += "?index_start_station=" + $("#index_start_station").val();
		tmpstr += "&index_end_station=" + $("#index_end_station").val();
		tmpstr += "&index_start_date=" + $("#index_start_date").val();
		window.location.href = tmpstr;
	});
});

$(function(){
	$("#index_ticket_transfer").click(function(){
		var tmpstr = "tickettransfer.html";
		tmpstr += "?index_start_station=" + $("#index_start_station").val();
		tmpstr += "&index_end_station=" + $("#index_end_station").val();
		tmpstr += "&index_start_date=" + $("#index_start_date").val();
		window.location.href = tmpstr;
	});
});