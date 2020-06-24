var main_json = [];
$(function(){
	$("#tickettransfer").ajaxForm(function(response_text)
	{
		if(response_text == "0")
		{
			swal("Error!","未找到对应换乘信息","error");
			main_json = [];
		}
		else
		{
			raw_json = response_text;
			main_json = JSON.parse(raw_json);
			$("#transfer_start_station").text(main_json[0].start_station);
			$("#transfer_start_station_time1").text(main_json[0].start_station_time);
			$("#transfer_end_station_time1").text(main_json[0].end_station_time);
			$("#transfer_seat1").text(main_json[0].seat);
			$("#transfer_price1").text(main_json[0].price);
			$("#transfer_mid_station").text(main_json[0].end_station);
			$("#transfer_start_station_time2").text(main_json[1].start_station_time);
			$("#transfer_end_station_time2").text(main_json[1].end_station_time);
			$("#transfer_seat2").text(main_json[1].seat);
			$("#transfer_price2").text(main_json[1].price);
			$("#transfer_end_station").text(main_json[1].end_station);
			$("#transfer_trainid1").text(main_json[0].trainid);
			$("#transfer_trainid2").text(main_json[1].trainid);
			$("#card_transfer").slideDown(500);
		}
		tickettransfer_show_total();
		
	 });
});
function tickettransfer_show_total()
{
	$("#tickettransfer_total").slideDown(500);
	document.getElementById('tickettransfer_total').innerHTML = "为您查询到最优车票：          (点击...查看车次详情)";
}
$(function(){
	$("#card_transfer").hide();
	$("#tickettransfer_total").hide();
});