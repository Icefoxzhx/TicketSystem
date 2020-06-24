var main_json = [];
var t_length = 0;
$(function(){
	$("#ticketinquire").ajaxForm(function(response_text)
	{
		alert(response_text);
		if(response_text == "-1")
		{
			swal("Error!","未找到对应信息","error");
			$("#card_inquire").slideUp(500);
			refresh_main_table();
			main_json = [];
			t_length = 0;
			ticketinquire_show_total();
			$("#ticketinquire_show_total").slideDown(500);
		}
		else
		{
			raw_json = response_text;
			main_json = JSON.parse(raw_json);
			if(main_json[0].total_num == 0)
			{
				swal("Error!","未找到对应信息","error");
				$("#card_inquire").slideUp(500);
				refresh_main_table();
				main_json = [];
				t_length = 0;
				ticketinquire_show_total();
				$("#ticketinquire_show_total").slideDown(500);
			}
			else
			{
				t_length = main_json[0].total_num;
				refresh_main_table();
				ticketinquire_show_total();
				ticketinquire_write_table(main_json[0].total_num);
				$("#ticketinquire_show_total").slideDown(500);
				$("#card_inquire").slideDown(500);
			}
		}
	 });
});
function refresh_main_table()
{
	$("tr:not(:first)").remove();
}

function ticketinquire_show_total()
{
	$("#ticketinquire_total").show();
	document.getElementById('ticketinquire_total').innerHTML = "共查询到<b>" + t_length + "</b>张车票";
}


var tab =document.getElementById("ticketinquire_main_table");
var tr = new Array();
tr[1] = 'abc';
function ticketinquire_write_table(m)
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
	$("#card_inquire").hide();
	$("#ticketinquire_total").hide();
});