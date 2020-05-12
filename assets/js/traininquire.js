var main_json = [];
var t_length = 0;
$(function(){
	$("#traininquire").ajaxForm(function(response_text)
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
			$("#card_inquire").show();
			raw_json = response_text;
			main_json = JSON.parse(raw_json);
			t_length = main_json.length - 1;
		}
		traininquire_show_total();
		traininquire_write_table(t_length);
	 });
});
function refresh_main_table()
{
	for(var i = 1; i <= t_length; i++)
		tab.removeChild(tr[i]);
}

function traininquire_show_total()
{
	$("#traininquire_total").show();
	document.getElementById('traininquire_total').innerHTML = main_json[0].trainid + " (" + main_json[0].type + ")";
}


var tab =document.getElementById("traininquire_main_table");
var tr = new Array();
tr[1] = 'abc';
function traininquire_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr[i] = document.createElement("tr");
		num = document.createElement("td");
		if(i == 1)
			num.innerHTML = "1  (始发站)";
		else if(i == m)
			num.innerHTML = m + "  (终点站)";
		else
			num.innerHTML = i;
		stationss = document.createElement("td");
		stationss.innerHTML = main_json[i].stations;
		arriving_timee = document.createElement("td");
		arriving_timee.innerHTML = main_json[i].arriving_time;
		leaving_timee = document.createElement("td");
		leaving_timee.innerHTML = main_json[i].leaving_time;
		pricee = document.createElement("td");
		pricee.innerHTML = main_json[i].price;
		seatt = document.createElement("td");
		seatt.innerHTML = main_json[i].seat;	
		tab.appendChild(tr[i]);
		tr[i].appendChild(num);
		tr[i].appendChild(stationss);
		tr[i].appendChild(arriving_timee);
		tr[i].appendChild(leaving_timee);
		tr[i].appendChild(pricee);
		tr[i].appendChild(seatt);
	}
}

$(function(){
	$("#card_inquire").hide();
	$("#traininquire_total").hide();
});