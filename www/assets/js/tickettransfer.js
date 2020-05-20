var main_json = [];
var t_length = 0;
$(function(){
	$("#tickettransfer").ajaxForm(function(response_text)
	{
		alert(response_text);
		refresh_main_table();
		if(response_text == "no")
		{
			swal("Error!","未找到对应信息","error");
			main_json = [];
		}
		else
		{
			$("#card_transfer").show();
			raw_json = response_text;
			main_json = JSON.parse(raw_json);
			t_length = 2;
		}
		tickettransfer_show_total();
		tickettransfer_write_table(2);
	 });
});
function refresh_main_table()
{
	for(var i = 1; i <= t_length; i++)
		tab.removeChild(tr[i]);
}

function tickettransfer_show_total()
{
	$("#tickettransfer_total").show();
	document.getElementById('tickettransfer_total').innerHTML = "为您查询到最优车票：";
}


var tab =document.getElementById("tickettransfer_main_table");
var tr = new Array();
tr[1] = 'abc';
function tickettransfer_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr[i] = document.createElement("tr");
		num = document.createElement("td");
		num.innerHTML = i;
		trainid = document.createElement("td");
		trainid.innerHTML = main_json[i].trainid;
		start_station = document.createElement("td");
		start_station.innerHTML = main_json[i].start_station;
		start_station_time = document.createElement("td");
		start_station_time.innerHTML = main_json[i].start_station_time;
		end_station = document.createElement("td");
		end_station.innerHTML = main_json[i].end_station;
		end_station_time = document.createElement("td");
		end_station_time.innerHTML = main_json[i].end_station_time;
		pricee = document.createElement("td");
		pricee.innerHTML = main_json[i].price;
		seatt = document.createElement("td");
		seatt.innerHTML = main_json[i].seat;	
		tab.appendChild(tr[i]);
		tr[i].appendChild(num);
		tr[i].appendChild(trainid);
		tr[i].appendChild(start_station);
		tr[i].appendChild(start_station_time);
		tr[i].appendChild(end_station);
		tr[i].appendChild(end_station_time);
		tr[i].appendChild(pricee);
		tr[i].appendChild(seatt);
	}
}

$(function(){
	$("#card_transfer").hide();
	$("#tickettransfer_total").hide();
});