var main_json = [];
var t_length = 0;
$(function(){
	$("#traininquire").ajaxForm(function(response_text)
	{
		alert(response_text);
		refresh_main_table();
		if(response_text == "-1")
		{
			swal("Error!","未找到对应信息","error");
			$("#card_inquire").slideUp(500);
			$("#traininquire_total").slideUp(500);
			$("#traininquire_operation").slideUp(500);
			refresh_main_table();
			main_json = [];
			t_length = 0;
		}
		else
		{
			raw_json = response_text;
			main_json = JSON.parse(raw_json);
			t_length = main_json.length - 1;
			traininquire_show_total();
			traininquire_write_table(t_length);
			$("#traininquire_total").slideDown(500);
			$("#traininquire_operation").slideDown(500);
			$("#card_inquire").slideDown(500);
		}
	 });
});
function refresh_main_table()
{
	$("tr:not(:first)").remove();
}

function traininquire_show_total()
{
	document.getElementById('traininquire_total').innerHTML = "为您查询到：" + main_json[0].trainid + " (" + main_json[0].type + ")";
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
	$("#traininquire_operation").hide();
});

$(function(){
	$("#train_release").click(function(){
		$.ajax({
	        type: "POST",
	        dataType: "text",
	        url: "trainrelease.php", 
	        data: {
	        	trainrelease_id: main_json[0].trainid
	        },
	        success: function (release_response_text) {
				if(release_response_text == "-1")
					swal("Oops", "发布车次失败，可能已经发布过！", "error");
				else if(release_response_text == "0")
					swal("Success", "发布车次成功！", "success");
	        },
	    });
	});
});
$(function(){
	$("#train_delete").click(function(){
		$.ajax({
	        type: "POST",
	        dataType: "text",
	        url: "traindelete.php", 
	        data: {
	        	traindelete_id: main_json[0].trainid
	        },
	        success: function (delete_response_text) {
				if(delete_response_text == "-1")
					swal("Oops", "删除车次失败，可能已经删除！", "error");
				else if(delete_response_text == "0")
					swal("Success", "删除车次成功！", "success");
	        },
	    });
	});
});